#include "pch.h"
#include "VertexArrayObject.h"

// Use/Register/Enable/Apply the Vertex Array


// Creates a new Vertex Array Object

void stu::VertexArrayObject::Create()
{
	glGenVertexArrays(1, &Id);
	isCreated = true;
}

stu::VertexArrayObject::VertexArrayObject()
{
	Id = 0;
}

void stu::VertexArrayObject::Use()
{
	if (!isCreated)
		Create();
	glBindVertexArray(Id);
}

void stu::VertexArrayObject::Record(std::function<void(void)> recording)
{
	Use();
	recording();
	Release();
}

void stu::VertexArrayObject::Release()
{
	glBindVertexArray(0);
}
