#pragma once
#include "Common.h"
namespace stu
{
	/// <summary>
	/// Manages and represents a single vertex attribute in the VBO
	/// </summary>
	class VertexAttribute
	{
	private:
		int componentType;
		int numComponentsPerAttribute;
		int id;
		int stride;
	public:

		/// <summary>
		///  Stores details about the Vertex attribute
		/// </summary>
		/// <param name="id"></param>
		/// <param name="componentType"></param>
		/// <param name="numComponentsPerAttribute"></param>
		/// <param name="stride"></param>
		void Create(int id, int componentType, int numComponentsPerAttribute, int stride = 0);

		/// <summary>
		/// Enable the VBO and set the vertex array pointer
		/// </summary>
		/// <param name="startOfData"></param>
		void Enable(const void* startOfData);
		static int MaxSupportedCount();
	};
}

