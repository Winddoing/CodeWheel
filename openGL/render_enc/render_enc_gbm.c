/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : render_enc_gbm.c
 *  Author       : wqshao
 *  Created Time : 2022-09-13 20:04:38
 *  Description  :
 *				sudo apt install libepoxy-dev
 *		编译： gcc render_enc_gbm.c `pkg-config --libs gbm epoxy`
 *
 *	render context      |      convert context           |
 *                      |                                |
 *                      |                                |
 *                      |                                |
 *                      |                                |
 *   +-------+          |  +--------+                    |
 *   | tex ID+-----copy-+--> in tex +----+               |
 *   +-------+          |  +--------+    |               |
 *                      |                |               |
 *                      |             convert            |
 *                      |                |               |
 *                      |                |               |
 *                      |                |     +-------+ |
 *                      |                +----->out tex| |
 *                      |                      +-------+ |
 *                      |                                |
 *
 * 渲染和转换（使用计算着色器）必须在不同的上下文中
 *
 * 原因：在同一个上下文中，使用片段着色器和计算着色器的时候，片段着色器无法渲染正常的颜色
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h> //sleep
#include <string.h>
#include <sys/mman.h> //mmap

#if 1
#include <epoxy/egl.h>
#include <epoxy/gl.h>
#else
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <GL/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif
#include <gbm.h>

/*#include <libdrm/drm.h> */
#include <libdrm/drm_fourcc.h> //GBM_FORMAT_ABGR8888
#include <fcntl.h>

#include <assert.h>
#include <errno.h>
#include <sched.h>


#define GL_CHECK(x) \
	x; \
	{ \
		GLenum glError = glGetError(); \
		if(glError != GL_NO_ERROR) { \
			fprintf(stderr, "glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__);exit(1); \
		} \
	}

#define GO_CHECK_GL_ERROR(...)   printf("CHECK_GL_ERROR [%s:%d], glGetError = %d\n",  __FUNCTION__, __LINE__, glGetError())

#define EGL_CHECK(x) \
    x; \
    { \
        EGLint eglError = eglGetError(); \
        if(eglError != EGL_SUCCESS) { \
            fprintf(stderr, "eglGetError() = %i (0x%.8x) at line %i\n", eglError, eglError, __LINE__); \
            ; \
        } \
    }

struct render_tst {
	EGLDisplay egl_display;
	EGLContext egl_context;
	EGLSurface egl_surface;
	GLuint framebuffer;
	GLuint renderbuffer;
	GLuint texture_load;
	GLuint texture_render;
	EGLImage image;
	uint16_t m_width;
	uint16_t m_height;
	uint16_t m_fd_handle;
	char m_device_path[128];
	int m_fd;
	struct gbm_device* m_gbm_device;
	struct gbm_surface* m_gbm_surface;
	int m_data[4];

	//rgba convert nv12
	EGLDisplay convert_dpy;
	EGLContext convert_cxt;
	GLuint cs_prog_handle;
	GLuint in_rgba_texture;
	GLuint out_nv12_texture;
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

void Initialize_EGL_gbm(struct render_tst* render)
{
	// set OpenGL rendering API
	eglBindAPI(EGL_OPENGL_API);

#if 1
	// get device fd
	if (strlen(render->m_device_path))
		render->m_fd = open(render->m_device_path, O_RDWR);
	else
		render->m_fd = open("/dev/dri/renderD128", O_RDWR);

	// create gbm device
	render->m_gbm_device = gbm_create_device(render->m_fd);

	// create gbm surface
	render->m_gbm_surface = gbm_surface_create(render->m_gbm_device, render->m_width, render->m_height,
			GBM_FORMAT_ARGB8888, GBM_BO_USE_RENDERING);

	// get an EGL display with gbm
	PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
		(PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
	render->egl_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA, render->m_gbm_device, NULL);

	// initialize the EGL display connection
	eglInitialize(render->egl_display, NULL, NULL);
#else
	egl_display = eglGetCurrentDisplay();
#endif

	// get the number of all EGL frame buffer configurations for a display
	EGLint num_configs;
	eglGetConfigs(render->egl_display, NULL, 0, &num_configs);

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
	eglChooseConfig(render->egl_display, egl_config_attribs, configs, num_configs, &num_configs);

	printf("===> func: %s, line: %d, num_configs=%d\n", __func__, __LINE__, num_configs);
	// search all configurations
	EGLConfig config_use = NULL;
	for (int i = 0; i < num_configs; ++i)
	{
		EGLint gbm_format;
		eglGetConfigAttrib(render->egl_display, configs[i], EGL_NATIVE_VISUAL_ID, &gbm_format);
		if (gbm_format == GBM_FORMAT_ARGB8888) // match found
		{
			config_use = configs[i];
			free(configs);
			break;
		}
	}

	// create an EGL rendering context
	render->egl_context = eglCreateContext(render->egl_display, config_use, EGL_NO_CONTEXT, NULL);
	//创建共享上下文
	//render->egl_context = eglCreateContext(render->egl_display, config_use, eglGetCurrentContext(), NULL);

#if 1 //与渲染测试相关
	// create an EGL window surface with gbm
	PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC eglCreatePlatformWindowSurfaceEXT =
		(PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");

	render->egl_surface = eglCreatePlatformWindowSurfaceEXT(render->egl_display, config_use, render->m_gbm_surface, NULL);

	// connect the context to the surface
	eglMakeCurrent(render->egl_display, render->egl_surface, render->egl_surface, render->egl_context);
#else
	eglMakeCurrent(render->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, render->egl_context);
#endif
}

void InitOpenGL(struct render_tst* render)
{
	// Initialize EGL
	Initialize_EGL_gbm(render);

#if 1
	// Setup GL scene
	gl_setup_scene();

	// 2X2 texture
	render->m_data[0] = 0x000000FF; // Red
	render->m_data[1] = 0x0000FF00; // Green
	render->m_data[2] = 0X00FF0000; // Blue
	render->m_data[3] = 0x00FFFFFF; // White

	// Create texture_load
	glGenTextures(1, &render->texture_load);
	glBindTexture(GL_TEXTURE_2D, render->texture_load);
	//指定纹理数据，m_data,大小是2x2
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, render->m_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif

	// Set the attribute for EGLImage
	EGLAttrib const attribute[] = {
		EGL_WIDTH, render->m_width,
		EGL_HEIGHT, render->m_height,
		EGL_LINUX_DRM_FOURCC_EXT, DRM_FORMAT_ABGR8888,
		EGL_DMA_BUF_PLANE0_FD_EXT, render->m_fd_handle,
		EGL_DMA_BUF_PLANE0_OFFSET_EXT, 0,
		EGL_DMA_BUF_PLANE0_PITCH_EXT, render->m_width * 4,
		EGL_NONE};

	// Create EGLImage from VA handle
	render->image = EGL_CHECK(eglCreateImage(render->egl_display, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, NULL, attribute));
	printf("===> func: %s, line: %d image=%p\n", __func__, __LINE__, render->image);

	// Create texture_render and bind it to the framebuffer
	glGenTextures(1, &render->texture_render);
	glBindTexture(GL_TEXTURE_2D, render->texture_render);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, render->m_width, render->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//将texture_render纹理与egl image进行关联，这样拷贝到texture_render中的纹理就直接到与egl image关联的dmabuf中
	PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES =
		(PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)eglGetProcAddress("glEGLImageTargetTexture2DOES");
	glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, render->image);


#if 1
	// Create renderbuffer
	glGenRenderbuffers(1, &render->renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, render->renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, render->m_width, render->m_height);

	// Create framebuffer
	glGenFramebuffers(1, &render->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, render->framebuffer);

	// 绑定渲染buffer后，后面测试中渲染的图像就直接到texture_render纹理中了
	// Attach texture and renderbuffer
	//将纹理图像附加到帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render->texture_render, 0);
	//将渲染缓冲区挂载到当前帧缓冲区上
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render->renderbuffer);

	// Check framebuffer
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	// Swap happens as soon as the render is complete
	eglSwapInterval(render->egl_display, 0);

	// Set viewport
	glViewport(0, 0, render->m_width, render->m_height);
#endif
}

void RenderOpenGL(struct render_tst* render)
{
#if 1
	// load the texture
	glBindTexture(GL_TEXTURE_2D, render->texture_load);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, render->m_data);

	// Render
	gl_draw_scene(render->texture_load);
	eglSwapBuffers(render->egl_display, render->egl_surface);

	// Rotate the texture
	rotate_data(render->m_data);
#endif
}

void ReleaseOpenGL(struct render_tst* render)
{
	glDeleteTextures(1, &render->texture_load);
	glDeleteTextures(1, &render->texture_render);
	glDeleteRenderbuffers(1, &render->renderbuffer);
	glDeleteFramebuffers(1, &render->framebuffer);
	eglMakeCurrent(render->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	if (render->image)
		EGL_CHECK(eglDestroyImage(render->egl_display, render->image));
	eglDestroyContext(render->egl_display, render->egl_context);
	eglDestroySurface(render->egl_display, render->egl_surface);
	eglTerminate(render->egl_display);
	gbm_device_destroy(render->m_gbm_device);
	gbm_surface_destroy(render->m_gbm_surface);
	close(render->m_fd);
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

	sprintf(file_name, "IMG_RGBA_%dx%d_%d_frames.dat", w, h, i);
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

//获取转换时间
uint32_t timerQueriesSupported = 0;

uint32_t IsExtensionSupported(const char *Extension)
{
	if (!Extension)
		return 0;

	int NumberOfExtensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
	for (int i = 0; i < NumberOfExtensions; i++) {
		const char*extensionString = (const char*)glGetStringi(GL_EXTENSIONS, i);
		if (strcmp(extensionString, Extension) == 0) {
			return 1;
		}
	}
	return 0;
}

static void convert_texture_prepare(struct render_tst* render)
{
	// Input texture (RGBA)
	glGenTextures(1, &render->in_rgba_texture);
	GL_CHECK(glActiveTexture(GL_TEXTURE0));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, render->in_rgba_texture));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, render->m_width, render->m_height));
	GL_CHECK(glBindImageTexture(0, render->in_rgba_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8));

	// Output texture (NV12)
	GL_CHECK(glGenTextures(1, &render->out_nv12_texture));
	GL_CHECK(glActiveTexture(GL_TEXTURE2));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, render->out_nv12_texture));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, render->m_width, render->m_height + (render->m_height / 2)));
	GL_CHECK(glBindImageTexture(1, render->out_nv12_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R8));
}

static int convert_pre_compute_program()
{
	// Create shader, load shader code, compile and attach.
	GLuint progHandle = glCreateProgram();
	GLuint cs = glCreateShader(GL_COMPUTE_SHADER);
	char *src = NULL;

	const GLchar* fullShaderSource = " \
		#version 310 es                                                      \n\
		#extension GL_NV_image_formats : enable                              \n\
		layout (rgba8, binding = 0) readonly uniform lowp image2D rgbaImage; \n\
		layout (r8, binding = 1) writeonly uniform lowp image2D yImage;      \n\
		layout (local_size_x = 16, local_size_y = 16) in;                    \n\
		layout(location = 2) uniform int height;                             \n\
		void main()                                                          \n\
		{                                                                    \n\
		    ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);                \n\
		    ivec2 nvPos;                                                     \n\
		    ivec2 ypos;                                                      \n\
		    vec4 vr;                                                         \n\
		    vec4 vl;                                                         \n\
		    vec4 vld;                                                        \n\
		    vec4 vrd;                                                        \n\
		    vec4 sumUV;                                                      \n\
		    vec4 yvec;                                                       \n\
		    float y0,u0,v0,y1,u1,v1,y2,u2,v2,y3,u3,v3;                       \n\
		    if(storePos.y >= height)                                         \n\
		    {                                                                \n\
		        return;                                                      \n\
		    }                                                                \n\
		    if(storePos.x % 2 == 0 && storePos.y % 2 == 0)                   \n\
		    {                                                                \n\
		        nvPos = storePos;                                            \n\
		        ypos = storePos;                                             \n\
		        vl = imageLoad(rgbaImage, storePos);                         \n\
		        storePos.x+=1;                                               \n\
		        vr = imageLoad(rgbaImage, storePos);                         \n\
		        storePos.y+=1;                                               \n\
		        vrd = imageLoad(rgbaImage, storePos);                        \n\
		        storePos.x-=1;                                               \n\
		        vld = imageLoad(rgbaImage, storePos);                        \n\
		                                                                     \n\
		        y0 = 0.299*vl.r + 0.587 *vl.g + 0.114*vl.b;                  \n\
		        u0 = (-0.169*vl.r - 0.331*vl.g + 0.500*vl.b) + 0.5;          \n\
		        v0 = (0.500*vl.r - 0.419*vl.g - 0.081*vl.b) + 0.5;           \n\
		                                                                     \n\
		        y1 = 0.299*vr.r + 0.587 *vr.g + 0.114*vr.b;                  \n\
		        u1 = (-0.169*vr.r - 0.331*vr.g + 0.500*vr.b) + 0.5;          \n\
		        v1 = (0.500*vr.r - 0.419*vr.g - 0.081*vr.b) + 0.5;           \n\
		                                                                     \n\
		        y2 = 0.299*vrd.r + 0.587 *vrd.g + 0.114*vrd.b;               \n\
		        u2 = (-0.169*vrd.r - 0.331*vrd.g + 0.500*vrd.b) + 0.5;       \n\
		        v2 = (0.500*vrd.r - 0.419*vrd.g - 0.081*vrd.b) + 0.5;        \n\
		                                                                     \n\
		        y3 = 0.299*vld.r + 0.587 *vld.g + 0.114*vld.b;               \n\
		        u3 = (-0.169*vld.r - 0.331*vld.g + 0.500*vld.b) + 0.5;       \n\
		        v3 = (0.500*vld.r - 0.419*vld.g - 0.081*vld.b) + 0.5;        \n\
		                                                                     \n\
		        sumUV.x = (u0+u1+u2+u3)/4.0;                                 \n\
		        sumUV.y = (v0+v1+v2+v3)/4.0;                                 \n\
		                                                                     \n\
		                                                                     \n\
		        yvec.x = y0;                                                 \n\
		        imageStore(yImage, ypos, yvec);                              \n\
		        ypos.x = ypos.x+1;                                           \n\
		        yvec.x = y1;                                                 \n\
		        imageStore(yImage, ypos, yvec);                              \n\
		        ypos.y = ypos.y+1;                                           \n\
		        yvec.x = y2;                                                 \n\
		        imageStore(yImage, ypos, yvec);                              \n\
		        ypos.x = ypos.x-1;                                           \n\
		        yvec.x = y3;                                                 \n\
		        imageStore(yImage, ypos, yvec);                              \n\
		                                                                     \n\
		        // calculate position of NV components                       \n\
		        nvPos.x = nvPos.x;                                           \n\
		        nvPos.y = nvPos.y/2;                                         \n\
		                                                                     \n\
		        // update start position of NV buffer                        \n\
		        nvPos.y += height;                                           \n\
		                                                                     \n\
		        imageStore(yImage, nvPos, sumUV);                            \n\
		        sumUV.x = sumUV.y;                                           \n\
		        nvPos.x += 1;                                                \n\
		        imageStore(yImage, nvPos, sumUV);                            \n\
		    }                                                                \n\
		}";

	glShaderSource(cs, 1, &fullShaderSource, NULL);
	glCompileShader(cs);

	int rvalue;
	glGetShaderiv(cs, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue) {
		printf("Error in compiling the compute shader\n");
		GLchar log[10240];
		GLsizei length;
		glGetShaderInfoLog(cs, 10239, &length, log);
		printf("Compiler log:\n%s\n", log);
		exit(1);
	}
	glAttachShader(progHandle, cs);

	glLinkProgram(progHandle);
	glGetProgramiv(progHandle, GL_LINK_STATUS, &rvalue);
	if (!rvalue) {
		printf("Error in linking compute shader program\n");
		GLchar log[10240];
		GLsizei length;
		glGetProgramInfoLog(progHandle, 10239, &length, log);
		printf("Linker log:\n%s\n", log);
		exit(1);
	}

	glUseProgram(progHandle);
	if (src)
		free(src);

	printf("===> func: %s, line: %d, progHandle=%d\n", __func__, __LINE__, progHandle);
	return progHandle;
}

static int convert_init(struct render_tst* render)
{
	uint32_t res;

	render->convert_dpy = eglGetCurrentDisplay();

	const char *eglExtensionString = NULL;

	eglExtensionString = eglQueryString(render->convert_dpy, EGL_EXTENSIONS);
	assert(strstr(eglExtensionString, "EGL_KHR_create_context") != NULL);
	assert(strstr(eglExtensionString, "EGL_KHR_surfaceless_context") != NULL);

	assert(strstr(eglExtensionString, "EGL_KHR_image") != NULL);
	// dma buff export extension check - not used in this version
	assert(strstr(eglExtensionString, "EGL_MESA_image_dma_buf_export") != NULL);

	static const EGLint config_attribs[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
		EGL_NONE
	};

	EGLConfig cfg;
	EGLint count;

	res = eglChooseConfig(render->convert_dpy, config_attribs, &cfg, 1, &count);
	assert(res);

	res = eglBindAPI(EGL_OPENGL_ES_API);
	assert(res);

	static const EGLint attribs[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 3,
		EGL_NONE
	};

	render->convert_cxt = eglCreateContext(render->convert_dpy,
			cfg,
			render->egl_context,
			attribs);

	assert(render->convert_cxt != EGL_NO_CONTEXT);

	res = eglMakeCurrent(render->convert_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, render->convert_cxt);

	// Support for GPU performance measurements
	if (IsExtensionSupported("GL_EXT_disjoint_timer_query")) {
		timerQueriesSupported = 1;
	}


	convert_texture_prepare(render);

	render->cs_prog_handle = convert_pre_compute_program();

	EGL_CHECK(eglMakeCurrent(render->egl_display, render->egl_surface, render->egl_surface, render->egl_context));
}

static void convert_copy_rgba(struct render_tst* render )
{
	EGL_CHECK(eglMakeCurrent(render->convert_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, render->convert_cxt));

	GLuint fbo = 0;
	GL_CHECK(glGenFramebuffers(1, &fbo));
	GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo));
	GL_CHECK(glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render->texture_render, 0));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, render->in_rgba_texture));

	GL_CHECK(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, render->m_width, render->m_height));

	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

	EGL_CHECK(eglMakeCurrent(render->egl_display, render->egl_surface, render->egl_surface, render->egl_context));

}

#define ALIGN(_v, _d) (((_v) + ((_d) - 1)) & ~((_d) - 1))

static void convert_rgba2nv12(struct render_tst* render)
{
	EGL_CHECK(eglMakeCurrent(render->convert_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, render->convert_cxt));

	GL_CHECK(glUseProgram(render->cs_prog_handle));

	// Pass height to shader
	GL_CHECK(glUniform1i(2, render->m_height));

	if (timerQueriesSupported) {
		GLuint timeElapsedQuery;
		GLuint64 elapsedTime;
		GLuint64 totalElapsedTime;
		GLuint64  maxElapsedTime, minElapsedTime;
		maxElapsedTime = 0;
		minElapsedTime = 0;
		minElapsedTime = ~minElapsedTime;
		elapsedTime = 0;
		totalElapsedTime = 0;
		int Iterations = 1;

		GL_CHECK(glGenQueriesEXT(1, &timeElapsedQuery));

		// Execute shader in a lopp and gather metrics
		for (int i = 0; i < Iterations; i++) {

			GL_CHECK(glBeginQueryEXT(GL_TIME_ELAPSED_EXT, timeElapsedQuery));
			// Calculating of num groups etc. should be done dynamically
			GL_CHECK(glDispatchCompute(render->m_width / 16, ALIGN(render->m_height, 16) / 16, 1));
			GL_CHECK(glEndQueryEXT(GL_TIME_ELAPSED_EXT));

			int timeout = 200000;
			GLuint ready = GL_FALSE;
			while (ready == GL_FALSE && timeout > 0) {
				sched_yield();
				GL_CHECK(glGetQueryObjectuivEXT(timeElapsedQuery, GL_QUERY_RESULT_AVAILABLE_EXT, &ready));
				timeout--;
			}
			if (timeout == 0) {
				printf("Timeout in glGetQueryObjectuivEXT! Error, process exiting!\n");
				exit(1);
			} else {
				// Get elapsed time
				GL_CHECK(glGetQueryObjectui64vEXT(timeElapsedQuery, GL_QUERY_RESULT_EXT, &elapsedTime));

				totalElapsedTime += elapsedTime;

				if (elapsedTime > maxElapsedTime) {
					maxElapsedTime = elapsedTime;
				}

				if (elapsedTime < minElapsedTime) {
					minElapsedTime = elapsedTime;
				}

			}
		}
		printf("Time elapsed avg: %f ms min %f max %f \n", (totalElapsedTime / 1000000.0) / Iterations,
				(minElapsedTime / 1000000.0),
				(maxElapsedTime / 1000000.0));

		GL_CHECK(glDeleteQueriesEXT(1, &timeElapsedQuery));
	} else {
		// No support for measurements - just only call shader once
		GL_CHECK(glDispatchCompute(render->m_width / 16, render->m_height / 16, 1));
	}

	EGL_CHECK(eglMakeCurrent(render->egl_display, render->egl_surface, render->egl_surface, render->egl_context));
}

void convert_save_nv12(struct render_tst* render, uint32_t index)
{
	EGL_CHECK(eglMakeCurrent(render->convert_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, render->convert_cxt));

	FILE *f;
	char img_file[64] = {0};

	sprintf(img_file, "IMG%d_%dx%d_raw.nv12", index, render->m_width, render->m_height);

	f = fopen(img_file, "wb");
	if (f) {
		GLuint fbo;
		GL_CHECK(glGenFramebuffers(1, &fbo));
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render->out_nv12_texture, 0));

#if 0
		unsigned char *rgbaPixels = (unsigned char *)malloc(render->m_width * (render->m_height + (render->m_height/2)) * 4);
		if (!rgbaPixels) {
			printf("Can't not allocate memory for output!\n");
			exit(1);
		}

		GL_CHECK(glReadPixels(0, 0, render->m_width, render->m_height + (render->m_height / 2), GL_RGBA, GL_UNSIGNED_BYTE, rgbaPixels));

		// Now convert create temporary buffer and convert (RGBA) to output (NV12)
		unsigned char *nv12ImageBuffer = (unsigned char*)malloc(render->m_width * (render->m_height + (render->m_height / 2)));
		if (!nv12ImageBuffer) {
			printf("Can't allocate memory for sharing converted buffer \n");
		}

		unsigned int k = 0;
		for (unsigned int i = 0; i < render->m_width * (render->m_height + (render->m_height / 2)) * 4; i += 4) {
			nv12ImageBuffer[k] = rgbaPixels[i];
			k++;
		}
#else
		unsigned char *nv12ImageBuffer = (unsigned char*)malloc(render->m_width * (render->m_height + (render->m_height / 2)));
		if (!nv12ImageBuffer) {
			printf("Can't allocate memory for sharing converted buffer \n");
		}

		//直接以8bit为一个单元读取该纹理中的数据
		GL_CHECK(glReadPixels(0, 0, render->m_width, render->m_height + (render->m_height / 2), GL_RED, GL_UNSIGNED_BYTE, nv12ImageBuffer));
#endif

		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GL_CHECK(glDeleteFramebuffers(1, &fbo));

		fwrite(nv12ImageBuffer, 1, render->m_width * (render->m_height + (render->m_height / 2)), f);

		fclose(f);

/*		free(rgbaPixels); */
		free(nv12ImageBuffer);
	} else {
		printf("Problem with accessing output file!\n");
	}

	EGL_CHECK(eglMakeCurrent(render->egl_display, render->egl_surface, render->egl_surface, render->egl_context));
}

void convert_deinit(struct render_tst* render)
{
	EGL_CHECK(eglMakeCurrent(render->convert_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, render->convert_cxt));

	GL_CHECK(glDeleteTextures(1, &render->in_rgba_texture));
	GL_CHECK(glDeleteTextures(1, &render->out_nv12_texture));
	EGL_CHECK(eglDestroyContext(render->convert_dpy, render->convert_cxt));

	EGL_CHECK(eglMakeCurrent(render->egl_display, render->egl_surface, render->egl_surface, render->egl_context));
}

int main(int argc, const char *argv[])
{
	struct render_tst render;
	int i = 0;

	render.m_width  = 1920;
	render.m_height = 1080;

	//render.m_fd_handle = fd;

	strncpy(render.m_device_path, "/dev/dri/renderD128", sizeof(render.m_device_path) - 1);

	printf("w:%d, h:%d, device path: %s\n", render.m_width, render.m_height, render.m_device_path);

	uint32_t frame_size = render.m_width * render.m_height * 4;
	GLbyte *data = (GLbyte*)malloc(frame_size);

	InitOpenGL(&render);
	convert_init(&render);

	for (i = 0; i < 4; i++) {
		printf("render i=%d\n", i);
		RenderOpenGL(&render);

		//dump_texture(render.texture_load, render.m_width, render.m_height, data);
		dump_texture(render.texture_render, render.m_width, render.m_height, data);
		save_data(data, i, render.m_width, render.m_height);

#if 1
		convert_copy_rgba(&render);
		convert_rgba2nv12(&render);
		convert_save_nv12(&render, i);
#endif

		sleep(1);
	}

	free(data);
	convert_deinit(&render);
	ReleaseOpenGL(&render);

	return 0;
}
