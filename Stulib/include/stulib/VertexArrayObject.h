#pragma once
#include "Common.h"
#include <functional>
namespace stu 
{
	class VertexArrayObject
	{
	private:
		bool isCreated = false;

		// Creates a new Vertex Array Object - automatically created when used for the first time
		void Create();
	public:
	
		VertexArrayObject();

		/// <summary>
		/// Replay vertex layout recording
		/// </summary>
		void Use();

		// Record the creation of the vertex layout
		void Record(std::function<void(void)> recording);

		void Release();
		GLuint Id;
	};
}
