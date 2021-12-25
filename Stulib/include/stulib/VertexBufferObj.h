#pragma once
#include "Common.h"
namespace stu
{
	/// <summary>
	/// Represents and manages a VBO
	/// </summary>
	class VertexBufferObject
	{
	private:
		GLenum intendedBindTarget;
		size_t size;
		bool isCreated = false;
		bool isBinded = false;
		bool preAllocateMemory = false;
		bool autoUse = false;		
		bool isReleased = false;
	public:
	
		/// <summary>
		/// Stores infomration about the VBO
		/// </summary>
		VertexBufferObject();

		// Creates the VBO
		void Create(size_t size, GLenum intendedBindTarget, bool preAllocate, bool autoBind = false);

		/// <summary>
		/// Binds the VBO and pre-allocates memory for it
		/// </summary>
		/// <returns></returns>
		bool Bind();

		/// <summary>
		/// Upload data to the VBO
		/// </summary>
		/// <param name="data"></param>
		void UploadData(const void* data, GLenum usage = GL_STATIC_DRAW);

		/// <summary>
		/// Specifies sub region within VBO
		/// </summary>
		/// <param name="startOffeset"></param>
		/// <param name="sizeOfNewData"></param>
		/// <param name="data"></param>
		void UploadPartionedData(GLintptr startOffeset, size_t sizeOfNewData, const void* data);

		/// <summary>
		/// Unbind the VBO
		/// </summary>
		/// <returns></returns>
		bool Release();

		~VertexBufferObject();

		size_t GetDataSize() { return size; }

		/// <summary>
		/// Open GL VBO Id
		/// </summary>
		GLuint Id;
	};
}

