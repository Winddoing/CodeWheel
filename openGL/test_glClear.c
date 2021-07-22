#include <stdio.h>
#include <gl/glut.h>

/*
 功能描述：使用OpenGL简单画一个矩形。
 */

//输出模式，0-单缓存模式；非0双缓存模式
#define OUTPUT_MODE 1

void display(void)
{
    //glClearColor函数设置好清除颜色，glClear利用glClearColor函数设置好的当前清除颜色设置窗口颜色
    glClearColor(1.0, 1.0, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);

    if (OUTPUT_MODE == 0) {
        glFlush();//单缓存GLUT_SINGLE时使用
    } else {
        glutSwapBuffers();//因为使用的是双缓存GLUT_DOUBLE，所以这里必须要交换缓存才会显示
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | (OUTPUT_MODE == 0 ? GLUT_SINGLE : GLUT_DOUBLE));
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("第一个 OpenGL 程序");
    glutDisplayFunc(&display);
    glutMainLoop();
    return 0;
}
