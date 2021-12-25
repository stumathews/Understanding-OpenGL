#include "pch.h"
#include "Spotlight.h"

Spotlight::Spotlight(std::string name, std::string meshPath, std::shared_ptr<stu::ShaderProgram> shaderProgram, std::shared_ptr<gamelib::event_manager> event_admin, glm::vec3 initialTranslationOffset)
	: GameObject(name, meshPath, shaderProgram, event_admin, initialTranslationOffset)
{
}
