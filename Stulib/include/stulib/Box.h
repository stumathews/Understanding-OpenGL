#pragma once
#include "Common.h"
#include "PerFrame.h"
#include "PerFrameOptions.h"
#include "Renderable.h"
#include "ShaderProgram.h"
#include "Common.h"


namespace stu 
{
	/// <summary>
	/// Simple Box.
	/// Pass in points of the Quad: by default uses NDCs but can use word-words if you're consistent
	/// </summary>
	class Box : public Renderable
	{
	private:
		std::shared_ptr<stu::ShaderProgram> shaderProgram;
		glm::vec3 A;
		glm::vec3 D;		
		glm::vec3 C;
		glm::vec3 B;
	public:
		/// <summary>
		/// Create a Box
		/// </summary>
		/// <param name="shaderProgram">Shader Program</param>
		/// <param name="A">Bottom Left Corner</param>
		/// <param name="D">Bttom Right Corner</param>
		/// <param name="C">Top Right Corner</param>
		/// <param name="B">Top Left Corner</param>
		Box(std::shared_ptr<stu::ShaderProgram> shaderProgram, 
			glm::vec3 A = glm::vec3(-1.0, -1.0, 0.0f), 
			glm::vec3 D = glm::vec3(1.0, -1.0, 0.0f), 
			glm::vec3 C = glm::vec3(1.0, 1.0, 0.0f),
			glm::vec3 B = glm::vec3(-1.0, 1.0, 0.0f));

		/// <summary>
		/// Creates the Box's Vertices and uploads them to GPU
		/// </summary>
		void Create() override;
		void Update(float delta) override;
		void Render() override;
	};
}

