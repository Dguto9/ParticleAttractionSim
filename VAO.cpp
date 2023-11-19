#include "VAO.h"
#include <GLAD/glad.h>

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

int VAO::Bind() {
	glBindVertexArray(ID);
	return 1;
}

int VAO::Unbind() {
	glBindVertexArray(0);
	return 1;
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &ID);
}
