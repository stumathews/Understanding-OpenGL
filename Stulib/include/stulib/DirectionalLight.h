#pragma once
#include "stulib/GameObject.h"
#include "stulib/Line.h"
#include "Pickup.h"
#include "stulib/Common.h"
class DirectionalLight : public GameObject
{
		glm::vec3 end;
		std::shared_ptr<stu::Line> directionLine;
		std::shared_ptr<Pickup> block;
		glm::vec3 colour;
		bool normalizeDirection = true;
		//glm::vec3 startPosition;
		bool performTransformation = true;
public: 
	DirectionalLight(std::string name,
		std::shared_ptr<stu::ShaderProgram>,
		std::shared_ptr<gamelib::event_manager> event_admin,
		glm::vec3 initialTranslationOffset,
		glm::vec3 directionOfLight,
		glm::vec3 colour = glm::vec3(255.0f, 255.0f, 255.0f),
		bool normaliseDirection = true,
		bool performTransformation = true
	);

	glutil::MatrixStack Transform(glutil::MatrixStack &modelViewMatrixStack);

	void Create() override;
	void Render() override;


};

