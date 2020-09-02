// Sample for rgb-nv12
#include <epoxy/egl.h>
#include <epoxy/gl.h>

#include <assert.h>
#include <fcntl.h>
#include <gbm.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>
#include <xf86drm.h>
#include <sched.h>

#include <xf86drmMode.h>
#include <drm/drm_fourcc.h>

#define ALIGN(_v, _d) (((_v) + ((_d) - 1)) & ~((_d) - 1))

#define GL_CHECK(x) \
        x; \
        { \
          GLenum glError = glGetError(); \
          if(glError != GL_NO_ERROR) { \
            fprintf(stderr, "glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__);exit(1); \
          } \
        }

#define EGL_CHECK(x) \
    x; \
    { \
        EGLint eglError = eglGetError(); \
        if(eglError != EGL_SUCCESS) { \
            fprintf(stderr, "eglGetError() = %i (0x%.8x) at line %i\n", eglError, eglError, __LINE__); \
            ; \
        } \
    }

#ifdef DMA_BUFF_SUPPORT
// Function creates DMA buffer from given texture id
// Such function should be provided by server process.
int CreateDmaBufferFromTexture(int DrmFD,
	EGLDisplay Display, 
	EGLContext Context, 
	GLuint Texture, 
	EGLImageKHR &Image)
{
	EGLint name, handle, stride;
	int fd;

	Image = EGL_CHECK(eglCreateImageKHR(Display,
		Context,
		EGL_GL_TEXTURE_2D_KHR,
		(EGLClientBuffer)(unsigned long)Texture,
		NULL));

	EGL_CHECK(eglExportDRMImageMESA(Display,
		Image,
		&name,
		&handle,
		&stride));

	printf("Exported: name [0x%x] handle[0x%x] stride[%d]\n", name, handle, stride);

	int result = drmPrimeHandleToFD(DrmFD, handle, DRM_CLOEXEC, &fd);
	if (result < 0)
	{
		printf("drmPrimeHandleToFD failed with %s \n", strerror(errno));
		exit(1);
	}

	return fd;
}

// Function creates texture from fd of passed dma-buf
// and returns KHR image
// Not used
EGLImageKHR CreateTextureFromFd(EGLDisplay Display,
	int fd, 
	int Width, 
	int Height, 
	int Stride,
	int Format, 
	GLuint LocalTextureId)
{	
	EGLint attrs[13];

	attrs[0] = EGL_DMA_BUF_PLANE0_FD_EXT;
	attrs[1] = fd;
	attrs[2] = EGL_DMA_BUF_PLANE0_PITCH_EXT;
	attrs[3] = Stride;
	attrs[4] = EGL_DMA_BUF_PLANE0_OFFSET_EXT;
	attrs[5] = 0;
	attrs[6] = EGL_WIDTH;
	attrs[7] = Width;
	attrs[8] = EGL_HEIGHT;
	attrs[9] = Height;
	attrs[10] = EGL_LINUX_DRM_FOURCC_EXT;
	attrs[11] = Format;
	attrs[12] = EGL_NONE;

	EGLImageKHR image = EGL_CHECK(eglCreateImageKHR(Display,
		EGL_NO_CONTEXT,
		EGL_LINUX_DMA_BUF_EXT,
		(EGLClientBuffer)NULL,
		attrs))

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, LocalTextureId));
	EGL_CHECK(glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES)image));
	return image;
}
#endif

GLuint PrepareComputeProgram()
{
	// Create shader, load shader code, compile and attach.
	GLuint progHandle = glCreateProgram();
	GLuint cs = glCreateShader(GL_COMPUTE_SHADER);
	char *src = NULL;
	
	char *fullShaderSource[2];
	char *version = "#version 310 es \n";
	fullShaderSource[0] = version;

	FILE *f = fopen("shader.cs", "r");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		size_t sz = ftell(f);
		rewind(f);
		src = (char*)malloc(sz + 1);
		size_t read = fread(src, 1, sz, f);
		if (read != sz)
		{
			printf("error reading a file!\n");
			exit(1);
		}
		src[read] = 0;
		fullShaderSource[1] = src;
		printf("loaded shader: \n %s \n", src);
		fclose(f);
	}
	else
	{
		printf("error opening file [%s] !\n", strerror(errno));
		exit(1);
	}

	glShaderSource(cs, 2, fullShaderSource, NULL);
	glCompileShader(cs);
	
	int rvalue;
	glGetShaderiv(cs, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue) 
	{
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
	if (!rvalue) 
	{
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

	return progHandle;
}

unsigned char *LoadBMP(const char *FileName)
{
	FILE * file;
	file = fopen(FileName, "rb");
	if (file)
	{
		fseek(file, 0L, SEEK_END);
		size_t sz = ftell(file);
		fseek(file, 0L, SEEK_SET);
		unsigned char *b = (unsigned char*)malloc(sz);
		size_t read = fread(b, 1, sz, file);
		if (read != sz)
		{
			free(b);
			return NULL;
		}
		return b;
	}
	else
	{ 
		printf("Error loading bmp!\n");
		exit(1);
	}

	return NULL;
}

unsigned int TextureWidth = 0;
unsigned int TextureHeight = 0;

GLuint RGBAInputTextureHandle = 0;
GLuint NV12TextureOutputHandle = 0;

bool timerQueriesSupported = false;

EGLImageKHR ImageRGBA = 0;
EGLImageKHR ImageNV12 = 0;

int fdRGBA = 0;
int fdNV12 = 0;

int Iterations = 1;

// Load bmp file (RGB) and convert it to RGBA	
unsigned char *PrepareBMPFile(char *FileName)
{
	// Allocate memory for RGBA output image.
	// Note that height of the texture on Intel cards must be 
	// divisible (without remainder) by 32. 
	unsigned char *rgbaImage = (unsigned char*)malloc(TextureWidth * (ALIGN(TextureHeight,32) * 4));
	if (!rgbaImage)
		return NULL;

	unsigned char *rgbImage = LoadBMP(FileName);
	if (!rgbImage)
	{
		free(rgbaImage);
		return NULL;
	}

	// convert RGB to RGBx 
	if (rgbImage)
	{
		// Skip BMP header
		unsigned char *src = &rgbImage[66];
		unsigned char *dst = rgbaImage;
		for (unsigned int i = 0; i < TextureWidth * TextureHeight * 3; i += 3)
		{
			dst[2] = src[i];
			dst[1] = src[i + 1];
			dst[0] = src[i + 2];
			// keep alpha as 0 
			dst[3] = 0;
			dst += 4;
		}
	}

	free(rgbImage);
	return rgbaImage;
}

void PrepareTextures(int DrmFd, EGLDisplay Display, EGLContext Context, unsigned char *PixelData)
{
	// Input texture (RGBA)
	glGenTextures(1, &RGBAInputTextureHandle);
	GL_CHECK(glActiveTexture(GL_TEXTURE0));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, RGBAInputTextureHandle));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CHECK(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, TextureWidth, TextureHeight));

	// Fill texture with pixel data
	GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TextureWidth, TextureHeight, GL_RGBA, GL_UNSIGNED_BYTE, PixelData));
	GL_CHECK(glBindImageTexture(0, RGBAInputTextureHandle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8));
	
#ifdef DMA_BUFF_SUPPORT
	fdRGBA = CreateDmaBufferFromTexture(DrmFd, Display, Context, RGBAInputTextureHandle, ImageRGBA);
#endif

	// Output texture (NV12)
	GL_CHECK(glGenTextures(1, &NV12TextureOutputHandle));
	GL_CHECK(glActiveTexture(GL_TEXTURE2));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, NV12TextureOutputHandle));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		
	GL_CHECK(glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, TextureWidth, TextureHeight + (TextureHeight/2)));
	GL_CHECK(glBindImageTexture(1, NV12TextureOutputHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R8));

#ifdef DMA_BUFF_SUPPORT
	fdNV12 = CreateDmaBufferFromTexture(DrmFd, Display, Context, NV12TextureOutputHandle, ImageNV12);
#endif

}

void ExecuteTest(GLuint ComputeHandle)
{
	glUseProgram(ComputeHandle);
	GLuint timeElapsedQuery;
	GLuint64 elapsedTime;
	GLuint64 totalElapsedTime;
	GLuint64  maxElapsedTime, minElapsedTime;
	maxElapsedTime = 0;
	minElapsedTime = 0;
	minElapsedTime = ~minElapsedTime;
	elapsedTime = 0;
	totalElapsedTime = 0;

	// Pass height to shader
	GL_CHECK(glUniform1i(2, TextureHeight));
	
	if (timerQueriesSupported)
	{
		GL_CHECK(glGenQueriesEXT(1, &timeElapsedQuery));

		// Execute shader in a lopp and gather metrics
		for (int i = 0; i < Iterations; i++)
		{

			GL_CHECK(glBeginQueryEXT(GL_TIME_ELAPSED_EXT, timeElapsedQuery));
			// Calculating of num groups etc. should be done dynamically
			GL_CHECK(glDispatchCompute(TextureWidth / 16, ALIGN(TextureHeight, 16) / 16, 1));
			GL_CHECK(glEndQueryEXT(GL_TIME_ELAPSED_EXT));

			int timeout = 200000;
			GLuint ready = GL_FALSE;
			while (ready == GL_FALSE && timeout > 0)
			{
				sched_yield();
				GL_CHECK(glGetQueryObjectuivEXT(timeElapsedQuery, GL_QUERY_RESULT_AVAILABLE_EXT, &ready));
				timeout--;
			}
			if (timeout == 0)
			{
				printf("Timeout in glGetQueryObjectuivEXT! Error, process exiting!\n");
				exit(1);
			}
			else
			{
				// Get elapsed time
				GL_CHECK(glGetQueryObjectui64vEXT(timeElapsedQuery, GL_QUERY_RESULT_EXT, &elapsedTime));

				totalElapsedTime += elapsedTime;

				if (elapsedTime > maxElapsedTime)
				{
					maxElapsedTime = elapsedTime;
				}

				if (elapsedTime < minElapsedTime)
				{
					minElapsedTime = elapsedTime;
				}

			}
		}
		printf("Time elapsed avg: %f ms min %f max %f \n", (totalElapsedTime / 1000000.0) / Iterations,
			(minElapsedTime / 1000000.0), 
			(maxElapsedTime / 1000000.0));
		
		GL_CHECK(glDeleteQueriesEXT(1, &timeElapsedQuery));
	}
	else
	{
		// No support for measurements - just only call shader once
		GL_CHECK(glDispatchCompute(TextureWidth / 16, TextureHeight / 16, 1));
	}
}

bool IsExtensionSupported(const char *Extension)
{
	if (!Extension)
		return false;

	int NumberOfExtensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
	for (int i = 0; i < NumberOfExtensions; i++)
	{
		const char*extensionString = (const char*)glGetStringi(GL_EXTENSIONS, i);
		if (strcmp(extensionString, Extension) == 0)
		{
			return true;
		}
	}
	return false;
}

// todo: fix all memory/handles leaks!
int main(int argc, char *argv[])
{
	bool res;

	if (argc < 5)
	{
		printf("syntax: %s width height input_file number_of_iterations\n", argv[0]);
		return 1;
	}

	TextureWidth = atoi(argv[1]);
	TextureHeight = atoi(argv[2]);
	Iterations = atoi(argv[4]);

	if (TextureWidth > 3840 || TextureHeight > 2160)
	{
		printf("Maximum input image dimension supported is <3840,2160> \n");
		return 1;
	}

	if (TextureWidth == 0 || TextureHeight == 0)
	{
		printf("Texture width or height can't be set 0\n");
		return 1;
	}

	printf("input texture [%d]x[%d] \n", TextureWidth, TextureHeight);

	unsigned char *pixelData = PrepareBMPFile(argv[3]);
	if (!pixelData)
	{
		return 1;
	}

	const char *eglExtensionString = NULL;

	// Open device; card0, card1 or whatever
	int32_t fd = open("/dev/dri/card0", O_RDWR);
	assert(fd > 0);

	struct gbm_device *gbm = gbm_create_device(fd);
	assert(gbm != NULL);

	eglExtensionString = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
	assert(strstr(eglExtensionString, "EGL_EXT_platform_base") != NULL);
	//
	/* setup EGL from the GBM device */
	EGLDisplay eglDpy = eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA, gbm, NULL);
	assert(eglDpy != NULL);

	res = eglInitialize(eglDpy, NULL, NULL);
	assert(res);

	eglExtensionString = eglQueryString(eglDpy, EGL_EXTENSIONS);
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

	res = eglChooseConfig(eglDpy, config_attribs, &cfg, 1, &count);
	assert(res);

	res = eglBindAPI(EGL_OPENGL_ES_API);
	assert(res);

	static const EGLint attribs[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 3,
		EGL_NONE
	};

	EGLContext coreContext = eglCreateContext(eglDpy,
		cfg,
		EGL_NO_CONTEXT,
		attribs);

	assert(coreContext != EGL_NO_CONTEXT);

	res = eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, coreContext);

	// Support for GPU performance measurements
	if (IsExtensionSupported("GL_EXT_disjoint_timer_query"))
	{
		timerQueriesSupported = true;
	}

	PrepareTextures(fd, eglDpy, coreContext, pixelData);
	GLuint computeHandle = PrepareComputeProgram();

	ExecuteTest(computeHandle);

	// Now dump result (NV12) to a file		
	FILE *f;
	f = fopen("raw.nv12", "wb");
	if (f)
	{
		GLuint fbo;
		GL_CHECK(glGenFramebuffers(1, &fbo));
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
		GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, NV12TextureOutputHandle, 0));

		unsigned char *rgbaPixels = (unsigned char *)malloc(TextureWidth * (TextureHeight + (TextureHeight/2)) * 4);
		if (!rgbaPixels)
		{
			printf("Can't not allocate memory for output!\n");
			exit(1);
		}

		GL_CHECK(glReadPixels(0, 0, TextureWidth, TextureHeight + (TextureHeight / 2), GL_RGBA, GL_UNSIGNED_BYTE, rgbaPixels));

		// Now convert create temporary buffer and convert (RGBA) to output (NV12)
		unsigned char *nv12ImageBuffer = (unsigned char*)malloc(TextureWidth * (TextureHeight + (TextureHeight / 2)));
		if (!nv12ImageBuffer)
		{
			printf("Can't allocate memory for sharing converted buffer \n");
		}

		unsigned int k = 0;
		for (unsigned int i = 0; i < TextureWidth * (TextureHeight + (TextureHeight / 2)) * 4; i += 4)
		{
			nv12ImageBuffer[k] = rgbaPixels[i];
			k++;
		}
		
		GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GL_CHECK(glDeleteFramebuffers(1, &fbo));

		fwrite(nv12ImageBuffer, 1, TextureWidth * (TextureHeight + (TextureHeight / 2)), f);

		fclose(f);

		free(rgbaPixels);
		free(nv12ImageBuffer);
	}
	else
	{
		printf("Problem with accessing output file!\n");
		return 1;
	}
	printf("\n");
	
#ifdef DMA_BUFF_SUPPORT
	if (ImageRGBA)
	{
		eglDestroyImageKHR(eglDpy, ImageRGBA);
	}

	if (ImageNV12)
	{
		eglDestroyImageKHR(eglDpy, ImageNV12);
	}
#endif
	close(fd);
	return 0;
}
