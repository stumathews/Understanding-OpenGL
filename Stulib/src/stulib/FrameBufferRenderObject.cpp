#pragma once
#include "pch.h"
#include "FrameBufferRenderObject.h"

FrameBufferRenderObject::FrameBufferRenderObject()
{
}

void FrameBufferRenderObject::Create(size_t width, size_t height)
{
	this->width = width;
	this->height = height;
	glGenRenderbuffers(1, &id);
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLuint FrameBufferRenderObject::GetId()
{
	return id;
}

size_t FrameBufferRenderObject::GetWidth()
{
	return width;
}

size_t FrameBufferRenderObject::GetHeight()
{
	return height;
}
