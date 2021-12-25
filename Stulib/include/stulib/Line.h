#pragma once
#include "Renderable.h"

namespace stu
{
	/// <summary>
	/// A line primative 
	/// </summary>
	class Line : public Renderable
	{
	public:
		Line(glm::vec3 start, glm::vec3 end, glm::vec3 color = glm::vec3(0.0f, 255.0f, 0.0f));
		
		std::vector<GLfloat> vertices;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> colours;
		
		// Inherited via Renderable
		virtual void Create() override;
		virtual void Render() override;
		virtual void Update(float delta) override;

		// Change the line
		void Set(glm::vec3 start, glm::vec3 end, glm::vec3 colour = glm::vec3(0.0f, 255.0f, 0.0f));
	

	private:
		void Initialize(glm::vec3& start, glm::vec3& end, glm::vec3 color = glm::vec3(0.0f, 255.0f, 0.0f));
		void SetColour(glm::vec3 &colour);
	};
}

