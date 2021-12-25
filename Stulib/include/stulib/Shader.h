#pragma once
#include "Common.h"
#include <iostream>

namespace stu {

	/// <summary>
	/// A shader can have an associated vertex layout.
	/// This allows objects that will be associated/used/rendered with it to create there VBOs according to the
	/// format specified here. This allows an object to support being rendered under diffirent shaders that use 
	/// diffirent layouts
	/// </summary>
	enum class VertexLayout
	{		
		// Position, Normal, Colour, Texture Coordinates - my default (used for interim coursework too)
		PosNormColTex,
		// Position, Texture Coordinates, Normal, Colour - course's material default
		PosTexNormCol
	};

	class Shader
	{
		/// <summary>
		/// The code of the shader
		/// </summary>
		std::string shaderCode;

	public:

		Shader(std::string shaderFilePath, std::string name, GLenum type);
	
		/// <summary>
		/// Create the shader and store is Id
		/// </summary>
		/// <returns></returns>
		GLuint Create();

		/// <summary>
		/// Compile the Shader
		/// </summary>
		/// <returns></returns>
		bool Compile();

		/// <summary>
		/// Check if its all good - no failures
		/// </summary>
		/// <returns></returns>
		bool IsReady();
	
		GLuint Id = 0;

		/// <summary>
		/// We can specify if this is a fragment or vertex shader or any other type supported by GLEnum.
		/// </summary>
		GLenum shaderType;

		/// <summary>
		/// Name of the shader
		/// </summary>
		std::string name;

		/// <summary>
		/// Path of the shader on the file system
		/// </summary>
		std::string shaderFilePath;

		/// <summary>
		/// Is it compiled?
		/// </summary>
		bool IsCompiled = false;

		/// <summary>
		/// Is it created?
		/// </summary>
		bool IsCreated = false;

		/// <summary>
		/// Is everything all good?  No Errors?
		/// </summary>
		bool IsOK = true;	
	};
}