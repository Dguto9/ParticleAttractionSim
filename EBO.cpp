#include "EBO.h"

#include <GLAD/glad.h>

EBO::EBO() {
    glGenBuffers(1, &ID);
}

int EBO::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    return 1;
}

int EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return 1;
}

int EBO::Data(unsigned int* indices, unsigned int size) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    return 1;
}

EBO::~EBO() {
    glDeleteBuffers(1, &ID);
}