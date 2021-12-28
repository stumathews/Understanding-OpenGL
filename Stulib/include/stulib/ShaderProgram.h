#pragma once
#include <string>
#include <vector>

#include <string>


#include "Common.h"
#include "Shader.h"
#include "ShaderErrorHandling.h"
#include "IShaderProgram.h"


namespace stu 
{
	/// <summary>
	/// A shader program encapsulation for openGL
	/// </summary>
	class ShaderProgram : public IShaderProgram
	{
		// We can release the openGl shader Id
		void Release();
		GLuint Id;
		std::string samplerName;
		std::string useTexBooleanName, projectionMatrixName, modelViewMatrixName, normalMatrixName;
	public:
		VertexLayout vertexLayout;

		ShaderProgram(VertexLayout vertexLayout,
			std::string samplerName = "sampler0",
			std::string useTexBooleanName = "bUseTexture",
			std::string projectionMatrixName = "matrices.projMatrix",
			std::string modelViewMatrixName = "matrices.modelViewMatrix",
			std::string normalMatrixName = "matrices.normalMatrix"	
		)
		{		
			this-> Id = glCreateProgram();
			this-> vertexLayout = vertexLayout;
			this->samplerName = samplerName;
			this->useTexBooleanName = useTexBooleanName;
			this->projectionMatrixName = projectionMatrixName;
			this->modelViewMatrixName = modelViewMatrixName;
			this->normalMatrixName = normalMatrixName;
		}

		std::string GetSamplerName();
		std::string GetUseTexBooleanName();
		void EnableUseTextureUniform(bool yesno);

		void SendModelViewMatrix(glm::mat4 matrix);

		void SendProjectionMatrix(glm::mat4 matrix);

		void SendNormalMatrix(const glm::mat3 matrix);

		void SendMaterialProperties(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

		void Use();

		// Send data to the shader via uniforms
		void SetUniform(std::string name, glm::mat4 matrix)
		{
			int iLoc = glGetUniformLocation(GetId(), name.c_str());
			glUniformMatrix4fv(iLoc, 1, GL_FALSE, glm::value_ptr(matrix));	
		}

		void SetUniform(std::string name, const int value)
		{
			int iLoc = glGetUniformLocation(GetId(), name.c_str());
			glUniform1i(iLoc, value);
		}

		void SetUniform(std::string sName, glm::mat4* mMatrices, int iCount)
		{
			int iLoc = glGetUniformLocation(GetId(), sName.c_str());
			glUniformMatrix4fv(iLoc, iCount, GL_FALSE, (GLfloat*)mMatrices);
		}

		void SetUniform(std::string sName, const glm::vec3 vVector)
		{
			int iLoc = glGetUniformLocation(GetId(), sName.c_str());
			glUniform3fv(iLoc, 1, glm::value_ptr(vVector));
		}

		void SetUniform(std::string sName, const glm::vec4 vVector)
		{
			int iLoc = glGetUniformLocation(GetId(), sName.c_str());
			glUniform4fv(iLoc, 1, glm::value_ptr(vVector));
		}

		void SetUniform(std::string sName, const glm::mat3 mMatrix)
		{
			int iLoc = glGetUniformLocation(GetId(), sName.c_str());
			glUniformMatrix3fv(iLoc, 1, GL_FALSE, glm::value_ptr(mMatrix));
		}
	
		/// <summary>
		/// List of shaders in the program
		/// </summary>
		std::vector<Shader> shaders;
	
		/// <summary>
		/// Add a new shader to the shader program. If you dont need to ref this shader, use the copy overload that does not pass ref
		/// </summary>
		/// <param name="shader"></param>
		void AddShader(Shader& shader);


		/// <summary>
		/// Makes a copy of the shader it uses
		/// </summary>
		/// <param name="shader"></param>
		void CopyAddShader(Shader shader);
		/// <summary>
		/// Gets the number of attached shaders
		/// </summary>
		/// <returns></returns>
		GLint GetNumAttachedShaders();

		/// <summary>
		/// Link the shader program
		/// </summary>
		/// <returns></returns>
		bool Link();

		// Inherited via IShaderProgram
		virtual GLuint GetId() override;

		/// <summary>
		/// All good?
		/// </summary>
		bool IsOK = true;
	};
}


