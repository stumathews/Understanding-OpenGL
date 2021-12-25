#include "pch.h"
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderErrorHandling.h"

using namespace std;
namespace stu 
{
	Shader::Shader(std::string shaderFilePath, std::string name, GLenum type)
	{
		this->Id = 0;
		this->shaderFilePath = shaderFilePath;
		this->name = name;
		this->IsCompiled = false;
		this->IsCreated = false;
		this->shaderType = type;
	}
	GLuint Shader::Create()
	{		
		if(IsCreated)
		{		
			std::cout << "Already created" << endl;
			return Id;
		}
		if(!IsOK)
		{
			std::cout << "Problem creating shader" << endl;
			return -1;
		}
		std::ifstream shaderFile;

		// ensures ifstream objects can throw exceptions:
		shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			shaderFile.open(shaderFilePath.c_str());			
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << shaderFile.rdbuf();
			shaderFile.close();
			
			shaderCode = vShaderStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR: "<< name <<" FILE_NOT_SUCCESFULLY_READ" << std::
			endl;
		}

		const GLchar* code = shaderCode.c_str();
		Id = glCreateShader(shaderType);	
		glShaderSource(Id, 1, &code, NULL); // vertexShaderSource is one long Cstring (ie \0 terminated)

		IsCreated = true;
		return IsCreated;
	}

	bool Shader::Compile()
	{
		if(!IsCreated)
		{
			std::cout << "Shader '"<< name<<"' not created" << std::endl;
			IsOK = IsCompiled = false;
			return IsOK;
		}

		if(!IsOK)
		{
			std::cout << "Problem creating shader '" << name << "'" << endl;
			return false;
		}

		glCompileShader(Id);

		std::string message;

		if(ShaderErrorHandling::is_gl_error(GL_COMPILE_STATUS, message, Id, false)){
			std::cout << name << ":" << message << endl;
			IsOK = IsCompiled = false;
			return IsOK;
		}
		IsCompiled = true;
		return IsCompiled;
	}

	bool Shader::IsReady() { return IsCreated && IsCompiled; }
}