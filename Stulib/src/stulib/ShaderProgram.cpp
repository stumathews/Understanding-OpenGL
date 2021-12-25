#include "pch.h"
#include "ShaderProgram.h"
#include "ShaderErrorHandling.h"
#include "Common.h"

using namespace std;
namespace stu {
	void ShaderProgram::Release()
	{
		for (auto& shader : shaders)
		{
			// now that we've linked the objects to the program, we can delete the original compiled shader objects
			glDeleteShader(shader.Id);
		}
	}

	void ShaderProgram::EnableUseTextureUniform(bool yesno)
	{
		SetUniform(GetUseTexBooleanName(), yesno);
	}

	void ShaderProgram::SendModelViewMatrix(glm::mat4 matrix)
	{
		SetUniform(modelViewMatrixName, matrix);
	}

	void ShaderProgram::SendProjectionMatrix(glm::mat4 matrix)
	{
		SetUniform(projectionMatrixName, matrix);
	}

	void ShaderProgram::SendNormalMatrix(const glm::mat3 matrix)
	{
		SetUniform(normalMatrixName, matrix);
	}

	void ShaderProgram::SendMaterialProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
	{		
		SetUniform("material1.shininess", shininess); // Shininess material property
		SetUniform("material1.Ma", ambient); // Ambient material reflectance
		SetUniform("material1.Md", diffuse); // Diffuse material reflectance
		SetUniform("material1.Ms", specular); // Specular material reflectance
	}

	std::string ShaderProgram::GetSamplerName()
	{
		return this->samplerName;
	}

	std::string ShaderProgram::GetUseTexBooleanName()
	{
		return this->useTexBooleanName;
	}

	void ShaderProgram::Use()
	{	
		glUseProgram(Id);
	}


	void ShaderProgram::AddShader(Shader &shader)
	{
		if(!shader.IsReady()){
			shader.Create();
			shader.Compile();
		}

		if(shader.IsOK)
			shaders.push_back(shader);
	}

	// Copy add
	void ShaderProgram::CopyAddShader(Shader shader)
	{
		AddShader(shader);
	}

	bool ShaderProgram::Link()
	{
		for(auto &shader : shaders)
		{
			if(shader.IsOK){
				glAttachShader(Id, shader.Id);
			}
		}

		GLint attachedCount;
		glGetProgramiv(Id, GL_ATTACHED_SHADERS, &attachedCount);
		std::cout << attachedCount << " attached shaders." << std::endl;

		glLinkProgram(Id);
				
		std::string message;

		if(ShaderErrorHandling::is_gl_error(GL_LINK_STATUS, message, Id, true))
		{
			std::cout << message << std::endl;
			throw message;
		}

		return IsOK;
	}

	GLuint ShaderProgram::GetId()
	{
		return Id;
	}
}