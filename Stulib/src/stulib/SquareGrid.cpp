#include "pch.h"
#include "SquareGrid.h"

stu::SquareGrid::SquareGrid(glm::vec3 initialPoint, int rows, int cols, int width, int height)
{
	this->initialPoint = initialPoint;
	this->rows = rows;
	this->cols = cols;
	this->width = width;
	this->height = height;
}

std::vector<std::shared_ptr<stu::Square>> stu::SquareGrid::CreateMyGrid()
{
	auto factor = 0.3f;
	std::vector<std::shared_ptr<stu::Square>> scene;
	auto initialSquare = std::make_shared<stu::Square>(initialPoint, width, height);
	auto nextStartingPoint = initialSquare->A;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			auto square = std::make_shared<stu::Square>(nextStartingPoint, width, height);
			square->Create();
			scene.push_back(square);
			nextStartingPoint = square->D;

			if (r == 0)
			{
				//first row
				if (c == 0)
				{
					// first col
					points.push_back(square->B);
					// special point
					auto A = square->A;
					auto centralPoint = glm::vec3(A.x + factor, A.y + factor, A.z);
					points.push_back(centralPoint);
					points.push_back(square->D);
					points.push_back(square->D);
				}
				else if (c == cols - 1)
				{
					//last col
					points.push_back(square->A);
					//special point
					auto D = square->D;
					auto centralPoint = glm::vec3(D.x - factor, D.y + factor, D.z);
					points.push_back(centralPoint);
					points.push_back(square->C);
					points.push_back(square->C);
				}
				else
				{
					//points.push_back(square->D);
				}
			}

			if (r == rows - 1)
			{
				// last row
				if (c == 0)
				{
					// start col
					points.push_back(square->A);
					//special point
					auto B = square->B;
					auto centralPoint = glm::vec3(B.x + factor, B.y - factor, B.z);
					points.push_back(centralPoint);
					points.push_back(square->C);
					points.push_back(square->C);
				}
				else if (c == cols - 1)
				{
					// last col
					points.push_back(square->B);
					//special point
					auto C = square->C;
					auto centralPoint = glm::vec3(C.x - factor, C.y - factor, C.z);
					points.push_back(centralPoint);
					points.push_back(square->D);
					points.push_back(square->D);
				}
				else
				{
					//points.push_back(square->C);
				}
			}

		}
		nextStartingPoint = glm::vec3(initialSquare->A.x, initialSquare->A.y + (r * width), initialSquare->A.z);
	}
	return scene;
}

void stu::SquareGrid::Create()
{
	squares = CreateMyGrid();
}

void stu::SquareGrid::Update(float delta)
{
}

void stu::SquareGrid::Render()
{
	for(auto &square : squares)
		square->Render();
}
