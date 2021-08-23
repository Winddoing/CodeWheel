#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hook_macro.h"

void *dlopen(const char *filename, int flags)
{
    void * (*real_dlopen)(const char *filename, int flags);
    real_dlopen = dlsym(RTLD_NEXT, "dlopen");
    void *handle = real_dlopen(filename, flags);
    printf("*******call dlopen(%s, %d), handle: %p******\n", filename, flags, handle);
    return handle;
}

int dlclose(void *handle)
{
	 printf("*******call dlclose(%p)*******\n", handle);
	 int (*real_dlclose)(void *handle);
     real_dlclose = dlsym(RTLD_NEXT, "dlclose");
     return (*real_dlclose)(handle);
}

typedef void* (*fips_dlsym_t)(void *handle, const char *symbol);

void* dlwrap_real_dlsym(void *handle, const char *name)
{
    static fips_dlsym_t real_dlsym = NULL;

    if (!real_dlsym) {
        /* FIXME: This brute-force, hard-coded searching for a versioned
         * symbol is really ugly. The only reason I'm doing this is because
         * I need some way to lookup the "dlsym" function in libdl, but
         * I can't use 'dlsym' to do it. So dlvsym works, but forces me
         * to guess what the right version is.
         *
         * Potential fixes here:
         *
         *   1. Use libelf to actually inspect libdl.so and
         *      find the right version, (finding the right
         *      libdl.so can be made easier with
         *      dl_iterate_phdr).
         *
         *   2. Use libelf to find the offset of the 'dlsym'
         *      symbol within libdl.so, (and then add this to
         *      the base address at which libdl.so is loaded
         *      as reported by dl_iterate_phdr).
         *
         * In the meantime, I'll just keep augmenting this
         * hard-coded version list as people report bugs. */
        const char *version[] = {
            "GLIBC_2.17",
            "GLIBC_2.4",
            "GLIBC_2.3",
            "GLIBC_2.2.5",
            "GLIBC_2.2",
            "GLIBC_2.0"
        };
        int num_versions = sizeof(version) / sizeof(version[0]);
        int i;
        for (i = 0; i < num_versions; i++) {
            real_dlsym = (fips_dlsym_t) dlvsym(RTLD_NEXT, "dlsym", version[i]);
            if (real_dlsym)
                break;
        }
        if (i == num_versions) {
            fprintf(stderr, "Internal error: Failed to find real dlsym\n");
            fprintf(stderr,
                    "This may be a simple matter of fips not knowing about the version of GLIBC that\n"
                    "your program is using. Current known versions are:\n\n\t");
            for (i = 0; i < num_versions; i++)
                fprintf(stderr, "%s ", version[i]);
            fprintf(stderr,
                    "\n\nYou can inspect your version by first finding libdl.so.2:\n"
                    "\n"
                    "\tldd <your-program> | grep libdl.so\n"
                    "\n"
                    "And then inspecting the version attached to the dlsym symbol:\n"
                    "\n"
                    "\treadelf -s /path/to/libdl.so.2 | grep dlsym\n"
                    "\n"
                    "And finally, adding the version to dlwrap.c:dlwrap_real_dlsym.\n");

            exit(1);
        }
    }

    return real_dlsym(handle, name);
}

extern eglGPA_t real_eglGPA;
extern glxGPA_t real_glxGPA;
extern glxGPA_t real_glxGPA_ARB;
void* dlsym(void *handle, const char *name)
{
    printf("dlsym(%s)\n", name);
    
    /*
     *  EGL
     */ 
    if(name && name[0] == 'e' && name[1] == 'g' && name[2] == 'l') {
        EGL_API_HOOKS(name);

        if(strcmp(name, "eglGetProcAddress") == 0) {
			if(real_eglGPA)
				return eglGetProcAddress;

            real_eglGPA = dlwrap_real_dlsym(handle, name);
			if(real_eglGPA) {
				printf("*********Get real eglGetProcAddress in dlsym*********\n");
				return eglGetProcAddress;
			}
            else
				return NULL; 
        }
    }
    
    //#ifdef SUPPORT_EGL
#if 0
    if(name && name[0] == 'g' && name[1] == 'l') {
        EGL_GL_HOOKS(name);
    }
#endif        
	
    /*
     *  GLX or GL
     */
    if(name && name[0] == 'g' && name[1] == 'l') {
        if(name[2] == 'X') {
            if( strcmp(name, "glXGetProcAddressARB") == 0 ) {
                if(real_glxGPA_ARB)
                    return glXGetProcAddressARB;
                
                real_glxGPA_ARB = dlwrap_real_dlsym(handle, name);
                if(real_glxGPA_ARB) {
                    printf("*********Get real glXGetProcAddressARB in dlsym*********\n");
                    return glXGetProcAddressARB;
                }
                else
                    return NULL;
            }
            
            if( strcmp(name, "glXGetProcAddress") == 0 ) {
                if(real_glxGPA)
                    return glXGetProcAddress;
                
                real_glxGPA = dlwrap_real_dlsym(handle, name);
                if(real_glxGPA) {
                    printf("*********Get real glXGetProcAddress in dlsym*********\n");
                    return glXGetProcAddress;
                }
                else
                    return NULL;
            }
            
            GLX_API_HOOKS(name);
        } else {
            GLX_GL_HOOKS(name);
        }        
    }
    
    printf("*************dlsym: search(%s)*************\n", name); 
    return dlwrap_real_dlsym(handle, name);
}
