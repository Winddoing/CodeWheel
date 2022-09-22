/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : render_enc_gbm.c
 *  Author       : wqshao
 *  Created Time : 2022-09-13 20:04:38
 *  Description  :
 *		编译：gcc render_enc_gbm.c `pkg-config --libs gbm gl egl`
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h> //sleep
#include <string.h>
#include <sys/mman.h> //mmap

#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#include <assert.h>
#include <GL/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <libdrm/drm.h>
#include <libdrm/drm_fourcc.h>
#include <fcntl.h>
#include <gbm.h>


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
	render->image = eglCreateImage(render->egl_display, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, NULL, attribute);

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
	eglDestroyImage(render->egl_display, render->image);
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
	struct render_tst render;
	int i = 0;

	render.m_width = 640;
	render.m_height = 480;

	//render.m_fd_handle = fd;

	strncpy(render.m_device_path, "/dev/dri/renderD128", sizeof(render.m_device_path) - 1);

	printf("w:%d, h:%d, device path: %s\n", render.m_width, render.m_height, render.m_device_path);

	uint32_t frame_size = render.m_width * render.m_height * 4;
	GLbyte *data = (GLbyte*)malloc(frame_size);

	InitOpenGL(&render);

	for (i = 0; i < 10; i++) {
		printf("render i=%d\n", i);
		RenderOpenGL(&render);

		//dump_texture(render.texture_load, render.m_width, render.m_height, data);
		dump_texture(render.texture_render, render.m_width, render.m_height, data);
		save_data(data, i, render.m_width, render.m_height);

		//int fb_size = render.m_width * render.m_height * 4;            
		//char *data = mmap(NULL, fb_size, PROT_READ, MAP_SHARED, render.m_fd_handle, 0);  
		//FILE *rawfp = fopen("data.yuv", "wb+");                                          
		//fwrite(data, fb_size, 1, rawfp);                                      
		//fclose(rawfp);                                                        
		//munmap(data, fb_size);                                                

		sleep(1);
	}

	free(data);
	ReleaseOpenGL(&render);

	return 0;
}
