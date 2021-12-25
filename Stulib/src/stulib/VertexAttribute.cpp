#include "pch.h"
#include "VertexAttribute.h"

namespace stu
{
	void VertexAttribute::Create(int id, int componentType, int numComponentsPerAttribute, int stride)
	{
		this->numComponentsPerAttribute = numComponentsPerAttribute;
		this->componentType = componentType;
		this->id = id;
		this->stride = stride;
	}
	void VertexAttribute::Enable( const void* startOfData)
	{
		glEnableVertexAttribArray(id);
		glVertexAttribPointer(id, numComponentsPerAttribute, componentType, GL_FALSE, stride, startOfData);
	}

	int VertexAttribute::MaxSupportedCount()
	{
		GLint nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);	
		return nrAttributes;
	}
}