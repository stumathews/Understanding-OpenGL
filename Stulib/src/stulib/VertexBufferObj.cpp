#include "pch.h"
#include "VertexBufferObj.h"

stu::VertexBufferObject::VertexBufferObject()
{
	Id = 0;
	intendedBindTarget = GL_ARRAY_BUFFER;
	size = 0;

}

// Creates the VBO

void stu::VertexBufferObject::Create(size_t size, GLenum intendedBindTarget, bool preAllocate, bool autoBind)
{
	this->intendedBindTarget = intendedBindTarget;
	this->size = size;
	this->preAllocateMemory = preAllocate;
	this->autoUse = autoBind;
	glGenBuffers(1, &Id);
	isCreated = true;
	if (autoBind)
		Bind();
}

/// <summary>
/// Binds the VBO and pre-allocates memory for it
/// </summary>
/// <returns></returns>

bool stu::VertexBufferObject::Bind()
{
	if (!isCreated) {
		isBinded = false;
		return isBinded;
	}
	glBindBuffer(intendedBindTarget, Id);

	if (preAllocateMemory)
		UploadData(NULL); // No data is uploaded by it is allocated - see size

	isBinded = true;
	return isBinded;
}

/// <summary>
/// Upload data to the VBO
/// </summary>
/// <param name="data"></param>

void stu::VertexBufferObject::UploadData(const void* data, GLenum usage)
{
	glBufferData(intendedBindTarget, size, data, usage);
}

/// <summary>
/// Specifies sub region within VBO
/// </summary>
/// <param name="startOffeset"></param>
/// <param name="sizeOfNewData"></param>
/// <param name="data"></param>

void stu::VertexBufferObject::UploadPartionedData(GLintptr startOffeset, size_t sizeOfNewData, const void* data)
{
	glBufferSubData(intendedBindTarget, startOffeset, sizeOfNewData, data);
}

/// <summary>
/// Unbind the VBO
/// </summary>
/// <returns></returns>

bool stu::VertexBufferObject::Release()
{
	if (!isBinded)
		return false;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	isReleased = true;
	return true;
}

stu::VertexBufferObject::~VertexBufferObject()
{
	if (!isReleased)
		Release();
}
