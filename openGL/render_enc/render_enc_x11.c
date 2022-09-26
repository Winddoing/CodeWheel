/* Copyright (C) 2022  All rights reserved.
 *
 *  File Name    : render_enc_x11.c
 *  Author       : root
 *  Created Time : 2022-09-13 10:04:13
 *  Description  :
 *      gcc render_enc.c `pkg-config --libs x11 egl gl`
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h> //sleep

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <X11/Xlib.h>
#include <assert.h>

#define DEBUG_MSG(fmt, ...) \
        do { \
            printf(fmt " (%s:%d)\n", \
                    ##__VA_ARGS__, __FUNCTION__, __LINE__); \
        } while (0)
#define ERROR_MSG(fmt, ...) \
        do { printf("ERROR: " fmt " (%s:%d)\n", \
                ##__VA_ARGS__, __FUNCTION__, __LINE__); } while (0)

#define GCHK(x) do { \
        GLenum err; \
        DEBUG_MSG(">>> %s", #x); \
        x; \
        err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            ERROR_MSG("<<< %s: failed: 0x%04x (%s)", #x, err, glStrError(err)); \
            exit(-1); \
        } \
        DEBUG_MSG("<<< %s: succeeded", #x); \
    } while (0)

struct renderer_tst {
    Display* x11_display;
    Window x11_window;
    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLSurface egl_surface;
    GLuint framebuffer;
    GLuint texture_write;
    GLuint texture_render;

    EGLImage image;
    uint16_t m_width;
    uint16_t m_height;

    unsigned char* m_pixel;
};
static uint32_t m_count = 0;

void create_x11_window(Display **x11_display, Window *x11_window, int width, int height)
{
    // Open X11 display and create window
    Display* display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, width, height, 1,
                                        BlackPixel(display, screen), WhitePixel(display, screen));
    XStoreName(display, window, "Test");
    XMapWindow(display, window);

    // Return
    *x11_display = display;
    *x11_window = window;
}

void initialize_egl(Display *x11_display, Window x11_window, EGLDisplay *egl_display, EGLContext *egl_context, EGLSurface *egl_surface)
{
    // Set OpenGL rendering API
    eglBindAPI(EGL_OPENGL_API);

    // get an EGL display connection
    EGLDisplay display = eglGetDisplay(x11_display);

    // initialize the EGL display connection
    eglInitialize(display, NULL, NULL);

    // get an appropriate EGL frame buffer configuration
    EGLConfig config;
    EGLint num_config;
    EGLint const attribute_list_config[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_NONE};
    eglChooseConfig(display, attribute_list_config, &config, 1, &num_config);

    // create an EGL rendering context
    EGLint const attrib_list[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 3,
        EGL_NONE};
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, attrib_list);

    // create an EGL window surface
    EGLSurface surface = eglCreateWindowSurface(display, config, x11_window, NULL);

    // connect the context to the surface
    eglMakeCurrent(display, surface, surface, context);

    // Return
    *egl_display = display;
    *egl_context = context;
    *egl_surface = surface;
}

void gl_setup_scene()
{
    // Shader source that draws a textures quad
    const char *vertex_shader_source = "#version 330 core\n"
                                       "layout (location = 0) in vec3 aPos;\n"
                                       "layout (location = 1) in vec2 aTexCoords;\n"

                                       "out vec2 TexCoords;\n"

                                       "void main()\n"
                                       "{\n"
                                       "   TexCoords = aTexCoords;\n"
                                       "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                       "}\0";
    const char *fragment_shader_source = "#version 330 core\n"
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
    glClear(GL_COLOR_BUFFER_BIT);

    // draw quad
    // VAO and shader program are already bound from the call to gl_setup_scene
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GenerateTexture(unsigned char* pixel, uint16_t width, uint16_t height, unsigned char count)
{
    for (int i = 0; i < width * height * 4; i = i + 4)
    {
        if (i < width * height * 4 * count/255)
        {
            pixel[i + 0] = 255;
            pixel[i + 1] = 0;
            pixel[i + 2] = 0;
            pixel[i + 3] = 0;
        }
        else if (i < width * height * 4 * 1/2)
        {
            pixel[i + 0] = 0;
            pixel[i + 1] = 255;
            pixel[i + 2] = 0;
            pixel[i + 3] = 0;
        }
        else
        {
            pixel[i + 0] = 0;
            pixel[i + 1] = 0;
            pixel[i + 2] = 255;
            pixel[i + 3] = 0;
        }
    }
}

void InitOpenGL(struct renderer_tst *render)
{
    // Generate pixel for texture
    render->m_pixel = (unsigned char*)malloc(render->m_width * render->m_height * 4);

    // Create X11 window
    create_x11_window(&render->x11_display, &render->x11_window, render->m_width, render->m_height);

    // Initialize EGL
    initialize_egl(render->x11_display, render->x11_window, &render->egl_display, &render->egl_context, &render->egl_surface);

    // Setup GL scene
    gl_setup_scene();

    // Set attribute for EGLImage
    EGLAttrib const attribute[] = {
        EGL_WIDTH, render->m_width,
        EGL_HEIGHT, render->m_height,
        EGL_NONE};

        //EGL_LINUX_DRM_FOURCC_EXT, 875708993,
        //EGL_DMA_BUF_PLANE0_FD_EXT, m_handle,
        //EGL_DMA_BUF_PLANE0_OFFSET_EXT, 0,
        //EGL_DMA_BUF_PLANE0_PITCH_EXT, m_width * 4,

    // Create EGLImage from VA handle
    render->image = eglCreateImage(render->egl_display, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, NULL, attribute);

    // Create framebuffer
    glGenFramebuffers(1, &render->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, render->framebuffer);

    // Create texture_write and load pixel into it
    glGenTextures(1, &render->texture_write);
    glBindTexture(GL_TEXTURE_2D, render->texture_write);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, render->m_width, render->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, render->m_pixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create texture_render and bind it to framebuffer
    glGenTextures(1, &render->texture_render);
    glBindTexture(GL_TEXTURE_2D, render->texture_render);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, render->m_width, render->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES =
        (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)eglGetProcAddress("glEGLImageTargetTexture2DOES");
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, render->image);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render->texture_render, 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    // Swap happens as soon as the render is complete
    eglSwapInterval(render->egl_display, 0);
}

void ReleaseOpenGL(struct renderer_tst *render)
{
    glDeleteTextures(1, &render->texture_write);
    glDeleteTextures(1, &render->texture_render);
    glDeleteFramebuffers(1, &render->framebuffer);
    //eglMakeCurrent(render->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglDestroyImage(render->egl_display, render->image);
    eglDestroyContext(render->egl_display, render->egl_context);
    eglDestroySurface(render->egl_display, render->egl_surface);
    eglTerminate(render->egl_display);

    XDestroyWindow(render->x11_display, render->x11_window);
    XCloseDisplay(render->x11_display);
}

void RenderOpenGL(struct renderer_tst *render)
{
    // Change texture
    GenerateTexture(render->m_pixel, render->m_width, render->m_height, m_count);
    glBindTexture(GL_TEXTURE_2D, render->texture_write);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, render->m_width, render->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, render->m_pixel);

    // Start Render
    //printf("===> func: %s, line: %d, file: %s\n", __func__, __LINE__, __FILE__);
    gl_draw_scene(render->texture_write);
    eglSwapBuffers(render->egl_display, render->egl_surface);

    m_count = m_count + 10;
}

void dump_texture(int tex_id, uint32_t w, uint32_t h, GLbyte* data)
{
    static GLuint rFbo = 0;

    glGetError();
    if(rFbo == 0) {
        glGenFramebuffers(1, &rFbo);
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, rFbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void save_data(GLbyte* data, int i, uint32_t w, uint32_t h)
{
    char file_name[256] = {0};
    FILE *file = NULL;
    uint32_t frame_size = w * h * 4;

    sprintf(file_name, "image_RGBA_%dx%d_%d_frames.dat", w, h, i);
    file = fopen(file_name, "w+");
    if(!file) {
        printf("Open %s failed\n", file_name);
        return;
    }

    size_t ret = fwrite(data, 1, frame_size, file);
    if(ret != frame_size) {
        printf("Write fail: wanted %d, but only write %ld\n", frame_size, ret);
        return;
    }

}

int main(int argc, const char *argv[])
{
    struct renderer_tst render;
    int i = 0;

    render.m_width = 640;
    render.m_height = 480;

    InitOpenGL(&render);

    uint32_t frame_size = render.m_width * render.m_height * 4;
    GLbyte *data = (GLbyte*)malloc(frame_size);

    for (i = 0; i < 10; i++) {
        printf("render i=%d\n", i);
        RenderOpenGL(&render);
        dump_texture(render.texture_write, render.m_width, render.m_height, data);

        save_data(data, i, render.m_width, render.m_height);
        sleep(1);
    }

    free(data);
    ReleaseOpenGL(&render);

    return 0;
}
