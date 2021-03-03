// This example program creates an EGL surface from a GBM surface.
//
// If the macro EGL_MESA_platform_gbm is defined, then the program
// creates the surfaces using the methods defined in this specification.
// Otherwise, it uses the methods defined by the EGL 1.4 specification.
//
// Compile with `cc -std=c99 example.c -lgbm -lEGL`.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <gbm.h>

// #include <epoxy/gl.h>

#define VIDEO_CARD_FILE "/dev/dri/card0"

struct my_display {
    struct gbm_device *gbmdev;
    EGLDisplay egldisplay;
};

struct my_config {
    struct my_display dpy;
    EGLConfig eglconfig;
};

struct my_window {
    struct my_config config;
    struct gbm_surface *gbmsurface;
    EGLSurface eglsurface;
};

static void check_extensions(void)
{
    const char *client_extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
    puts("client_extensions: ");
    puts(client_extensions);
    puts("\n");

    if (!client_extensions) {
        // EGL_EXT_client_extensions is unsupported.
        fprintf(stderr, "EGL_EXT_client_extensions is unsupported.\n");
        abort();
    }
    if (!strstr(client_extensions, "EGL_MESA_platform_gbm")) {
        fprintf(stderr, "EGL_MESA_platform_gbm not found.\n");
        abort();
    }
}

static struct my_display get_display(void)
{
    struct my_display dpy;

    int card_fd = open(VIDEO_CARD_FILE, O_RDWR | FD_CLOEXEC);
    if (card_fd < 0) {
        fprintf(stderr, "Failed to open card: " VIDEO_CARD_FILE "\n");
        abort();
    }

    dpy.gbmdev = gbm_create_device(card_fd);
    if (!dpy.gbmdev) {
        fprintf(stderr, "gbm_create_device() failed.\n");
        abort();
    }

    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
        (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
    if (!eglGetPlatformDisplayEXT) {
        fprintf(stderr, "Failed to get proc address: eglGetPlatformDisplayEXT \n");
        abort();
    }


    dpy.egldisplay = eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA, dpy.gbmdev, NULL);

    if (dpy.egldisplay == EGL_NO_DISPLAY) {
        fprintf(stderr, "eglGetPlatformDisplayEXT() failed.\n");
        abort();
    }

    EGLint major, minor;
    if (!eglInitialize(dpy.egldisplay, &major, &minor)) {
        fprintf(stderr, "eglInitialize() failed.\n");
        abort();
    }

    return dpy;
}

static struct my_config get_config(struct my_display dpy)
{
    struct my_config config = {
        .dpy = dpy,
    };

    EGLint egl_config_attribs[] = {
        EGL_BUFFER_SIZE,        32,
        EGL_DEPTH_SIZE,         EGL_DONT_CARE,
        EGL_STENCIL_SIZE,       EGL_DONT_CARE,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_NONE,
    };

    EGLint num_configs;
    if (!eglGetConfigs(dpy.egldisplay, NULL, 0, &num_configs)) {
        fprintf(stderr, "eglGetConfigs() failed.\n");
        abort();
    }

    EGLConfig *configs = (EGLConfig *)malloc(num_configs * sizeof(EGLConfig));
    if (!eglChooseConfig(dpy.egldisplay, egl_config_attribs,
                configs, num_configs, &num_configs)) {
        fprintf(stderr, "eglChooseConfig() failed.\n");
        abort();
    }

    fprintf(stdout, "num egl configs = %d\n", num_configs);

    if (num_configs == 0) {
        abort();
    }

    // Find a config whose native visual ID is the desired GBM format.
    int selected_config = -1;
    for (int i = 0; i < num_configs; ++i) {
        EGLint gbm_format;
        EGLint blue_size, red_size, green_size, alpha_size;

        if (!eglGetConfigAttrib(dpy.egldisplay, configs[i],
                    EGL_NATIVE_VISUAL_ID, &gbm_format)) {
            fprintf(stderr, "eglGetConfigAttrib() failed.\n");
            abort();
        }

        eglGetConfigAttrib(dpy.egldisplay, configs[i], EGL_RED_SIZE, &red_size);
        eglGetConfigAttrib(dpy.egldisplay, configs[i], EGL_GREEN_SIZE, &green_size);
        eglGetConfigAttrib(dpy.egldisplay, configs[i], EGL_BLUE_SIZE, &blue_size);
        eglGetConfigAttrib(dpy.egldisplay, configs[i], EGL_ALPHA_SIZE, &alpha_size);

        // gbm_format is actually a fourcc code, constructed as
        //   __gbm_fourcc_code('A', 'R', '1', '5'), so it can be printed
        //   as string
        char gbm_format_str[8] = {0, 0, 0, 0,  0, 0, 0, 0};
        memcpy(gbm_format_str, &gbm_format, sizeof(EGLint));
        printf("  %d-th GBM format: %s;  sizes(RGBA) = %d,%d,%d,%d,\n",
                i, gbm_format_str, red_size, green_size, blue_size, alpha_size);

        if (gbm_format == GBM_FORMAT_XRGB8888) {
            if (selected_config == -1) {
                selected_config = i;
            }
        }

        if (gbm_format == GBM_FORMAT_ARGB8888) {
            if (selected_config == -1) {
                selected_config = i;
            }
        }
    }

    printf("Selected config: %d\n", selected_config);
    if (selected_config >= 0) {
        config.eglconfig = configs[selected_config];
        free(configs);
        return config;
    }

    // Failed to find a config with matching GBM format.
    fprintf(stderr, "Failed to find a config with matching GBM format.\n");
    abort();
}

static struct my_window get_window(struct my_config config)
{
    struct my_window window = {
        .config = config,
    };

    window.gbmsurface = gbm_surface_create(config.dpy.gbmdev,
            256, 256,
            //GBM_FORMAT_XRGB8888,
            GBM_FORMAT_ARGB8888,
            GBM_BO_USE_RENDERING);
    if (!window.gbmsurface) {
        fprintf(stderr, "gbm_surface_create() failed.\n");
        abort();
    }

    PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC eglCreatePlatformWindowSurfaceEXT =
        (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");
    if (!eglCreatePlatformWindowSurfaceEXT) {
        fprintf(stderr, "Failed to get proc address: eglCreatePlatformWindowSurfaceEXT \n");
        abort();
    }

    window.eglsurface = eglCreatePlatformWindowSurfaceEXT(config.dpy.egldisplay,
            config.eglconfig,
            window.gbmsurface,
            NULL);

    if (window.eglsurface == EGL_NO_SURFACE) {
        fprintf(stderr, "eglCreatePlatformWindowSurfaceEXT() failed.\n");
        abort();
    }

    return window;
}


int main()
{
    check_extensions();

    struct my_display dpy = get_display();
    struct my_config config = get_config(dpy);
    struct my_window window = get_window(config);

    printf("If we are here, all is OK. Cleanup\n");

    gbm_surface_destroy(window.gbmsurface);
    window.gbmsurface = NULL;

    return 0;
}

