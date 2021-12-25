#pragma once
#include "Common.h"
namespace stu {
	class ElementBufferObject
	{
	private:
	public:
		void Create()
		{
			glGenBuffers(1, &Id);
		}
		GLuint Id;
	};
}

