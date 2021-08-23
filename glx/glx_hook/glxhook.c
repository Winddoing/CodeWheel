/*
 *     gcc glhook_v46.c glxhook.c -o glhook.so -fPIC -shared -ldl -D_GNU_SOURCE
 *     export LD_PRELOAD=/home/glx_hook/glhook.so
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <EGL/egl.h>
#include <pthread.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "hook_macro.h"

#if 1
/*
 *  GLX API
 */
XVisualInfo* glXChooseVisual( Display *dpy, int screen, int *attribList );
GLXContext glXCreateContext( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct );
void glXDestroyContext( Display *dpy, GLXContext ctx );
Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx);
void glXCopyContext( Display *dpy, GLXContext src, GLXContext dst, unsigned long mask );
void glXSwapBuffers( Display *dpy, GLXDrawable drawable );
GLXPixmap glXCreateGLXPixmap( Display *dpy, XVisualInfo *visual, Pixmap pixmap );
void glXDestroyGLXPixmap( Display *dpy, GLXPixmap pixmap );
Bool glXQueryExtension( Display *dpy, int *errorb, int *event );
Bool glXQueryVersion( Display *dpy, int *maj, int *min );
Bool glXIsDirect( Display *dpy, GLXContext ctx );
int glXGetConfig( Display *dpy, XVisualInfo *visual, int attrib, int *value );
GLXContext glXGetCurrentContext( void );
GLXDrawable glXGetCurrentDrawable( void );
void glXWaitGL( void );
void glXWaitX( void );
void glXUseXFont( Font font, int first, int count, int list );

/* GLX 1.1 and later */
const char *glXQueryExtensionsString( Display *dpy, int screen );
const char *glXQueryServerString( Display *dpy, int screen, int name );
const char *glXGetClientString( Display *dpy, int name );

/* GLX 1.2 and later */
Display *glXGetCurrentDisplay( void );

/* GLX 1.3 and later */
GLXFBConfig *glXChooseFBConfig( Display *dpy, int screen, const int *attribList, int *nitems );
int glXGetFBConfigAttrib( Display *dpy, GLXFBConfig config, int attribute, int *value );
GLXFBConfig *glXGetFBConfigs( Display *dpy, int screen, int *nelements );
XVisualInfo *glXGetVisualFromFBConfig( Display *dpy, GLXFBConfig config );
GLXWindow glXCreateWindow( Display *dpy, GLXFBConfig config, Window win, const int *attribList );
void glXDestroyWindow( Display *dpy, GLXWindow window );
GLXPixmap glXCreatePixmap( Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList );
void glXDestroyPixmap( Display *dpy, GLXPixmap pixmap );
GLXPbuffer glXCreatePbuffer( Display *dpy, GLXFBConfig config, const int *attribList );
void glXDestroyPbuffer( Display *dpy, GLXPbuffer pbuf );
void glXQueryDrawable( Display *dpy, GLXDrawable draw, int attribute, unsigned int *value );
GLXContext glXCreateNewContext( Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct );
Bool glXMakeContextCurrent( Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx );
GLXDrawable glXGetCurrentReadDrawable( void );
int glXQueryContext( Display *dpy, GLXContext ctx, int attribute, int *value );
void glXSelectEvent( Display *dpy, GLXDrawable drawable, unsigned long mask );
void glXGetSelectedEvent( Display *dpy, GLXDrawable drawable, unsigned long *mask );

/*
 * ARB 2. GLX_ARB_get_proc_address
 */
__GLXextFuncPtr glXGetProcAddressARB (const GLubyte *);

/*
 *  Other GLX API
 */
GLXContext glXCreateContextAttribsARB(Display *dpy, GLXFBConfig config, GLXContext shareCtx, Bool direct, const int *attrib_list);
#endif

XVisualInfo* glXChooseVisual( Display *dpy, int screen, int *attribList )
{
    printf("call glXChooseVisual\n");
    XVisualInfo* (*real_glXChooseVisual)( Display *dpy, int screen, int *attribList );
    real_glXChooseVisual = dlwrap_real_dlsym(RTLD_NEXT, "glXChooseVisual");
    return real_glXChooseVisual(dpy, screen, attribList);
}

GLXContext glXCreateContext( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct )
{
    GLXContext ctx;
    GLXContext (*real_glXCreateContext)( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct );
    real_glXCreateContext = dlwrap_real_dlsym(RTLD_NEXT, "glXCreateContext");
    ctx = real_glXCreateContext(dpy, vis, shareList, direct);
    printf("call glXCreateContext(shareList=%p) = %p\n", shareList, ctx);
    return ctx;
}

void glXDestroyContext( Display *dpy, GLXContext ctx )
{
    printf("call glXDestroyContext(%p)\n", ctx);
    void (*real_glXDestroyContext)( Display *dpy, GLXContext ctx );
    real_glXDestroyContext = dlwrap_real_dlsym(RTLD_NEXT, "glXDestroyContext");
    real_glXDestroyContext(dpy, ctx);
}

Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx)
{
    printf("call glXMakeCurrent(drawable=%ld, ctx=%p)\n", drawable, ctx);
    Bool (*real_glXMakeCurrent)( Display *dpy, GLXDrawable drawable, GLXContext ctx);
    real_glXMakeCurrent = dlwrap_real_dlsym(RTLD_NEXT, "glXMakeCurrent");
    return real_glXMakeCurrent(dpy, drawable, ctx);
}

void glXCopyContext( Display *dpy, GLXContext src, GLXContext dst, unsigned long mask )
{
    printf("call glXCopyContext(src=%p, dst=%p)\n", src, dst);
    void (*real_glXCopyContext)( Display *dpy, GLXContext src, GLXContext dst, unsigned long mask );
    real_glXCopyContext = dlwrap_real_dlsym(RTLD_NEXT, "glXCopyContext");
    real_glXCopyContext(dpy, src, dst, mask);
}

void glXSwapBuffers( Display *dpy, GLXDrawable drawable )
{
    printf("call glXSwapBuffers(drawable=%ld)\n", drawable);
    void (*real_glXSwapBuffers)( Display *dpy, GLXDrawable drawable );
    real_glXSwapBuffers = dlwrap_real_dlsym(RTLD_NEXT, "glXSwapBuffers");
    real_glXSwapBuffers(dpy, drawable);
}

GLXPixmap glXCreateGLXPixmap( Display *dpy, XVisualInfo *visual, Pixmap pixmap )
{
    printf("call glXCreateGLXPixmap\n");
    GLXPixmap (*real_glXCreateGLXPixmap)( Display *dpy, XVisualInfo *visual, Pixmap pixmap );
    real_glXCreateGLXPixmap = dlwrap_real_dlsym(RTLD_NEXT, "glXCreateGLXPixmap");
    return real_glXCreateGLXPixmap(dpy, visual, pixmap);
}

void glXDestroyGLXPixmap( Display *dpy, GLXPixmap pixmap )
{
    printf("call glXDestroyGLXPixmap\n");
    void (*real_glXDestroyGLXPixmap)( Display *dpy, GLXPixmap pixmap );
    real_glXDestroyGLXPixmap = dlwrap_real_dlsym(RTLD_NEXT, "glXDestroyGLXPixmap");
    real_glXDestroyGLXPixmap(dpy, pixmap);
}

Bool glXQueryExtension( Display *dpy, int *errorb, int *event )
{
    printf("call glXQueryExtension\n");
    Bool (*real_glXQueryExtension)( Display *dpy, int *errorb, int *event );
    real_glXQueryExtension = dlwrap_real_dlsym(RTLD_NEXT, "glXQueryExtension");
    Bool ret = real_glXQueryExtension(dpy, errorb, event);
    return ret;
}

Bool glXQueryVersion( Display *dpy, int *maj, int *min )
{
    printf("call glXQueryVersion\n");
    Bool (*real_glXQueryVersion)( Display *dpy, int *maj, int *min );
    real_glXQueryVersion = dlwrap_real_dlsym(RTLD_NEXT, "glXQueryVersion");
    return real_glXQueryVersion(dpy, maj, min);
}

Bool glXIsDirect( Display *dpy, GLXContext ctx )
{
    printf("call glXIsDirect(ctx=%p)\n", ctx);
    Bool (*real_glXIsDirect)( Display *dpy, GLXContext ctx );
    real_glXIsDirect = dlwrap_real_dlsym(RTLD_NEXT, "glXIsDirect");
    return real_glXIsDirect(dpy, ctx);
}

int glXGetConfig( Display *dpy, XVisualInfo *visual, int attrib, int *value )
{
    printf("call glXGetConfig\n");
    int (*real_glXGetConfig)( Display *dpy, XVisualInfo *visual, int attrib, int *value );
    real_glXGetConfig = dlwrap_real_dlsym(RTLD_NEXT, "glXGetConfig");
    return real_glXGetConfig( dpy, visual, attrib, value );
}

GLXContext glXGetCurrentContext( void )
{
    printf("call glXGetCurrentContext\n");
    GLXContext (*real_glXGetCurrentContext)( void );
    real_glXGetCurrentContext = dlwrap_real_dlsym(RTLD_NEXT, "glXGetCurrentContext");
    GLXContext ctx = real_glXGetCurrentContext();
    printf("call glXGetCurrentContext() = %p\n", ctx);
    return ctx;
}

GLXDrawable glXGetCurrentDrawable( void )
{
    printf("call glXGetCurrentDrawable\n");
    GLXDrawable (*real_glXGetCurrentDrawable)( void );
    real_glXGetCurrentDrawable = dlwrap_real_dlsym(RTLD_NEXT, "glXGetCurrentDrawable");
    GLXDrawable drawable = real_glXGetCurrentDrawable();
    printf("call glXGetCurrentDrawable() = %ld\n", drawable);
    return drawable;
}

void glXWaitGL( void )
{
    printf("call glXWaitGL\n");
    void (*real_glXWaitGL)( void );
    real_glXWaitGL = dlwrap_real_dlsym(RTLD_NEXT, "glXWaitGL");
    real_glXWaitGL(  );
}

void glXWaitX( void )
{
    printf("call glXWaitX\n");
    void (*real_glXWaitX)( void );
    real_glXWaitX = dlwrap_real_dlsym(RTLD_NEXT, "glXWaitX");
    real_glXWaitX(  );
}

void glXUseXFont( Font font, int first, int count, int list )
{
    printf("call glXUseXFont\n");
    void (*real_glXUseXFont)( Font font, int first, int count, int list );
    real_glXUseXFont = dlwrap_real_dlsym(RTLD_NEXT, "glXUseXFont");
    real_glXUseXFont( font, first, count, list );
}

/* GLX 1.1 and later */
const char * glXQueryExtensionsString( Display *dpy, int screen )
{
    printf("call glXQueryExtensionsString\n");
    const char * (*real_glXQueryExtensionsString)( Display *dpy, int screen );
    real_glXQueryExtensionsString = dlwrap_real_dlsym(RTLD_NEXT, "glXQueryExtensionsString");
    return real_glXQueryExtensionsString( dpy, screen );
}

const char * glXQueryServerString( Display *dpy, int screen, int name )
{
    const char * (*real_glXQueryServerString)( Display *dpy, int screen, int name );
    real_glXQueryServerString = dlwrap_real_dlsym(RTLD_NEXT, "glXQueryServerString");
    const char *result = real_glXQueryServerString( dpy, screen, name );
    printf("call glXQueryServerString() = %s\n", result);
    return result;
}

const char * glXGetClientString( Display *dpy, int name )
{
    const char * (*real_glXGetClientString)( Display *dpy, int name );
    real_glXGetClientString = dlwrap_real_dlsym(RTLD_NEXT, "glXGetClientString");
    const char *ret = real_glXGetClientString( dpy, name );
    printf("call glXGetClientString(%d) = %s\n", name, ret);
    return ret;
}

/* GLX 1.2 and later */
Display * glXGetCurrentDisplay( void )
{
    printf("call glXGetCurrentDisplay\n");
    Display * (*real_glXGetCurrentDisplay)( void );
    real_glXGetCurrentDisplay = dlwrap_real_dlsym(RTLD_NEXT, "glXGetCurrentDisplay");
    return real_glXGetCurrentDisplay(  );
}

/* GLX 1.3 and later */
GLXFBConfig * glXChooseFBConfig( Display *dpy, int screen, const int *attribList, int *nitems )
{
    printf("call glXChooseFBConfig\n");
    GLXFBConfig * (*real_glXChooseFBConfig)( Display *dpy, int screen, const int *attribList, int *nitems );
    real_glXChooseFBConfig = dlwrap_real_dlsym(RTLD_NEXT, "glXChooseFBConfig");
    return real_glXChooseFBConfig( dpy, screen, attribList, nitems );
}

int glXGetFBConfigAttrib( Display *dpy, GLXFBConfig config, int attribute, int *value )
{
    printf("call glXGetFBConfigAttrib\n");
    int (*real_glXGetFBConfigAttrib)( Display *dpy, GLXFBConfig config, int attribute, int *value );
    real_glXGetFBConfigAttrib = dlwrap_real_dlsym(RTLD_NEXT, "glXGetFBConfigAttrib");
    return real_glXGetFBConfigAttrib( dpy, config, attribute, value );
}

GLXFBConfig * glXGetFBConfigs( Display *dpy, int screen, int *nelements )
{
    printf("call glXGetFBConfigs\n");
    GLXFBConfig * (*real_glXGetFBConfigs)( Display *dpy, int screen, int *nelements );
    real_glXGetFBConfigs = dlwrap_real_dlsym(RTLD_NEXT, "glXGetFBConfigs");
    return real_glXGetFBConfigs( dpy, screen, nelements );
}

XVisualInfo * glXGetVisualFromFBConfig( Display *dpy, GLXFBConfig config )
{
    printf("call glXGetVisualFromFBConfig\n");
    XVisualInfo * (*real_glXGetVisualFromFBConfig)( Display *dpy, GLXFBConfig config );
    real_glXGetVisualFromFBConfig = dlwrap_real_dlsym(RTLD_NEXT, "glXGetVisualFromFBConfig");
    return real_glXGetVisualFromFBConfig( dpy, config );
}

GLXWindow glXCreateWindow( Display *dpy, GLXFBConfig config, Window win, const int *attribList )
{
    printf("call glXCreateWindow\n");
    GLXWindow (*real_glXCreateWindow)( Display *dpy, GLXFBConfig config, Window win, const int *attribList );
    real_glXCreateWindow = dlwrap_real_dlsym(RTLD_NEXT, "glXCreateWindow");
    return real_glXCreateWindow( dpy, config, win, attribList );
}

void glXDestroyWindow( Display *dpy, GLXWindow window )
{
    printf("call glXDestroyWindow\n");
    void (*real_glXDestroyWindow)( Display *dpy, GLXWindow window );
    real_glXDestroyWindow = dlwrap_real_dlsym(RTLD_NEXT, "glXDestroyWindow");
    real_glXDestroyWindow( dpy, window );
}

GLXPixmap glXCreatePixmap( Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList )
{
    printf("call glXCreatePixmap\n");
    GLXPixmap (*real_glXCreatePixmap)( Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attribList );
    real_glXCreatePixmap = dlwrap_real_dlsym(RTLD_NEXT, "glXCreatePixmap");
    return real_glXCreatePixmap( dpy, config, pixmap, attribList );
}

void glXDestroyPixmap( Display *dpy, GLXPixmap pixmap )
{
    printf("call glXDestroyPixmap\n");
    void (*real_glXDestroyPixmap)( Display *dpy, GLXPixmap pixmap );
    real_glXDestroyPixmap = dlwrap_real_dlsym(RTLD_NEXT, "glXDestroyPixmap");
    real_glXDestroyPixmap( dpy, pixmap );
}

GLXPbuffer glXCreatePbuffer( Display *dpy, GLXFBConfig config, const int *attribList )
{
    printf("call glXCreatePbuffer\n");
    GLXPbuffer (*real_glXCreatePbuffer)( Display *dpy, GLXFBConfig config, const int *attribList );
    real_glXCreatePbuffer = dlwrap_real_dlsym(RTLD_NEXT, "glXCreatePbuffer");
    return real_glXCreatePbuffer( dpy, config, attribList );
}

void glXDestroyPbuffer( Display *dpy, GLXPbuffer pbuf )
{
    printf("call glXDestroyPbuffer\n");
    void (*real_glXDestroyPbuffer)( Display *dpy, GLXPbuffer pbuf );
    real_glXDestroyPbuffer = dlwrap_real_dlsym(RTLD_NEXT, "glXDestroyPbuffer");
    real_glXDestroyPbuffer( dpy, pbuf );
}

void glXQueryDrawable( Display *dpy, GLXDrawable draw, int attribute, unsigned int *value )
{
    printf("call glXQueryDrawable\n");
    void (*real_glXQueryDrawable)( Display *dpy, GLXDrawable draw, int attribute, unsigned int *value );
    real_glXQueryDrawable = dlwrap_real_dlsym(RTLD_NEXT, "glXQueryDrawable");
    real_glXQueryDrawable( dpy, draw, attribute, value );
}

GLXContext glXCreateNewContext( Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct )
{
    printf("call glXCreateNewContext(shareList=%p)\n", shareList);
    GLXContext (*real_glXCreateNewContext)( Display *dpy, GLXFBConfig config, int renderType, GLXContext shareList, Bool direct );
    real_glXCreateNewContext = dlwrap_real_dlsym(RTLD_NEXT, "glXCreateNewContext");
    return real_glXCreateNewContext( dpy, config, renderType, shareList, direct );
}

Bool glXMakeContextCurrent( Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx )
{
    printf("call glXMakeContextCurrent(draw=%ld, read=%ld, ctx=%p)\n", draw, read, ctx);
    Bool (*real_glXMakeContextCurrent)( Display *dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx );
    real_glXMakeContextCurrent = dlwrap_real_dlsym(RTLD_NEXT, "glXMakeContextCurrent");
    return real_glXMakeContextCurrent(dpy, draw, read, ctx);
}

GLXDrawable glXGetCurrentReadDrawable( void )
{
    GLXDrawable (*real_glXGetCurrentReadDrawable)( void );
    real_glXGetCurrentReadDrawable = dlwrap_real_dlsym(RTLD_NEXT, "glXGetCurrentReadDrawable");
    GLXDrawable drawable = real_glXGetCurrentReadDrawable();
    printf("call glXGetCurrentReadDrawable() = %ld\n", drawable);
    return drawable;
}

int glXQueryContext( Display *dpy, GLXContext ctx, int attribute, int *value )
{
    printf("call glXQueryContext(ctx=%p)\n", ctx);
    int (*real_glXQueryContext)( Display *dpy, GLXContext ctx, int attribute, int *value );
    real_glXQueryContext = dlwrap_real_dlsym(RTLD_NEXT, "glXQueryContext");
    return real_glXQueryContext(dpy, ctx, attribute, value);
}

void glXSelectEvent( Display *dpy, GLXDrawable drawable, unsigned long mask )
{
    printf("call glXSelectEvent\n");
    void (*real_glXSelectEvent)( Display *dpy, GLXDrawable drawable, unsigned long mask );
    real_glXSelectEvent = dlwrap_real_dlsym(RTLD_NEXT, "glXSelectEvent");
    real_glXSelectEvent( dpy, drawable, mask );
}

void glXGetSelectedEvent( Display *dpy, GLXDrawable drawable, unsigned long *mask )
{
    printf("call glXGetSelectedEvent\n");
    void (*real_glXGetSelectedEvent)( Display *dpy, GLXDrawable drawable, unsigned long *mask );
    real_glXGetSelectedEvent = dlwrap_real_dlsym(RTLD_NEXT, "glXGetSelectedEvent");
    real_glXGetSelectedEvent( dpy, drawable, mask );
}

GLXContext glXCreateContextAttribsARB(Display *dpy, GLXFBConfig config, GLXContext shareCtx, Bool direct, const int *attrib_list)
{
    GLXContext (*real_glXCreateContextAttribsARB)(Display *dpy, GLXFBConfig config, GLXContext shareCtx, Bool direct, const int *attrib_list);
    real_glXCreateContextAttribsARB = dlwrap_real_dlsym(RTLD_NEXT, "glXCreateContextAttribsARB");
    GLXContext ctx = real_glXCreateContextAttribsARB( dpy, config, shareCtx, direct, attrib_list );
    printf("call glXCreateContextAttribsARB(shareCtx=%p) = %p\n", shareCtx, ctx);
    return ctx;
}

glxGPA_t real_glxGPA_ARB = NULL;
glxGPA_t real_glxGPA = NULL;
__GLXextFuncPtr glXGetProcAddressARB (const GLubyte *fname)
{
    printf("call glXGetProcAddressARB(%s)\n", fname);
    
    /* For Ubuntu VM running on x86 server */
    if(fname && fname[0] == 'g' && fname[1] == 'l') {
        if(fname[2] == 'X') {
            GLX_API_HOOKS(fname);
        } else {
            GLX_GL_HOOKS(fname);
        }
    }
    
    printf("*********glXGetProcAddressARB: search (%s)*********\n", fname);
    
	if(real_glxGPA_ARB) {
		return real_glxGPA_ARB(fname);
	} else {
		real_glxGPA_ARB = dlwrap_real_dlsym(RTLD_NEXT, "glXGetProcAddressARB");
		if(real_glxGPA_ARB) {
			printf("*********Get real glXGetProcAddressARB in glXGetProcAddressARB*********\n");
			return real_glxGPA_ARB(fname);
		}
		else
			return NULL;
	}
}

__GLXextFuncPtr glXGetProcAddress (const GLubyte *fname)
{
    printf("call glXGetProcAddress(%s)\n", fname);
    
    /* For Ubuntu VM running on x86 server */
    if(fname && fname[0] == 'g' && fname[1] == 'l') {
        if(fname[2] == 'X') {
            GLX_API_HOOKS(fname);
        } else {
            GLX_GL_HOOKS(fname);
        }
    }
    
    printf("*********glXGetProcAddress: search (%s)*********\n", fname);
    
	if(real_glxGPA) {
		return real_glxGPA(fname);
	} else {
		real_glxGPA = dlwrap_real_dlsym(RTLD_NEXT, "glXGetProcAddress");
		if(real_glxGPA) {
			printf("*********Get real glXGetProcAddress in glXGetProcAddress*********\n");
			return real_glxGPA(fname);
		}
		else
			return NULL;
	}
}

#ifdef __cplusplus
}
#endif
