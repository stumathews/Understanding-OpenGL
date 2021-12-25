#pragma once
#include "GameObject.h"
#include "Common.h"
class Spotlight : public GameObject
{
public:
	Spotlight(std::string name, std::string meshPath, std::shared_ptr<stu::ShaderProgram> shaderProgram, std::shared_ptr<gamelib::event_manager> event_admin, glm::vec3 initialTranslationOffset  = glm::vec3(0.0f,0.0f,0.0f));
	glm::vec3 ambiantColour;
	glm::vec3 diffuseColour;
	glm::vec3 specularColour;
	glm::vec3 direction;
	float cutoff, exponent;
};

