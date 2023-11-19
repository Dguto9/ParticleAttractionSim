#pragma once
class VBO
{
public:
	unsigned int ID;
	VBO();
	int Bind();
	int Unbind();
	int Data(float *vertices, unsigned int size);
	~VBO();
};







