#pragma once
#include "stulib/GameObject.h"
#include "stulib/events/event_manager.h"
#include "stulib/Square.h"
#include "stulib/Triangle.h"
class TitleScreen : public GameObject
{
private:
	int timeoutSecs;
	std::shared_ptr<stu::Texture> texture;
	std::shared_ptr<stu::Square> surface;
public:
	TitleScreen(std::string name, std::shared_ptr<stu::Texture> texture, std::shared_ptr<stu::ShaderProgram> shaderProgram, std::shared_ptr<gamelib::event_manager>, int timeoutSecs, glm::vec3 bottLeft = glm::vec3(0));
	void Render() override;
	void Update(float delta) override;
	void Create() override;
};

