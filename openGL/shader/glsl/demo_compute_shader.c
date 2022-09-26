/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : demo_compute_shader.c
 *  Author       : wqshao
 *  Created Time : 2022-09-21 14:37:18
 *  Description  : 传输一个一维数组，然后给数组里面每个元素加1
 *		sudo apt install libglew-dev freeglut3-dev
 *		g++ demo_compute_shader.c `pkg-config --libs glew` -lglut
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

void InitWindow(int, char*[]);
void Initialize(int, char*[]);
void CreateShaders(void);
void CreateTexture(void);

GLuint programId, computeShaderId;
GLuint inputTexId, outputTexId;

const int kArraySize = 32;

const GLchar* Program = " \
    #version 430\n\
    layout (local_size_x = 16, local_size_y = 1) in;\n\
    layout (r32f, binding = 0) uniform image1D in_array; \n\
    layout (r32f, binding = 1) uniform image1D out_array; \n\
    \
    void main() \n\
    { \
        int pos = int(gl_GlobalInvocationID.x);\n\
        vec4 value = imageLoad(in_array, pos);\n\
        value.x += 1.0f;\n\
        imageStore(out_array, pos, value);\n\
    } \
";

void CheckGLErrors()
{
    GLenum e = glGetError();
    if (e != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL error: %s (%d)\n", gluErrorString(e), e);
        exit(20);
    }
}

// 创建opengl的窗口
void InitWindow(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitContextVersion(2, 0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    int WindowHandle = glutCreateWindow("test");

    if (WindowHandle < 1) {
        fprintf(
            stderr,
            "ERROR: Could not create a new rendering window.\n"
        );
        exit(EXIT_FAILURE);
    }

    //glutDisplayFunc(RenderFunction);
    //glutCloseFunc(Cleanup);
}

void Initialize(int argc, char* argv[])
{
    GLenum GlewInitResult;

    InitWindow(argc, argv);

    //glewExperimental = GL_TRUE;
    GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult) {
        fprintf(
            stderr,
            "ERROR: %s\n",
            glewGetErrorString(GlewInitResult)
        );
        exit(EXIT_FAILURE);
    }

    fprintf(
        stdout,
        "INFO: OpenGL Version: %s\n",
        glGetString(GL_VERSION)
    );

    CreateShaders();
    CreateTexture();
}

void CreateShaders(void)
{
    GLchar messages[256];
    GLenum ErrorCheckValue = glGetError();

    /* Compile the shader. */
    computeShaderId = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShaderId, 1, &Program, NULL);
    glCompileShader(computeShaderId);

    /* Print the compilation log. */
    glGetShaderInfoLog(computeShaderId, sizeof(messages), NULL, messages);
    printf("Compile Log: %s\n", messages);

    /* Set up program objects. */
    programId = glCreateProgram();

    /* Create a complete program object. */
    glAttachShader(programId, computeShaderId);
    glLinkProgram(programId);

    /* And print the link log. */
    glGetProgramInfoLog(programId, sizeof(messages), NULL, messages);
    printf("Link Log: %s\n", messages);

    CheckGLErrors();
}

void CreateTexture(void)
{
    // Create the input texture
    glGenTextures(1, &inputTexId);

    // And bind it to texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, inputTexId);
    // Set texture size and format
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, kArraySize);

    // Create the output texture
    glGenTextures(1, &outputTexId);

    // And bind it to texture unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, outputTexId);
    // Set texture size and format
    glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, kArraySize);

    glBindImageTexture(0, inputTexId, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
    glBindImageTexture(1, outputTexId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

    CheckGLErrors();
}

void DoCompute()
{
    float *inputData = new float[kArraySize];
    float *outputData = new float[kArraySize];

    int i;
    for (i = 0; i < kArraySize; i++)
    {
        inputData[i] = i;
    }

    glBindTexture(GL_TEXTURE_1D, inputTexId);
    glTexSubImage1D(GL_TEXTURE_1D, 0, 0, kArraySize, GL_RED, GL_FLOAT, inputData);


    { // launch compute shaders!
        glUseProgram(programId);
        glDispatchCompute((GLuint)kArraySize/16, 1, 1);
    }

    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glBindTexture(GL_TEXTURE_1D, outputTexId);
    glGetTexImage(GL_TEXTURE_1D, 0, GL_RED, GL_FLOAT, outputData);
    glBindTexture(GL_TEXTURE_2D, 0);

    CheckGLErrors();

    for (i = 0; i < kArraySize; i++)
    {
        printf("%f ", outputData[i]);
    }
	printf("\n");

    delete []outputData;
    delete []inputData;
}

int main(int argc, char *argv[])
{
    Initialize(argc, argv);

    DoCompute();
    //glutMainLoop();

    return(0);
}
