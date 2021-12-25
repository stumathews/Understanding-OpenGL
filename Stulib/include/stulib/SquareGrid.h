#pragma once
#include "Renderable.h"
#include "Common.h"
#include "Square.h"
#include <vector>
namespace stu
{
	class SquareGrid : public Renderable
	{
	public:
		int rows, cols, width, height;
		glm::vec3 initialPoint;

		SquareGrid(glm::vec3 initialPoint, int rows, int cols, int width, int height);

		std::vector<std::shared_ptr<stu::Square>> squares;
		std::vector<glm::vec3> points;

		std::vector<std::shared_ptr<stu::Square>> CreateMyGrid();

		// Inherited via Renderable
		virtual void Create() override;
		virtual void Update(float delta) override;
		virtual void Render() override;
	};
}

