#pragma once
#include "common.h"
#include <iostream>
namespace stu
{
	/// <summary>
	/// Abstracts error handing for both shader program and shaders themselves
	/// </summary>
	class ShaderErrorHandling
	{
	public:

		/// <summary>
		///  Check to see if there was an error in the ... isShaderProgram?
		/// </summary>
		/// <param name="query"></param>
		/// <param name="message"></param>
		/// <param name="Id"></param>
		/// <param name="isShaderProgram"></param>
		/// <returns>The message to the caller (who is isShaderProgram or not)</returns>
		static bool is_gl_error(GLenum query, std::string &message, GLuint Id, bool isShaderProgram)
		{
			GLint success;
			GLchar infoLog[512];
			if(isShaderProgram)
				glGetProgramiv(Id, query, &success);
			else			
				glGetShaderiv(Id, query, &success);			

			if(!success)
			{
				if(isShaderProgram){
					glGetProgramInfoLog(Id, 512, NULL, infoLog);
					message = "Program ERROR:" + std::string(infoLog);
				} else {			
					glGetShaderInfoLog(Id, 512, NULL, infoLog);
					message = "Shader " + Id + std::string("ERROR:") + std::string(infoLog);
				}
				
				
				return true;
			}
			return false;
		};
	};
}
