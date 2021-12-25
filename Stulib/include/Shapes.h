#pragma once
#include "stulib/PerFrame.h"
#include "stulib/Common.h"
#include "stulib/ShaderProgram.h"
#include "stulib/GameObject.h"

class Shapes
{
public:
	Shapes();
	void TextureTriangles(glm::vec3& initialPoint, bool draw, std::vector<std::shared_ptr<stu::IPerFrame>> &scene, std::shared_ptr<stu::ShaderProgram> withInterimShaderProrgam);
	void CreateCube(glm::vec3& A, bool draw, std::vector<shared_ptr<stu::IPerFrame>>& scene, std::shared_ptr<stu::ShaderProgram> withInterimShaderProrgam);
	void MergeTriangles(glm::vec3& A, bool draw, std::vector<std::shared_ptr<stu::IPerFrame>> &scene, std::shared_ptr<stu::ShaderProgram> withInterimShaderProrgam);
	void drawSquareGrid(glm::vec3& A, bool draw);
	void CreateStandAloneTriangle(glm::vec3& initialPoint, bool draw, std::vector<std::shared_ptr<stu::IPerFrame>> &scene, std::shared_ptr<stu::ShaderProgram> withInterimShaderProrgam);
	void CreateHouse(glm::vec3& initialPoint, bool draw, std::vector<std::shared_ptr<stu::IPerFrame>> &scene, std::shared_ptr<stu::ShaderProgram> &withInterimShaderProrgam);
	void CreateTextureSquare(glm::vec3& A, bool draw);
	
};