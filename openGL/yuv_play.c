#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <error.h>
#include <errno.h>

#include <GL/glut.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>


//Select one of the Texture mode (Set '1'):
#define TEXTURE_DEFAULT   0
//Rotate the texture
#define TEXTURE_ROTATE    0
//Show half of the Texture
#define TEXTURE_HALF      0


#define DISPLAY_SCREEN_W 500
#define DISPLAY_SCREEN_H 500


int pixel_w = 320, pixel_h = 180;
//YUV file
static FILE *infile = NULL;
//static unsigned char buf[pixel_w*pixel_h*3/2];
static unsigned char buf[320 * 180 * 3 / 2];
static unsigned char *plane[3];


GLuint p;
GLuint id_y, id_u, id_v; // Texture id
GLuint textureUniformY, textureUniformU,textureUniformV;


#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4

void display(void){
    if (fread(buf, 1, pixel_w*pixel_h*3/2, infile) != pixel_w*pixel_h*3/2){
        // Loop
        fseek(infile, 0, SEEK_SET);
        fread(buf, 1, pixel_w*pixel_h*3/2, infile);
    }
	//Clear
	glClearColor(0.0,255,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Y
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w, pixel_h, 0, GL_RED, GL_UNSIGNED_BYTE, plane[0]);
	glUniform1i(textureUniformY, 0);
	//U
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id_u);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w/2, pixel_h/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[1]);
    glUniform1i(textureUniformU, 1);
	//V
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, id_v);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w/2, pixel_h/2, 0, GL_RED, GL_UNSIGNED_BYTE, plane[2]);
    glUniform1i(textureUniformV, 2);

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// Show
	//Double
    glutSwapBuffers();
	//Single
	//glFlush();
}

void timeFunc(int value){
    display();
    // Timer: 40ms
    glutTimerFunc(40, timeFunc, 0);
}

static GLuint load_shader(GLenum type, const char *shader_str)
{
    GLuint shader;
    GLint compiled;
    GLint info_len = 0;
    char *info_log;

    assert(shader_str);
    /* Create the shader object */
    shader = glCreateShader(type);
    if (shader == 0){
        printf("glCreateShader failed.");
        return -EFAULT;
    }
    /* Load the shader source */
    glShaderSource(shader, 1, &shader_str, NULL);
    /* Compile the shader */
    glCompileShader(shader);
    /* Check the compile status */
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled){
        printf("compile opengl program failed.");
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
        if (info_len > 1) {
            info_log = (char *)malloc(sizeof(char)*info_len);
            glGetShaderInfoLog(shader,info_len,NULL,info_log);
            printf("Error compiling shader: %s", info_log);
            free(info_log);
        }
        glDeleteShader(shader);
        return -EFAULT;
    }
    return shader;
}

//Init Shader
void InitShaders()
{
    GLint vertCompiled, fragCompiled, linked;

    GLint v, f;

    char v_shader_str[] = \
        "attribute vec4 vertexIn;\n"
        "attribute vec2 textureIn;\n"
        "varying vec2 textureOut;\n"
        "void main(void)\n"
        "{\n"
        "   gl_Position = vertexIn;\n"
        "   textureOut = textureIn;\n"
        "}\n";

    char f_shader_str[] = \
        "varying vec2 textureOut;\n"
        "uniform sampler2D tex_y;\n"
        "uniform sampler2D tex_u;\n"
        "uniform sampler2D tex_v;\n"
        "void main(void)\n"
        "{\n"
        "    vec3 yuv;\n"
        "    vec3 rgb;\n"
        "    yuv.x = texture2D(tex_y, textureOut).r;\n"
        "    yuv.y = texture2D(tex_u, textureOut).r - 0.5;\n"
        "    yuv.z = texture2D(tex_v, textureOut).r - 0.5;\n"
        "    rgb = mat3(1, 1, 1, 0, -0.39465,\n"
        "            2.03211, 1.13983, -0.58060, 0) * yuv;\n"
        "    gl_FragColor = vec4(rgb, 1);\n"
        "}\n";

    v = load_shader(GL_VERTEX_SHADER, v_shader_str);
    f = load_shader(GL_FRAGMENT_SHADER, f_shader_str);

	//Debug
    glGetShaderiv(v, GL_COMPILE_STATUS, &vertCompiled);
    glCompileShader(f);
    glGetShaderiv(f, GL_COMPILE_STATUS, &fragCompiled);

	//Program: Step1
    p = glCreateProgram();
	//Program: Step2
    glAttachShader(p,v);
    glAttachShader(p,f);

    glBindAttribLocation(p, ATTRIB_VERTEX, "vertexIn");
    glBindAttribLocation(p, ATTRIB_TEXTURE, "textureIn");
	//Program: Step3
    glLinkProgram(p);
	//Debug
    glGetProgramiv(p, GL_LINK_STATUS, &linked);
	//Program: Step4
    glUseProgram(p);


	//Get Uniform Variables Location
	textureUniformY = glGetUniformLocation(p, "tex_y");
	textureUniformU = glGetUniformLocation(p, "tex_u");
	textureUniformV = glGetUniformLocation(p, "tex_v");

#if TEXTURE_ROTATE
    static const GLfloat vertexVertices[] = {
        -1.0f, -0.5f,
         0.5f, -1.0f,
        -0.5f,  1.0f,
         1.0f,  0.5f,
    };
#else
	static const GLfloat vertexVertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f,  1.0f,
		1.0f,  1.0f,
	};
#endif

#if TEXTURE_HALF
	static const GLfloat textureVertices[] = {
		0.0f,  1.0f,
		0.5f,  1.0f,
		0.0f,  0.0f,
		0.5f,  0.0f,
	};
#else
	static const GLfloat textureVertices[] = {
		0.0f,  1.0f,
		1.0f,  1.0f,
		0.0f,  0.0f,
		1.0f,  0.0f,
	};
#endif
	//Set Arrays
    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
	//Enable it
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);


	//Init Texture
    glGenTextures(1, &id_y);
    glBindTexture(GL_TEXTURE_2D, id_y);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &id_u);
    glBindTexture(GL_TEXTURE_2D, id_u);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &id_v);
    glBindTexture(GL_TEXTURE_2D, id_v);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

int main(int argc, char* argv[])
{
	if((infile=fopen("test_yuv420p_320x180.yuv", "rb"))==NULL){
		printf("cannot open this file\n");
		return -1;
	}

	//YUV Data
    plane[0] = buf;
    plane[1] = plane[0] + pixel_w*pixel_h;
    plane[2] = plane[1] + pixel_w*pixel_h/4;

    //Init GLUT
    glutInit(&argc, argv);

	//GLUT_DOUBLE
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA /*| GLUT_STENCIL | GLUT_DEPTH*/);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(DISPLAY_SCREEN_W, DISPLAY_SCREEN_H);
    glutCreateWindow("Video Play OpenGL (Texture)");

    glutDisplayFunc(&display);
    glutTimerFunc(40, timeFunc, 0);

    InitShaders();

    // Begin!
    glutMainLoop();

    return 0;
}






