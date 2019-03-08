/*#############################################################
 *     File Name    : demo1.c
 *     Author       : winddoing
 *     Created Time : 2019年03月07日 星期四 09时45分52秒
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <GL/glut.h>

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
bool bwinding = true;

void init()
{
    glClearColor(0, 0, 0, 0);
}

void reshape(int w, int h)
{
    if(h == 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(w <= h) {
        glOrtho(-100, 100, -100 * h / w, 100 * h / w, -100, 100);

    } else {
        glOrtho(-100, 100, -100 * w / h, 100 * w / h, -100, 100);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);

    glPointSize(5);
    glLineWidth(5);

    if(bwinding) {
        glFrontFace(GL_CW);

    } else {
        glFrontFace(GL_CCW);
    }

    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0);
    glVertex3f(0, 60, 0);
    glColor3f(1, 0, 0);
    glVertex3f(-60, -60, 0);
    glColor3f(0, 0, 1);
    glVertex3f(60, -60, 0);
    glEnd();
    glPopMatrix();
    glutSwapBuffers();
}

void specialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) xRot -= 5.0f;

    if(key == GLUT_KEY_DOWN) xRot += 5.0f;

    if(key == GLUT_KEY_LEFT) yRot -= 5.0f;

    if(key == GLUT_KEY_RIGHT) yRot += 5.0f;

    if(xRot > 356.0f) xRot = 0;

    if(yRot > 356.0f) yRot = 0;

    if(xRot < -1.0f) xRot = 355;

    if(yRot < -1.0f) yRot = 355;

    glutPostRedisplay();

}

void ProcessMenu(int value)
{
    switch(value) {
    case 1:
        glPolygonMode(GL_FRONT, GL_FILL);
        break;

    case 2:
        glPolygonMode(GL_FRONT, GL_LINE);
        break;

    case 3:
        glPolygonMode(GL_FRONT, GL_POINT);
        break;

    case 4:
        glPolygonMode(GL_BACK, GL_FILL);
        break;

    case 5:
        glPolygonMode(GL_BACK, GL_LINE);
        break;

    case 6:
        glPolygonMode(GL_BACK, GL_POINT);
        break;

    case 7:
        glShadeModel(GL_FLAT);
        break;

    case 8:
        glShadeModel(GL_SMOOTH);
        break;

    case 9:
        bwinding = !bwinding;
        break;

    default:
        break;
    }

    glutPostRedisplay();
}



int main(int argc, char** argv)
{
    int nModeMenu;
    int nMainMenu;
    int nColorMenu;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("多边形演示");
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutDisplayFunc(display);

    nModeMenu = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("正面多边形填充模式", 1);
    glutAddMenuEntry("正面线框模型", 2);
    glutAddMenuEntry("正面点模型", 3);
    glutAddMenuEntry("反面多边形填充模式", 4);
    glutAddMenuEntry("反面相框模型", 5);
    glutAddMenuEntry("反面点模型", 6);

    nColorMenu = glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("平面明暗模式", 7);
    glutAddMenuEntry("光滑敏感模式", 8);

    nMainMenu = glutCreateMenu(ProcessMenu);
    glutAddSubMenu("多边形模式", nModeMenu);
    glutAddSubMenu("颜色模式", nColorMenu);
    glutAddMenuEntry("改变绕法", 9);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
    init();
    glutMainLoop();
    return 0;
}
