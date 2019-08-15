/*#############################################################
 *     File Name	: get_version.c
 *     Author		: winddoing
 *     Created Time	: 2019年08月14日 星期三 18时09分35秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glut.h>

int main(int argc, char *argv[])
{
    char name[]="mainWindow";

    glutInit (&argc, argv);
    glutCreateWindow(name);

    const char* version = (const char*)glGetString(GL_VERSION);
    printf("OpenGL version: %s\n", version);

    return 0;
}
