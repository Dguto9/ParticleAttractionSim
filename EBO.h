#pragma once
class EBO
{
public:
	unsigned int ID;
	EBO();
	int Bind();
	int Unbind();
	int Data(unsigned int* indices, unsigned int size);
	~EBO();
};
