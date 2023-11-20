#include "Shader.h"
#include <GLAD/glad.h>
#include <fstream>
#include <iostream>

Shader::Shader(const char* path, unsigned int programID, int shaderType) {
	//std::cout << "running the stuff lmao" << std::endl;
	std::ifstream file(path, std::ios::ate | std::ios::binary);
	int length = file.tellg();
	file.seekg(0, std::ios::beg);
	char* source = (char*)malloc(length * sizeof(char));
	//std::cout << source << std::endl;
	file.read(source, length);
	source[file.gcount()] = '\0';
	std::cout << source << std::endl;
	ID = glCreateShader(shaderType);
	glShaderSource(ID, 1, &source, NULL);
	glCompileShader(ID);
	glAttachShader(programID, ID);
}
