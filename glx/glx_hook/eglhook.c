#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <stdlib.h>

#include "hook_macro.h"

EGLBoolean eglChooseConfig (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    printf("call eglChooseConfig \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglChooseConfig");
    return real_func(dpy, attrib_list, configs, config_size, num_config);
}

EGLBoolean eglCopyBuffers (EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
    printf("call eglCopyBuffers \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCopyBuffers");
    return real_func(dpy, surface, target);
}

EGLContext eglCreateContext (EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
{
    printf("call eglCreateContext \n");
    EGLContext (*real_func)(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateContext");
    return real_func(dpy, config, share_context, attrib_list);
}

EGLSurface eglCreatePbufferSurface (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
    printf("call eglCreatePbufferSurface \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreatePbufferSurface");
    return real_func(dpy, config, attrib_list);
}

EGLSurface eglCreatePixmapSurface (EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
    printf("call eglCreatePixmapSurface \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreatePixmapSurface");
    return real_func(dpy, config, pixmap, attrib_list);
}

EGLSurface eglCreateWindowSurface (EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
    printf("call eglCreateWindowSurface \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateWindowSurface");
    return real_func(dpy, config, win, attrib_list);
}

EGLBoolean eglDestroyContext (EGLDisplay dpy, EGLContext ctx)
{
    printf("call eglDestroyContext \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLContext ctx);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDestroyContext");
    return real_func(dpy, ctx);
}

EGLBoolean eglDestroySurface (EGLDisplay dpy, EGLSurface surface)
{
    printf("call eglDestroySurface \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDestroySurface");
    return real_func(dpy, surface);
}

EGLBoolean eglGetConfigAttrib (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
    printf("call eglGetConfigAttrib \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetConfigAttrib");
    return real_func(dpy, config, attribute, value);
}

EGLBoolean eglGetConfigs (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    printf("call eglGetConfigs \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetConfigs");
    return real_func(dpy, configs, config_size, num_config);
}

EGLDisplay eglGetCurrentDisplay (void)
{
    printf("call eglGetCurrentDisplay \n");
    EGLDisplay (*real_func)(void);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetCurrentDisplay");
    return real_func();
}

EGLSurface eglGetCurrentSurface (EGLint readdraw)
{
    printf("call eglGetCurrentSurface \n");
    EGLSurface (*real_func)(EGLint readdraw);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetCurrentSurface");
    return real_func(readdraw);
}

EGLDisplay eglGetDisplay (EGLNativeDisplayType display_id)
{
    printf("call eglGetDisplay \n");
    EGLDisplay (*real_func)(EGLNativeDisplayType display_id);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetDisplay");
    return real_func(display_id);
}

EGLint eglGetError (void)
{
    printf("call eglGetError \n");
    EGLint (*real_func)(void);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetError");
    return real_func();
}

EGLBoolean eglInitialize (EGLDisplay dpy, EGLint *major, EGLint *minor)
{
    printf("call eglInitialize \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLint *major, EGLint *minor);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglInitialize");
    return real_func(dpy, major, minor);
}

EGLBoolean eglMakeCurrent (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
    printf("call eglMakeCurrent \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglMakeCurrent");
    return real_func(dpy, draw, read, ctx);
}

EGLBoolean eglQueryContext (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value)
{
    printf("call eglQueryContext \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryContext");
    return real_func(dpy, ctx, attribute, value);
}

EGLBoolean eglQuerySurface (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
    printf("call eglQuerySurface \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQuerySurface");
    return real_func(dpy, surface, attribute, value);
}

EGLBoolean eglSwapBuffers (EGLDisplay dpy, EGLSurface surface)
{
    printf("call eglSwapBuffers \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSwapBuffers");
    return real_func(dpy, surface);
}

EGLBoolean eglTerminate (EGLDisplay dpy)
{
    printf("call eglTerminate \n");
    EGLBoolean (*real_func)(EGLDisplay dpy);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglTerminate");
    return real_func(dpy);
}

EGLBoolean eglWaitGL (void)
{
    printf("call eglWaitGL \n");
    EGLBoolean (*real_func)(void);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglWaitGL");
    return real_func();
}

EGLBoolean eglWaitNative (EGLint engine)
{
    printf("call eglWaitNative \n");
    EGLBoolean (*real_func)(EGLint engine);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglWaitNative");
    return real_func(engine);
}

EGLBoolean eglBindTexImage (EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    printf("call eglBindTexImage \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglBindTexImage");
    return real_func(dpy, surface, buffer);
}

EGLBoolean eglReleaseTexImage (EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    printf("call eglReleaseTexImage \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglReleaseTexImage");
    return real_func(dpy, surface, buffer);
}

EGLBoolean eglSurfaceAttrib (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
{
    printf("call eglSurfaceAttrib \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSurfaceAttrib");
    return real_func(dpy, surface, attribute, value);
}

EGLBoolean eglSwapInterval (EGLDisplay dpy, EGLint interval)
{
    printf("call eglSwapInterval \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLint interval);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSwapInterval");
    return real_func(dpy, interval);
}

EGLBoolean eglBindAPI (EGLenum api)
{
    printf("call eglBindAPI \n");
    EGLBoolean (*real_func)(EGLenum api);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglBindAPI");
    return real_func(api);
}

EGLenum eglQueryAPI (void)
{
    printf("call eglQueryAPI \n");
    EGLenum (*real_func)(void);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryAPI");
    return real_func();
}

EGLSurface eglCreatePbufferFromClientBuffer (EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
{
    printf("call eglCreatePbufferFromClientBuffer \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreatePbufferFromClientBuffer");
    return real_func(dpy, buftype, buffer, config, attrib_list);
}

EGLBoolean eglReleaseThread (void)
{
    printf("call eglReleaseThread \n");
    EGLBoolean (*real_func)(void);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglReleaseThread");
    return real_func();
}

EGLBoolean eglWaitClient (void)
{
    printf("call eglWaitClient \n");
    EGLBoolean (*real_func)(void);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglWaitClient");
    return real_func();
}

EGLContext eglGetCurrentContext (void)
{
    printf("call eglGetCurrentContext \n");
    EGLContext (*real_func)(void);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetCurrentContext");
    return real_func();
}

EGLSync eglCreateSync (EGLDisplay dpy, EGLenum type, const EGLAttrib *attrib_list)
{
    printf("call eglCreateSync \n");
    EGLSync (*real_func)(EGLDisplay dpy, EGLenum type, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateSync");
    return real_func(dpy, type, attrib_list);
}

EGLBoolean eglDestroySync (EGLDisplay dpy, EGLSync sync)
{
    printf("call eglDestroySync \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSync sync);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDestroySync");
    return real_func(dpy, sync);
}

EGLint eglClientWaitSync (EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout)
{
    printf("call eglClientWaitSync \n");
    EGLint (*real_func)(EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglClientWaitSync");
    return real_func(dpy, sync, flags, timeout);
}

EGLBoolean eglGetSyncAttrib (EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib *value)
{
    printf("call eglGetSyncAttrib \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetSyncAttrib");
    return real_func(dpy, sync, attribute, value);
}

EGLImage eglCreateImage (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLAttrib *attrib_list)
{
    printf("call eglCreateImage \n");
    EGLImage (*real_func)(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateImage");
    return real_func(dpy, ctx, target, buffer, attrib_list);
}

EGLBoolean eglDestroyImage (EGLDisplay dpy, EGLImage image)
{
    printf("call eglDestroyImage \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLImage image);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDestroyImage");
    return real_func(dpy, image);
}

EGLDisplay eglGetPlatformDisplay (EGLenum platform, void *native_display, const EGLAttrib *attrib_list)
{
    printf("call eglGetPlatformDisplay \n");
    EGLDisplay (*real_func)(EGLenum platform, void *native_display, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetPlatformDisplay");
    return real_func(platform, native_display, attrib_list);
}

EGLSurface eglCreatePlatformWindowSurface (EGLDisplay dpy, EGLConfig config, void *native_window, const EGLAttrib *attrib_list)
{
    printf("call eglCreatePlatformWindowSurface \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLConfig config, void *native_window, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreatePlatformWindowSurface");
    return real_func(dpy, config, native_window, attrib_list);
}

EGLSurface eglCreatePlatformPixmapSurface (EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLAttrib *attrib_list)
{
    printf("call eglCreatePlatformPixmapSurface \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreatePlatformPixmapSurface");
    return real_func(dpy, config, native_pixmap, attrib_list);
}

EGLBoolean eglWaitSync (EGLDisplay dpy, EGLSync sync, EGLint flags)
{
    printf("call eglWaitSync \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSync sync, EGLint flags);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglWaitSync");
    return real_func(dpy, sync, flags);
}

EGLSyncKHR eglCreateSync64KHR (EGLDisplay dpy, EGLenum type, const EGLAttribKHR *attrib_list)
{
    printf("call eglCreateSync64KHR \n");
    EGLSyncKHR (*real_func)(EGLDisplay dpy, EGLenum type, const EGLAttribKHR *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateSync64KHR");
    return real_func(dpy, type, attrib_list);
}

EGLint eglDebugMessageControlKHR (EGLDEBUGPROCKHR callback, const EGLAttrib *attrib_list)
{
    printf("call eglDebugMessageControlKHR \n");
    EGLint (*real_func)(EGLDEBUGPROCKHR callback, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDebugMessageControlKHR");
    return real_func(callback, attrib_list);
}

EGLBoolean eglQueryDebugKHR (EGLint attribute, EGLAttrib *value)
{
    printf("call eglQueryDebugKHR \n");
    EGLBoolean (*real_func)(EGLint attribute, EGLAttrib *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryDebugKHR");
    return real_func(attribute, value);
}

EGLint eglLabelObjectKHR (EGLDisplay display, EGLenum objectType, EGLObjectKHR object, EGLLabelKHR label)
{
    printf("call eglLabelObjectKHR \n");
    EGLint (*real_func)(EGLDisplay display, EGLenum objectType, EGLObjectKHR object, EGLLabelKHR label);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglLabelObjectKHR");
    return real_func(display, objectType, object, label);
}

EGLBoolean eglQueryDisplayAttribKHR (EGLDisplay dpy, EGLint name, EGLAttrib *value)
{
    printf("call eglQueryDisplayAttribKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLint name, EGLAttrib *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryDisplayAttribKHR");
    return real_func(dpy, name, value);
}

EGLSyncKHR eglCreateSyncKHR (EGLDisplay dpy, EGLenum type, const EGLint *attrib_list)
{
    printf("call eglCreateSyncKHR \n");
    EGLSyncKHR (*real_func)(EGLDisplay dpy, EGLenum type, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateSyncKHR");
    return real_func(dpy, type, attrib_list);
}

EGLBoolean eglDestroySyncKHR (EGLDisplay dpy, EGLSyncKHR sync)
{
    printf("call eglDestroySyncKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSyncKHR sync);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDestroySyncKHR");
    return real_func(dpy, sync);
}

EGLint eglClientWaitSyncKHR (EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout)
{
    printf("call eglClientWaitSyncKHR \n");
    EGLint (*real_func)(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglClientWaitSyncKHR");
    return real_func(dpy, sync, flags, timeout);
}

EGLBoolean eglGetSyncAttribKHR (EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint *value)
{
    printf("call eglGetSyncAttribKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetSyncAttribKHR");
    return real_func(dpy, sync, attribute, value);
}

EGLImageKHR eglCreateImageKHR (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list)
{
    printf("call eglCreateImageKHR \n");
    EGLImageKHR (*real_func)(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateImageKHR");
    return real_func(dpy, ctx, target, buffer, attrib_list);
}

EGLBoolean eglDestroyImageKHR (EGLDisplay dpy, EGLImageKHR image)
{
    printf("call eglDestroyImageKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLImageKHR image);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDestroyImageKHR");
    return real_func(dpy, image);
}

EGLBoolean eglLockSurfaceKHR (EGLDisplay dpy, EGLSurface surface, const EGLint *attrib_list)
{
    printf("call eglLockSurfaceKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglLockSurfaceKHR");
    return real_func(dpy, surface, attrib_list);
}

EGLBoolean eglUnlockSurfaceKHR (EGLDisplay dpy, EGLSurface surface)
{
    printf("call eglUnlockSurfaceKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglUnlockSurfaceKHR");
    return real_func(dpy, surface);
}

EGLBoolean eglQuerySurface64KHR (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLAttribKHR *value)
{
    printf("call eglQuerySurface64KHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLAttribKHR *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQuerySurface64KHR");
    return real_func(dpy, surface, attribute, value);
}

EGLBoolean eglSetDamageRegionKHR (EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects)
{
    printf("call eglSetDamageRegionKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSetDamageRegionKHR");
    return real_func(dpy, surface, rects, n_rects);
}

EGLBoolean eglSignalSyncKHR (EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode)
{
    printf("call eglSignalSyncKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSignalSyncKHR");
    return real_func(dpy, sync, mode);
}

EGLStreamKHR eglCreateStreamKHR (EGLDisplay dpy, const EGLint *attrib_list)
{
    printf("call eglCreateStreamKHR \n");
    EGLStreamKHR (*real_func)(EGLDisplay dpy, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateStreamKHR");
    return real_func(dpy, attrib_list);
}

EGLBoolean eglDestroyStreamKHR (EGLDisplay dpy, EGLStreamKHR stream)
{
    printf("call eglDestroyStreamKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDestroyStreamKHR");
    return real_func(dpy, stream);
}

EGLBoolean eglStreamAttribKHR (EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint value)
{
    printf("call eglStreamAttribKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglStreamAttribKHR");
    return real_func(dpy, stream, attribute, value);
}

EGLBoolean eglQueryStreamKHR (EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint *value)
{
    printf("call eglQueryStreamKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLint *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryStreamKHR");
    return real_func(dpy, stream, attribute, value);
}

EGLBoolean eglQueryStreamu64KHR (EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLuint64KHR *value)
{
    printf("call eglQueryStreamu64KHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLuint64KHR *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryStreamu64KHR");
    return real_func(dpy, stream, attribute, value);
}

EGLStreamKHR eglCreateStreamAttribKHR (EGLDisplay dpy, const EGLAttrib *attrib_list)
{
    printf("call eglCreateStreamAttribKHR \n");
    EGLStreamKHR (*real_func)(EGLDisplay dpy, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateStreamAttribKHR");
    return real_func(dpy, attrib_list);
}

EGLBoolean eglSetStreamAttribKHR (EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLAttrib value)
{
    printf("call eglSetStreamAttribKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLAttrib value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSetStreamAttribKHR");
    return real_func(dpy, stream, attribute, value);
}

EGLBoolean eglQueryStreamAttribKHR (EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLAttrib *value)
{
    printf("call eglQueryStreamAttribKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLAttrib *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryStreamAttribKHR");
    return real_func(dpy, stream, attribute, value);
}

EGLBoolean eglStreamConsumerAcquireAttribKHR (EGLDisplay dpy, EGLStreamKHR stream, const EGLAttrib *attrib_list)
{
    printf("call eglStreamConsumerAcquireAttribKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglStreamConsumerAcquireAttribKHR");
    return real_func(dpy, stream, attrib_list);
}

EGLBoolean eglStreamConsumerReleaseAttribKHR (EGLDisplay dpy, EGLStreamKHR stream, const EGLAttrib *attrib_list)
{
    printf("call eglStreamConsumerReleaseAttribKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglStreamConsumerReleaseAttribKHR");
    return real_func(dpy, stream, attrib_list);
}

EGLBoolean eglStreamConsumerGLTextureExternalKHR (EGLDisplay dpy, EGLStreamKHR stream)
{
    printf("call eglStreamConsumerGLTextureExternalKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglStreamConsumerGLTextureExternalKHR");
    return real_func(dpy, stream);
}

EGLBoolean eglStreamConsumerAcquireKHR (EGLDisplay dpy, EGLStreamKHR stream)
{
    printf("call eglStreamConsumerAcquireKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglStreamConsumerAcquireKHR");
    return real_func(dpy, stream);
}

EGLBoolean eglStreamConsumerReleaseKHR (EGLDisplay dpy, EGLStreamKHR stream)
{
    printf("call eglStreamConsumerReleaseKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglStreamConsumerReleaseKHR");
    return real_func(dpy, stream);
}

EGLNativeFileDescriptorKHR eglGetStreamFileDescriptorKHR (EGLDisplay dpy, EGLStreamKHR stream)
{
    printf("call eglGetStreamFileDescriptorKHR \n");
    EGLNativeFileDescriptorKHR (*real_func)(EGLDisplay dpy, EGLStreamKHR stream);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetStreamFileDescriptorKHR");
    return real_func(dpy, stream);
}

EGLStreamKHR eglCreateStreamFromFileDescriptorKHR (EGLDisplay dpy, EGLNativeFileDescriptorKHR file_descriptor)
{
    printf("call eglCreateStreamFromFileDescriptorKHR \n");
    EGLStreamKHR (*real_func)(EGLDisplay dpy, EGLNativeFileDescriptorKHR file_descriptor);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateStreamFromFileDescriptorKHR");
    return real_func(dpy, file_descriptor);
}

EGLBoolean eglQueryStreamTimeKHR (EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLTimeKHR *value)
{
    printf("call eglQueryStreamTimeKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum attribute, EGLTimeKHR *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryStreamTimeKHR");
    return real_func(dpy, stream, attribute, value);
}

EGLSurface eglCreateStreamProducerSurfaceKHR (EGLDisplay dpy, EGLConfig config, EGLStreamKHR stream, const EGLint *attrib_list)
{
    printf("call eglCreateStreamProducerSurfaceKHR \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLConfig config, EGLStreamKHR stream, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateStreamProducerSurfaceKHR");
    return real_func(dpy, config, stream, attrib_list);
}

EGLBoolean eglSwapBuffersWithDamageKHR (EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects)
{
    printf("call eglSwapBuffersWithDamageKHR \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSwapBuffersWithDamageKHR");
    return real_func(dpy, surface, rects, n_rects);
}

EGLint eglWaitSyncKHR (EGLDisplay dpy, EGLSyncKHR sync, EGLint flags)
{
    printf("call eglWaitSyncKHR \n");
    EGLint (*real_func)(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglWaitSyncKHR");
    return real_func(dpy, sync, flags);
}

void eglSetBlobCacheFuncsANDROID (EGLDisplay dpy, EGLSetBlobFuncANDROID set, EGLGetBlobFuncANDROID get)
{
    printf("call eglSetBlobCacheFuncsANDROID \n");
    void (*real_func)(EGLDisplay dpy, EGLSetBlobFuncANDROID set, EGLGetBlobFuncANDROID get);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSetBlobCacheFuncsANDROID");
    return real_func(dpy, set, get);
}

EGLClientBuffer eglCreateNativeClientBufferANDROID (const EGLint *attrib_list)
{
    printf("call eglCreateNativeClientBufferANDROID \n");
    EGLClientBuffer (*real_func)(const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateNativeClientBufferANDROID");
    return real_func(attrib_list);
}

EGLBoolean eglGetCompositorTimingSupportedANDROID (EGLDisplay dpy, EGLSurface surface, EGLint name)
{
    printf("call eglGetCompositorTimingSupportedANDROID \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint name);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetCompositorTimingSupportedANDROID");
    return real_func(dpy, surface, name);
}

EGLBoolean eglGetCompositorTimingANDROID (EGLDisplay dpy, EGLSurface surface, EGLint numTimestamps,  const EGLint *names, EGLnsecsANDROID *values)
{
    printf("call eglGetCompositorTimingANDROID \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint numTimestamps,  const EGLint *names, EGLnsecsANDROID *values);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetCompositorTimingANDROID");
    return real_func(dpy, surface, numTimestamps, names, values);
}

EGLBoolean eglGetNextFrameIdANDROID (EGLDisplay dpy, EGLSurface surface, EGLuint64KHR *frameId)
{
    printf("call eglGetNextFrameIdANDROID \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLuint64KHR *frameId);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetNextFrameIdANDROID");
    return real_func(dpy, surface, frameId);
}

EGLBoolean eglGetFrameTimestampSupportedANDROID (EGLDisplay dpy, EGLSurface surface, EGLint timestamp)
{
    printf("call eglGetFrameTimestampSupportedANDROID \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint timestamp);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetFrameTimestampSupportedANDROID");
    return real_func(dpy, surface, timestamp);
}

EGLBoolean eglGetFrameTimestampsANDROID (EGLDisplay dpy, EGLSurface surface, EGLuint64KHR frameId, EGLint numTimestamps,  const EGLint *timestamps, EGLnsecsANDROID *values)
{
    printf("call eglGetFrameTimestampsANDROID \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLuint64KHR frameId, EGLint numTimestamps,  const EGLint *timestamps, EGLnsecsANDROID *values);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetFrameTimestampsANDROID");
    return real_func(dpy, surface, frameId, numTimestamps, timestamps, values);
}

#if 0
EGLClientBuffer eglGetNativeClientBufferANDROID (const struct AHardwareBuffer *buffer)
{
    printf("call eglGetNativeClientBufferANDROID \n");
    EGLClientBuffer (*real_func)(const struct AHardwareBuffer *buffer);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetNativeClientBufferANDROID");
    return real_func(buffer);
}
#endif

EGLint eglDupNativeFenceFDANDROID (EGLDisplay dpy, EGLSyncKHR sync)
{
    printf("call eglDupNativeFenceFDANDROID \n");
    EGLint (*real_func)(EGLDisplay dpy, EGLSyncKHR sync);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDupNativeFenceFDANDROID");
    return real_func(dpy, sync);
}

EGLBoolean eglPresentationTimeANDROID (EGLDisplay dpy, EGLSurface surface, EGLnsecsANDROID time)
{
    printf("call eglPresentationTimeANDROID \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLnsecsANDROID time);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglPresentationTimeANDROID");
    return real_func(dpy, surface, time);
}

EGLBoolean eglQuerySurfacePointerANGLE (EGLDisplay dpy, EGLSurface surface, EGLint attribute, void **value)
{
    printf("call eglQuerySurfacePointerANGLE \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, void **value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQuerySurfacePointerANGLE");
    return real_func(dpy, surface, attribute, value);
}

EGLBoolean eglClientSignalSyncEXT (EGLDisplay dpy, EGLSync sync, const EGLAttrib *attrib_list)
{
    printf("call eglClientSignalSyncEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSync sync, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglClientSignalSyncEXT");
    return real_func(dpy, sync, attrib_list);
}

EGLBoolean eglCompositorSetContextListEXT (const EGLint *external_ref_ids, EGLint num_entries)
{
    printf("call eglCompositorSetContextListEXT \n");
    EGLBoolean (*real_func)(const EGLint *external_ref_ids, EGLint num_entries);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCompositorSetContextListEXT");
    return real_func(external_ref_ids, num_entries);
}

EGLBoolean eglCompositorSetContextAttributesEXT (EGLint external_ref_id, const EGLint *context_attributes, EGLint num_entries)
{
    printf("call eglCompositorSetContextAttributesEXT \n");
    EGLBoolean (*real_func)(EGLint external_ref_id, const EGLint *context_attributes, EGLint num_entries);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCompositorSetContextAttributesEXT");
    return real_func(external_ref_id, context_attributes, num_entries);
}

EGLBoolean eglCompositorSetWindowListEXT (EGLint external_ref_id, const EGLint *external_win_ids, EGLint num_entries)
{
    printf("call eglCompositorSetWindowListEXT \n");
    EGLBoolean (*real_func)(EGLint external_ref_id, const EGLint *external_win_ids, EGLint num_entries);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCompositorSetWindowListEXT");
    return real_func(external_ref_id, external_win_ids, num_entries);
}

EGLBoolean eglCompositorSetWindowAttributesEXT (EGLint external_win_id, const EGLint *window_attributes, EGLint num_entries)
{
    printf("call eglCompositorSetWindowAttributesEXT \n");
    EGLBoolean (*real_func)(EGLint external_win_id, const EGLint *window_attributes, EGLint num_entries);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCompositorSetWindowAttributesEXT");
    return real_func(external_win_id, window_attributes, num_entries);
}

EGLBoolean eglCompositorBindTexWindowEXT (EGLint external_win_id)
{
    printf("call eglCompositorBindTexWindowEXT \n");
    EGLBoolean (*real_func)(EGLint external_win_id);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCompositorBindTexWindowEXT");
    return real_func(external_win_id);
}

EGLBoolean eglCompositorSetSizeEXT (EGLint external_win_id, EGLint width, EGLint height)
{
    printf("call eglCompositorSetSizeEXT \n");
    EGLBoolean (*real_func)(EGLint external_win_id, EGLint width, EGLint height);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCompositorSetSizeEXT");
    return real_func(external_win_id, width, height);
}

EGLBoolean eglCompositorSwapPolicyEXT (EGLint external_win_id, EGLint policy)
{
    printf("call eglCompositorSwapPolicyEXT \n");
    EGLBoolean (*real_func)(EGLint external_win_id, EGLint policy);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCompositorSwapPolicyEXT");
    return real_func(external_win_id, policy);
}

EGLBoolean eglQueryDeviceAttribEXT (EGLDeviceEXT device, EGLint attribute, EGLAttrib *value)
{
    printf("call eglQueryDeviceAttribEXT \n");
    EGLBoolean (*real_func)(EGLDeviceEXT device, EGLint attribute, EGLAttrib *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryDeviceAttribEXT");
    return real_func(device, attribute, value);
}

const char * eglQueryDeviceStringEXT (EGLDeviceEXT device, EGLint name)
{
    printf("call eglQueryDeviceStringEXT \n");
    const char * (*real_func)(EGLDeviceEXT device, EGLint name);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryDeviceStringEXT");
    return real_func(device, name);
}

EGLBoolean eglQueryDevicesEXT (EGLint max_devices, EGLDeviceEXT *devices, EGLint *num_devices)
{
    printf("call eglQueryDevicesEXT \n");
    EGLBoolean (*real_func)(EGLint max_devices, EGLDeviceEXT *devices, EGLint *num_devices);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryDevicesEXT");
    return real_func(max_devices, devices, num_devices);
}

EGLBoolean eglQueryDisplayAttribEXT (EGLDisplay dpy, EGLint attribute, EGLAttrib *value)
{
    printf("call eglQueryDisplayAttribEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLint attribute, EGLAttrib *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryDisplayAttribEXT");
    return real_func(dpy, attribute, value);
}

EGLBoolean eglQueryDmaBufFormatsEXT (EGLDisplay dpy, EGLint max_formats, EGLint *formats, EGLint *num_formats)
{
    printf("call eglQueryDmaBufFormatsEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLint max_formats, EGLint *formats, EGLint *num_formats);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryDmaBufFormatsEXT");
    return real_func(dpy, max_formats, formats, num_formats);
}

EGLBoolean eglQueryDmaBufModifiersEXT (EGLDisplay dpy, EGLint format, EGLint max_modifiers, EGLuint64KHR *modifiers, EGLBoolean *external_only, EGLint *num_modifiers)
{
    printf("call eglQueryDmaBufModifiersEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLint format, EGLint max_modifiers, EGLuint64KHR *modifiers, EGLBoolean *external_only, EGLint *num_modifiers);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryDmaBufModifiersEXT");
    return real_func(dpy, format, max_modifiers, modifiers, external_only, num_modifiers);
}

EGLBoolean eglGetOutputLayersEXT (EGLDisplay dpy, const EGLAttrib *attrib_list, EGLOutputLayerEXT *layers, EGLint max_layers, EGLint *num_layers)
{
    printf("call eglGetOutputLayersEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, const EGLAttrib *attrib_list, EGLOutputLayerEXT *layers, EGLint max_layers, EGLint *num_layers);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetOutputLayersEXT");
    return real_func(dpy, attrib_list, layers, max_layers, num_layers);
}

EGLBoolean eglGetOutputPortsEXT (EGLDisplay dpy, const EGLAttrib *attrib_list, EGLOutputPortEXT *ports, EGLint max_ports, EGLint *num_ports)
{
    printf("call eglGetOutputPortsEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, const EGLAttrib *attrib_list, EGLOutputPortEXT *ports, EGLint max_ports, EGLint *num_ports);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetOutputPortsEXT");
    return real_func(dpy, attrib_list, ports, max_ports, num_ports);
}

EGLBoolean eglOutputLayerAttribEXT (EGLDisplay dpy, EGLOutputLayerEXT layer, EGLint attribute, EGLAttrib value)
{
    printf("call eglOutputLayerAttribEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLOutputLayerEXT layer, EGLint attribute, EGLAttrib value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglOutputLayerAttribEXT");
    return real_func(dpy, layer, attribute, value);
}

EGLBoolean eglQueryOutputLayerAttribEXT (EGLDisplay dpy, EGLOutputLayerEXT layer, EGLint attribute, EGLAttrib *value)
{
    printf("call eglQueryOutputLayerAttribEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLOutputLayerEXT layer, EGLint attribute, EGLAttrib *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryOutputLayerAttribEXT");
    return real_func(dpy, layer, attribute, value);
}

const char * eglQueryOutputLayerStringEXT (EGLDisplay dpy, EGLOutputLayerEXT layer, EGLint name)
{
    printf("call eglQueryOutputLayerStringEXT \n");
    const char * (*real_func)(EGLDisplay dpy, EGLOutputLayerEXT layer, EGLint name);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryOutputLayerStringEXT");
    return real_func(dpy, layer, name);
}

EGLBoolean eglOutputPortAttribEXT (EGLDisplay dpy, EGLOutputPortEXT port, EGLint attribute, EGLAttrib value)
{
    printf("call eglOutputPortAttribEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLOutputPortEXT port, EGLint attribute, EGLAttrib value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglOutputPortAttribEXT");
    return real_func(dpy, port, attribute, value);
}

EGLBoolean eglQueryOutputPortAttribEXT (EGLDisplay dpy, EGLOutputPortEXT port, EGLint attribute, EGLAttrib *value)
{
    printf("call eglQueryOutputPortAttribEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLOutputPortEXT port, EGLint attribute, EGLAttrib *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryOutputPortAttribEXT");
    return real_func(dpy, port, attribute, value);
}

const char * eglQueryOutputPortStringEXT (EGLDisplay dpy, EGLOutputPortEXT port, EGLint name)
{
    printf("call eglQueryOutputPortStringEXT \n");
    const char * (*real_func)(EGLDisplay dpy, EGLOutputPortEXT port, EGLint name);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryOutputPortStringEXT");
    return real_func(dpy, port, name);
}

EGLDisplay eglGetPlatformDisplayEXT (EGLenum platform, void *native_display, const EGLint *attrib_list)
{
    printf("call eglGetPlatformDisplayEXT \n");
    EGLDisplay (*real_func)(EGLenum platform, void *native_display, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetPlatformDisplayEXT");
    if(!real_func) return NULL; 
    return real_func(platform, native_display, attrib_list);
}

EGLSurface eglCreatePlatformWindowSurfaceEXT (EGLDisplay dpy, EGLConfig config, void *native_window, const EGLint *attrib_list)
{
    printf("call eglCreatePlatformWindowSurfaceEXT \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLConfig config, void *native_window, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreatePlatformWindowSurfaceEXT");
    return real_func(dpy, config, native_window, attrib_list);
}

EGLSurface eglCreatePlatformPixmapSurfaceEXT (EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLint *attrib_list)
{
    printf("call eglCreatePlatformPixmapSurfaceEXT \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreatePlatformPixmapSurfaceEXT");
    return real_func(dpy, config, native_pixmap, attrib_list);
}

EGLBoolean eglStreamConsumerOutputEXT (EGLDisplay dpy, EGLStreamKHR stream, EGLOutputLayerEXT layer)
{
    printf("call eglStreamConsumerOutputEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLOutputLayerEXT layer);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglStreamConsumerOutputEXT");
    return real_func(dpy, stream, layer);
}

EGLBoolean eglSwapBuffersWithDamageEXT (EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects)
{
    printf("call eglSwapBuffersWithDamageEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSwapBuffersWithDamageEXT");
    return real_func(dpy, surface, rects, n_rects);
}

EGLBoolean eglUnsignalSyncEXT (EGLDisplay dpy, EGLSync sync, const EGLAttrib *attrib_list)
{
    printf("call eglUnsignalSyncEXT \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSync sync, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglUnsignalSyncEXT");
    return real_func(dpy, sync, attrib_list);
}

EGLSurface eglCreatePixmapSurfaceHI (EGLDisplay dpy, EGLConfig config, struct EGLClientPixmapHI *pixmap)
{
    printf("call eglCreatePixmapSurfaceHI \n");
    EGLSurface (*real_func)(EGLDisplay dpy, EGLConfig config, struct EGLClientPixmapHI *pixmap);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreatePixmapSurfaceHI");
    return real_func(dpy, config, pixmap);
}

EGLImageKHR eglCreateDRMImageMESA (EGLDisplay dpy, const EGLint *attrib_list)
{
    printf("call eglCreateDRMImageMESA \n");
    EGLImageKHR (*real_func)(EGLDisplay dpy, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateDRMImageMESA");
    return real_func(dpy, attrib_list);
}

EGLBoolean eglExportDRMImageMESA (EGLDisplay dpy, EGLImageKHR image, EGLint *name, EGLint *handle, EGLint *stride)
{
    printf("call eglExportDRMImageMESA \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLImageKHR image, EGLint *name, EGLint *handle, EGLint *stride);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglExportDRMImageMESA");
    return real_func(dpy, image, name, handle, stride);
}

EGLBoolean eglExportDMABUFImageQueryMESA (EGLDisplay dpy, EGLImageKHR image, int *fourcc, int *num_planes, EGLuint64KHR *modifiers)
{
    printf("call eglExportDMABUFImageQueryMESA \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLImageKHR image, int *fourcc, int *num_planes, EGLuint64KHR *modifiers);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglExportDMABUFImageQueryMESA");
    return real_func(dpy, image, fourcc, num_planes, modifiers);
}

EGLBoolean eglExportDMABUFImageMESA (EGLDisplay dpy, EGLImageKHR image, int *fds, EGLint *strides, EGLint *offsets)
{
    printf("call eglExportDMABUFImageMESA \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLImageKHR image, int *fds, EGLint *strides, EGLint *offsets);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglExportDMABUFImageMESA");
    return real_func(dpy, image, fds, strides, offsets);
}

char * eglGetDisplayDriverConfig (EGLDisplay dpy)
{
    printf("call eglGetDisplayDriverConfig \n");
    char * (*real_func)(EGLDisplay dpy);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetDisplayDriverConfig");
    return real_func(dpy);
}

const char * eglGetDisplayDriverName (EGLDisplay dpy)
{
    printf("call eglGetDisplayDriverName \n");
    const char * (*real_func)(EGLDisplay dpy);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetDisplayDriverName");
    return real_func(dpy);
}

EGLBoolean eglSwapBuffersRegionNOK (EGLDisplay dpy, EGLSurface surface, EGLint numRects, const EGLint *rects)
{
    printf("call eglSwapBuffersRegionNOK \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint numRects, const EGLint *rects);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSwapBuffersRegionNOK");
    return real_func(dpy, surface, numRects, rects);
}

EGLBoolean eglSwapBuffersRegion2NOK (EGLDisplay dpy, EGLSurface surface, EGLint numRects, const EGLint *rects)
{
    printf("call eglSwapBuffersRegion2NOK \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint numRects, const EGLint *rects);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSwapBuffersRegion2NOK");
    return real_func(dpy, surface, numRects, rects);
}

EGLBoolean eglQueryNativeDisplayNV (EGLDisplay dpy, EGLNativeDisplayType *display_id)
{
    printf("call eglQueryNativeDisplayNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLNativeDisplayType *display_id);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryNativeDisplayNV");
    return real_func(dpy, display_id);
}

EGLBoolean eglQueryNativeWindowNV (EGLDisplay dpy, EGLSurface surf, EGLNativeWindowType *window)
{
    printf("call eglQueryNativeWindowNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surf, EGLNativeWindowType *window);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryNativeWindowNV");
    return real_func(dpy, surf, window);
}

EGLBoolean eglQueryNativePixmapNV (EGLDisplay dpy, EGLSurface surf, EGLNativePixmapType *pixmap)
{
    printf("call eglQueryNativePixmapNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surf, EGLNativePixmapType *pixmap);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryNativePixmapNV");
    return real_func(dpy, surf, pixmap);
}

EGLBoolean eglPostSubBufferNV (EGLDisplay dpy, EGLSurface surface, EGLint x, EGLint y, EGLint width, EGLint height)
{
    printf("call eglPostSubBufferNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLSurface surface, EGLint x, EGLint y, EGLint width, EGLint height);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglPostSubBufferNV");
    return real_func(dpy, surface, x, y, width, height);
}

EGLBoolean eglStreamConsumerGLTextureExternalAttribsNV (EGLDisplay dpy, EGLStreamKHR stream, const EGLAttrib *attrib_list)
{
    printf("call eglStreamConsumerGLTextureExternalAttribsNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, const EGLAttrib *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglStreamConsumerGLTextureExternalAttribsNV");
    return real_func(dpy, stream, attrib_list);
}

EGLBoolean eglStreamFlushNV (EGLDisplay dpy, EGLStreamKHR stream)
{
    printf("call eglStreamFlushNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglStreamFlushNV");
    return real_func(dpy, stream);
}

EGLBoolean eglQueryDisplayAttribNV (EGLDisplay dpy, EGLint attribute, EGLAttrib *value)
{
    printf("call eglQueryDisplayAttribNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLint attribute, EGLAttrib *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryDisplayAttribNV");
    return real_func(dpy, attribute, value);
}

EGLBoolean eglSetStreamMetadataNV (EGLDisplay dpy, EGLStreamKHR stream, EGLint n, EGLint offset, EGLint size, const void *data)
{
    printf("call eglSetStreamMetadataNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLint n, EGLint offset, EGLint size, const void *data);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSetStreamMetadataNV");
    return real_func(dpy, stream, n, offset, size, data);
}

EGLBoolean eglQueryStreamMetadataNV (EGLDisplay dpy, EGLStreamKHR stream, EGLenum name, EGLint n, EGLint offset, EGLint size, void *data)
{
    printf("call eglQueryStreamMetadataNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum name, EGLint n, EGLint offset, EGLint size, void *data);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryStreamMetadataNV");
    return real_func(dpy, stream, name, n, offset, size, data);
}

EGLBoolean eglResetStreamNV (EGLDisplay dpy, EGLStreamKHR stream)
{
    printf("call eglResetStreamNV \n");
    EGLBoolean (*real_func)(EGLDisplay dpy, EGLStreamKHR stream);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglResetStreamNV");
    return real_func(dpy, stream);
}

EGLSyncKHR eglCreateStreamSyncNV (EGLDisplay dpy, EGLStreamKHR stream, EGLenum type, const EGLint *attrib_list)
{
    printf("call eglCreateStreamSyncNV \n");
    EGLSyncKHR (*real_func)(EGLDisplay dpy, EGLStreamKHR stream, EGLenum type, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateStreamSyncNV");
    return real_func(dpy, stream, type, attrib_list);
}

EGLSyncNV eglCreateFenceSyncNV (EGLDisplay dpy, EGLenum condition, const EGLint *attrib_list)
{
    printf("call eglCreateFenceSyncNV \n");
    EGLSyncNV (*real_func)(EGLDisplay dpy, EGLenum condition, const EGLint *attrib_list);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglCreateFenceSyncNV");
    return real_func(dpy, condition, attrib_list);
}

EGLBoolean eglDestroySyncNV (EGLSyncNV sync)
{
    printf("call eglDestroySyncNV \n");
    EGLBoolean (*real_func)(EGLSyncNV sync);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglDestroySyncNV");
    return real_func(sync);
}

EGLBoolean eglFenceNV (EGLSyncNV sync)
{
    printf("call eglFenceNV \n");
    EGLBoolean (*real_func)(EGLSyncNV sync);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglFenceNV");
    return real_func(sync);
}

EGLint eglClientWaitSyncNV (EGLSyncNV sync, EGLint flags, EGLTimeNV timeout)
{
    printf("call eglClientWaitSyncNV \n");
    EGLint (*real_func)(EGLSyncNV sync, EGLint flags, EGLTimeNV timeout);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglClientWaitSyncNV");
    return real_func(sync, flags, timeout);
}

EGLBoolean eglSignalSyncNV (EGLSyncNV sync, EGLenum mode)
{
    printf("call eglSignalSyncNV \n");
    EGLBoolean (*real_func)(EGLSyncNV sync, EGLenum mode);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglSignalSyncNV");
    return real_func(sync, mode);
}

EGLBoolean eglGetSyncAttribNV (EGLSyncNV sync, EGLint attribute, EGLint *value)
{
    printf("call eglGetSyncAttribNV \n");
    EGLBoolean (*real_func)(EGLSyncNV sync, EGLint attribute, EGLint *value);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetSyncAttribNV");
    return real_func(sync, attribute, value);
}

EGLuint64NV eglGetSystemTimeFrequencyNV (void)
{
    printf("call eglGetSystemTimeFrequencyNV \n");
    EGLuint64NV (*real_func)(void);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetSystemTimeFrequencyNV");
    return real_func();
}

EGLuint64NV eglGetSystemTimeNV (void)
{
    printf("call eglGetSystemTimeNV \n");
    EGLuint64NV (*real_func)(void);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglGetSystemTimeNV");
    return real_func();
}

const char * eglQueryString (EGLDisplay dpy, EGLint name)
{
    printf("call eglQueryString \n");
    const char * (*real_func)(EGLDisplay dpy, EGLint name);
    real_func = dlwrap_real_dlsym(RTLD_NEXT, "eglQueryString");
    if(!real_func) {
		void *handle = dlopen("libEGL.so", RTLD_LAZY | RTLD_LOCAL);
		if(!handle) {
			printf("couldn't open libEGL.so: %s\n", dlerror());
			return NULL;
		}
		real_func = dlwrap_real_dlsym(handle, "eglQueryString");
		if(!real_func) {
			printf("couldn't find eglQueryString: %s\n", dlerror());
			return NULL;
		}
		const char *result = real_func(dpy, name);
		dlclose(handle);
		printf("eglQueryString: %s\n", result);
		return result;
	}
    return real_func(dpy, name);
}

eglGPA_t real_eglGPA = NULL;
__eglMustCastToProperFunctionPointerType eglGetProcAddress (const char *fname)
{
    printf("call eglGetProcAddress(%s) \n", fname);
    
    if(fname && fname[0] == 'e' && fname[1] == 'g' && fname[2] == 'l') {
        //EGL_API_HOOKS(fname);
    }
    //EGL_GL_HOOKS(fname);
    
    //printf("*********eglGetProcAddress: search (%s)*********\n", fname);
    
	if(real_eglGPA) {
		return real_eglGPA(fname);
	} else {
		real_eglGPA = dlwrap_real_dlsym(RTLD_NEXT, "eglGetProcAddress");
		if(real_eglGPA) {
			printf("*********Get real eglGetProcAddress in eglGetProcAddress*********\n");
			return real_eglGPA(fname);
		}
		else
			return NULL;
	}
}
