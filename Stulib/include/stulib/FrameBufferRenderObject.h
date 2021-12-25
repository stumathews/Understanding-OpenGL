#pragma once
#include "Common.h"
class FrameBufferRenderObject
{
	GLuint id;
	size_t width, height;
public:
	FrameBufferRenderObject();

	void Create(size_t width = 800, size_t height = 600);
	GLuint GetId();

	size_t GetWidth();

	size_t GetHeight();
};