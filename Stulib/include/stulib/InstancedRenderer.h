#pragma once
#include "GameObject.h"
#include "ShaderProgram.h"
#include "Renderable.h"

namespace stu
{
	class InstancedRenderer
	{
		size_t totalNumberOfVertices = 0;
		
		VertexArrayObject VAO;
		VertexBufferObject VBO;
		VertexBufferObject instanceColoursArrayVBO;
		VertexBufferObject instanceViewMatricesArrayVBO;
		bool isCreated = false;
		std::shared_ptr<stu::ShaderProgram> shaderProgram;
		std::shared_ptr<GameObject> instance;
		std::vector<glm::vec3> instanceColours;
		std::vector<glm::mat4> instanceMatrices;
	public:
		InstancedRenderer(std::shared_ptr<GameObject> instance, std::vector<glm::vec3> instanceColours);

		void UpdateInstancePositions(std::vector<glm::mat4> instanceMatrices);

		void Render();
		void Create();

		virtual size_t GetNumberOfVertices();
		bool CompileShaders();

		std::shared_ptr<stu::ShaderProgram> GetShaderProgram() 
		{
			return shaderProgram;
		}
				
	};
}

