#pragma once

#include <glad/glad.h>

struct BindlessPtrNV
{
	GLuint   index;
	GLuint   reserved;
	GLuint64 address;
	GLuint64 length;
};

struct DrawArraysIndirectCommand
{
	GLuint  count;
	GLuint  instanceCount;
	GLuint  first;
	GLuint  baseInstance;
};

struct DrawArraysIndirectBindlessCommandNV
{
	DrawArraysIndirectCommand   cmd;
	BindlessPtrNV               vertexBuffers[1];
};