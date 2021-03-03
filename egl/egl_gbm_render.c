/*#############################################################
 *     File Name	: egl_gbm_render.c
 *     Author		: winddoing
 *     Created Time	: 2021年03月03日 星期三 15时18分25秒
 *     Description	:
 *  Compile: gcc egl_gbm_render.c `pkg-config --libs --cflags egl gl gbm`
 *############################################################*/
#include <assert.h>
#include <GL/gl.h>
#include <GLES3/gl31.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <gbm.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct egl_gbm {
    int m_fd;
    struct gbm_device* m_gbm_device;
    struct gbm_surface* m_gbm_surface;

    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLSurface egl_surface;

    GLuint framebuffer;
    GLuint renderbuffer;
    GLuint texture_load;
    GLuint texture_render;

    //EGLImage image;
    uint16_t m_width;
    uint16_t m_height;
    //uint16_t m_handle;
    uint32_t m_frame_cnt;
    int m_data[4];
};

void rotate_data(int data[4])
{
    int temp = data[0];
    data[0] = data[2];
    data[2] = data[3];
    data[3] = data[1];
    data[1] = temp;
}

void gl_setup_scene()
{
    // Shader source that draws a textures quad
    const char* vertex_shader_source = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoords;\n"

        "out vec2 TexCoords;\n"

        "void main()\n"
        "{\n"
        "   TexCoords = aTexCoords;\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragment_shader_source = "#version 330 core\n"
        "out vec4 FragColor;\n"

        "in vec2 TexCoords;\n"

        "uniform sampler2D Texture1;\n"

        "void main()\n"
        "{\n"
        "   FragColor = texture(Texture1, TexCoords);\n"
        "}\0";

    // vertex shader
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    // fragment shader
    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    // link shaders
    int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // quad
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Prebind needed stuff for drawing
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
}

void gl_draw_scene(GLuint texture)
{
    // clear
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw quad
    // VAO and shader program are already bound from the call to gl_setup_scene
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

static int init_egl_gbm(struct egl_gbm *R)
{
    // set OpenGL rendering API
    eglBindAPI(EGL_OPENGL_API);

    // get device fd
    R->m_fd = open("/dev/dri/renderD128", O_RDWR);

    // create gbm device
    R->m_gbm_device = gbm_create_device(R->m_fd);

    // create gbm surface
    R->m_gbm_surface = gbm_surface_create(R->m_gbm_device, R->m_width, R->m_height,
            GBM_FORMAT_ARGB8888, GBM_BO_USE_RENDERING);

    // get an EGL display with gbm
    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
        (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
    R->egl_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA, R->m_gbm_device, NULL);

    // initialize the EGL display connection
    eglInitialize(R->egl_display, NULL, NULL);

    // get the number of all EGL frame buffer configurations for a display
    EGLint num_configs;
    eglGetConfigs(R->egl_display, NULL, 0, &num_configs);

    // attributes
    EGLint egl_config_attribs[] = {
        EGL_BUFFER_SIZE,        32,
        EGL_DEPTH_SIZE,         EGL_DONT_CARE,
        EGL_STENCIL_SIZE,       EGL_DONT_CARE,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_NONE};

    // get a list of EGL frame buffer configurations that match specified attributes
    EGLConfig* configs = (EGLConfig*)malloc(num_configs * sizeof(EGLConfig));
    eglChooseConfig(R->egl_display, egl_config_attribs, configs, num_configs, &num_configs);

    // search all configurations
    EGLConfig config_use = NULL;
    for (int i = 0; i < num_configs; ++i)
    {
        EGLint gbm_format;
        eglGetConfigAttrib(R->egl_display, configs[i], EGL_NATIVE_VISUAL_ID, &gbm_format);
        if (gbm_format == GBM_FORMAT_ARGB8888) // match found
        {
            config_use = configs[i];
            free(configs);
            break;
        }
    }

    // create an EGL rendering context
    R->egl_context = eglCreateContext(R->egl_display, config_use, EGL_NO_CONTEXT, NULL);

    // create an EGL window surface with gbm
    PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC eglCreatePlatformWindowSurfaceEXT =
        (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");
    R->egl_surface = eglCreatePlatformWindowSurfaceEXT(R->egl_display, config_use, R->m_gbm_surface, NULL);

    printf("===> func: %s, line: %d, egl_surface=%p\n", __func__, __LINE__, R->egl_surface);
    // connect the context to the surface
    eglMakeCurrent(R->egl_display, R->egl_surface, R->egl_surface, R->egl_context);

}

static int init_opengl(struct egl_gbm *R)
{
    // Initialize EGL
    init_egl_gbm(R);

    // Setup GL scene
    gl_setup_scene();

    // 2X2 texture
    R->m_data[0] = 0x000000FF; // Red
    R->m_data[1] = 0x0000FF00; // Green
    R->m_data[2] = 0X00FF0000; // Blue
    R->m_data[3] = 0x00FFFFFF; // White

    // Create texture_load
    glGenTextures(1, &R->texture_load);
    glBindTexture(GL_TEXTURE_2D, R->texture_load);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, R->m_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    assert(glGetError() == GL_NO_ERROR);

#if 0
    // Set the attribute for EGLImage
    EGLAttrib const attribute[] = {
        EGL_WIDTH, R->m_width,
        EGL_HEIGHT, R->m_height,
        EGL_LINUX_DRM_FOURCC_EXT, DRM_FORMAT_ABGR8888,
        EGL_DMA_BUF_PLANE0_FD_EXT, m_handle,
        EGL_DMA_BUF_PLANE0_OFFSET_EXT, 0,
        EGL_DMA_BUF_PLANE0_PITCH_EXT, R->m_width * 4,
        EGL_NONE};

    // Create EGLImage from VA handle
    R->image = eglCreateImage(R->egl_display, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, NULL, attribute);

#endif

    // Create texture_render and bind it to the framebuffer
    glGenTextures(1, &R->texture_render);
    glBindTexture(GL_TEXTURE_2D, R->texture_render);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, R->m_width, R->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    assert(glGetError() == GL_NO_ERROR);

#if 0
    PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES =
        (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)eglGetProcAddress("glEGLImageTargetTexture2DOES");
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, R->image);
#endif

    // Create renderbuffer
    glGenRenderbuffers(1, &R->renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, R->renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, R->m_width, R->m_height);
    assert(glGetError() == GL_NO_ERROR);

    // Create framebuffer
    glGenFramebuffers(1, &R->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, R->framebuffer);
    assert(glGetError() == GL_NO_ERROR);

    // Attach texture and renderbuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, R->texture_render, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, R->renderbuffer);
    assert(glGetError() == GL_NO_ERROR);

    // Check framebuffer
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    // Swap happens as soon as the render is complete
    eglSwapInterval(R->egl_display, 0);

    // Set viewport
    glViewport(0, 0, R->m_width, R->m_height);

}

static void read_framebuffer_data(struct egl_gbm *R)
{
    static FILE *file = NULL;
    static GLbyte *pbits = NULL;  /* CPU memory to save image */
    static uint32_t frame_cnt = 0;
    uint32_t frame_size = R->m_width * R->m_height * 4;

    if (!file) {
        file = fopen("rgba.bin", "w+");
        assert(file);
        pbits = (GLbyte *)malloc(frame_size);
        assert(pbits);
    }

    glPixelStorei(GL_PACK_ALIGNMENT, 1);  /* default is 4, must set=1 */
    assert(glGetError() == GL_NO_ERROR);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    assert(glGetError() == GL_NO_ERROR);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    assert(glGetError() == GL_NO_ERROR);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
    assert(glGetError() == GL_NO_ERROR);

    GLenum lastBuffer;
    glGetIntegerv(GL_READ_BUFFER, &lastBuffer);
    assert(glGetError() == GL_NO_ERROR);
    glReadBuffer(GL_FRONT_LEFT);
    assert(glGetError() == GL_NO_ERROR);
    glReadPixels(0, 0, R->m_width, R->m_height, GL_RGBA, GL_UNSIGNED_BYTE, pbits);
    assert(glGetError() == GL_NO_ERROR);
    glReadBuffer(lastBuffer);
    assert(glGetError() == GL_NO_ERROR);

    frame_cnt++;

    size_t ret = fwrite(pbits, 1, frame_size, file);
    assert(ret > 0);

    if (R->m_frame_cnt == frame_cnt) {
        free(pbits);
        fclose(file);
    }
}


void opengl_render(struct egl_gbm *R)
{
    // load the texture
    glBindTexture(GL_TEXTURE_2D, R->texture_load);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, R->m_data);

    // Render
    gl_draw_scene(R->texture_load);

    eglSwapBuffers(R->egl_display, R->egl_surface);

    //read_framebuffer_data(R);
#if 1
    {
        static FILE *file = NULL;
        static GLbyte *pbits = NULL;  /* CPU memory to save image */
        static uint32_t frame_cnt = 0;
        uint32_t frame_size = R->m_width * R->m_height * 4;

        static GLuint rFbo = 0;

        if (!file) {
            file = fopen("rgba.bin", "w+");
            assert(file);
            pbits = (GLbyte *)malloc(frame_size);
            assert(pbits);
        }

        if (rFbo == 0) {
            glGenFramebuffers(1, &rFbo);
            assert(glGetError() == GL_NO_ERROR);
        }

        glBindFramebuffer(GL_READ_FRAMEBUFFER, rFbo);
        assert(glGetError() == GL_NO_ERROR);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, R->texture_render, 0);
        assert(glGetError() == GL_NO_ERROR);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        assert(glGetError() == GL_NO_ERROR);


        glPixelStorei(GL_PACK_ALIGNMENT, 1);  /* default is 4, must set=1 */
        assert(glGetError() == GL_NO_ERROR);
        glPixelStorei(GL_PACK_ROW_LENGTH, 0);
        assert(glGetError() == GL_NO_ERROR);
        glPixelStorei(GL_PACK_SKIP_ROWS, 0);
        assert(glGetError() == GL_NO_ERROR);
        glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
        assert(glGetError() == GL_NO_ERROR);

        glReadPixels(0, 0, R->m_width, R->m_height, GL_RGBA, GL_UNSIGNED_BYTE, pbits);
        assert(glGetError() == GL_NO_ERROR);

        frame_cnt++;

        size_t ret = fwrite(pbits, 1, frame_size, file);
        assert(ret > 0);

        if (R->m_frame_cnt == frame_cnt) {
            free(pbits);
            fclose(file);
        }
    }
#endif

    // Rotate the texture
    rotate_data(R->m_data);
}

void deinit_opengl(struct egl_gbm *R)
{
    glDeleteTextures(1, &R->texture_load);
    glDeleteTextures(1, &R->texture_render);
    glDeleteRenderbuffers(1, &R->renderbuffer);
    glDeleteFramebuffers(1, &R->framebuffer);
    eglMakeCurrent(R->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    //eglDestroyImage(R->egl_display, R->image);
    eglDestroyContext(R->egl_display, R->egl_context);
    eglDestroySurface(R->egl_display, R->egl_surface);
    eglTerminate(R->egl_display);
    gbm_device_destroy(R->m_gbm_device);
    gbm_surface_destroy(R->m_gbm_surface);
    close(R->m_fd);
}

int main(int argc, const char *argv[])
{
    int i = 0;
    struct egl_gbm render;

    render.m_width  = 64;
    render.m_height = 64;
    render.m_frame_cnt = 20;

    init_opengl(&render);

    for (i = 0; i < render.m_frame_cnt; i++) {
        opengl_render(&render);
    }

    deinit_opengl(&render);

    return 0;
}
