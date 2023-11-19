#include "VBO.h"

#include <GLAD/glad.h>

VBO::VBO() {
    glGenBuffers(1, &ID);
}

int VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    return 1;
}

int VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return 1;
}

int VBO::Data(float *vertices, unsigned int size) {
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return 1;
}

VBO::~VBO() {
    glDeleteBuffers(1, &ID);
}