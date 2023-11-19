#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "VAO.h"
#include "VBO.h"

#define M_PI 3.1415926535

const char* vertexShaderSource = R"glsl(
#version 330 core

in vec3 aPos;


uniform mat4 transformMat;

void main(){
    gl_Position = transformMat * vec4(aPos, 1.0);
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core

out vec4 color;

uniform vec3 inColor;

void main(){
    color = vec4(inColor, 1.0);
}
)glsl";

const int electronCount = 10;  
float electrons[electronCount*6];
const int protonCount = 10;
float protons[protonCount * 6];

float distanceSquared(float x1, float y1, float z1, float x2, float y2, float z2);
float vectorNormalize(float x, float y, float z);

int main(void)
{
    srand(time(0));

    for (int i = 0; i < electronCount * 6; i+=6) {
        for (int j = 0; j < 3; j++) {
            electrons[i+j] = ((float)rand() / RAND_MAX) - 0.5;
        }
    }
    for (int i = 0; i < protonCount * 6; i+=6) {
        for (int j = 0; j < 3; j++) {
            protons[i+j] = ((float)rand() / RAND_MAX) - 0.5;
        }
    }

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "iminyourwalls", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    VAO vertArrayBuffer;
    vertArrayBuffer.Bind();

    VBO vertBuffer;
    vertBuffer.Bind();
    vertBuffer.Data(electrons, sizeof(electrons));

    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderID);

    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderID);

    unsigned int shaderProgramID = glCreateProgram();     
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);
    glUseProgram(shaderProgramID);

    int positionAttribute = glGetAttribLocation(shaderProgramID, "aPos");
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(positionAttribute);

    int frag_inColor = glGetUniformLocation(shaderProgramID, "inColor");
    int vert_transformMat = glGetUniformLocation(shaderProgramID, "transformMat");

    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::mat4 viewMat = glm::mat4(1.0f);
    glm::mat4 projectionMat = glm::mat4(1.0f);

    modelMat = glm::translate(modelMat, glm::vec3(0, 0, -5));
    modelMat = glm::rotate(modelMat, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projectionMat = glm::perspective(glm::radians(45.0f), 640.0f/480.0f, 0.1f, 1000.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float dt = 0.0001;
        for (int i = 0; i < electronCount*6; i += 6) {
            for (int j = 0; j < protonCount * 6; j += 6) {
                float dist = distanceSquared(electrons[i], electrons[i + 1], electrons[i + 2], protons[j], protons[j+1], protons[j+2]);
                float normalize = vectorNormalize(protons[j] - electrons[i], protons[j+1] - electrons[i + 1], protons[i+2] - electrons[i + 2]);
                electrons[i + 3] += ((protons[j] - electrons[i]) / normalize) * (dt / dist);
                electrons[i + 4] += ((protons[j+1] - electrons[i + 1]) / normalize) * (dt / dist);
                electrons[i + 5] += ((protons[j+2] - electrons[i + 2]) / normalize) * (dt / dist);
            }
        }
        for (int i = 0; i < protonCount * 6; i += 6) {
            for (int j = 0; j < electronCount * 6; j += 6) {
                float dist = distanceSquared(protons[i], protons[i + 1], protons[i + 2], electrons[j], electrons[j + 1], electrons[j + 2]);
                float normalize = vectorNormalize(electrons[j] - protons[i], electrons[j + 1] - protons[i + 1], electrons[i + 2] - protons[i + 2]);
                protons[i + 3] += ((electrons[j] - protons[i]) / normalize) * (dt / dist);
                protons[i + 4] += ((electrons[j + 1] - protons[i + 1]) / normalize) * (dt / dist);
                protons[i + 5] += ((electrons[j + 2] - protons[i + 2]) / normalize) * (dt / dist);
            }
        }
        for (int i = 0; i < electronCount * 6; i+=6) {
            electrons[i] += electrons[i + 3]*dt;
            electrons[i + 1] += electrons[i + 4]*dt;
            electrons[i + 2] += electrons[i + 5]*dt;
        }
        for (int i = 0; i < protonCount * 6; i += 6) {
            protons[i] += protons[i + 3] * dt;
            protons[i + 1] += protons[i + 4] * dt;
            protons[i + 2] += protons[i + 5] * dt;
        }

        glm::mat4 mvp = projectionMat * viewMat * modelMat;
        glUniformMatrix4fv(vert_transformMat, 1, GL_FALSE, glm::value_ptr(mvp));
        //glUniform3f(frag_inColor, abs(sin(glfwGetTime())), abs(sin(glfwGetTime() + (2 * M_PI / 3))), abs(sin(glfwGetTime() + (4 * M_PI / 3))));
        glUniform3f(frag_inColor, 0.0, 0.5, 1.0);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        vertBuffer.Data(electrons, sizeof(electrons));
        glDrawArrays(GL_POINTS, 0, electronCount);
        glUniform3f(frag_inColor, 1.0, 0.0, 0.0);
        vertBuffer.Data(protons, sizeof(protons));
        glDrawArrays(GL_POINTS, 0, protonCount);
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

float distanceSquared(float x1, float y1, float z1, float x2, float y2, float z2) {
    return pow((x2 - x1), 2) + pow((y2 - y1), 2) + pow((z2 - z1), 2);
}

float vectorNormalize(float x, float y, float z) {
    return sqrt((x * x) + (y * y) + (z * z));
}