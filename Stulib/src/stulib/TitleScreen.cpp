#include "pch.h"
#include "TitleScreen.h"


TitleScreen::TitleScreen(std::string name, std::shared_ptr<stu::Texture> texture, std::shared_ptr<stu::ShaderProgram> shaderProgram, std::shared_ptr<gamelib::event_manager>, int timeoutSecs, glm::vec3 bottLeft)
	:GameObject(name, shaderProgram, event_admin, bottLeft)
{
	this->texture = texture;
	this->timeoutSecs = timeoutSecs;
}

void TitleScreen::Render()
{

	surface->Render();

}

void TitleScreen::Update(float delta)
{
}

void TitleScreen::Create()
{
	GameObject::Create();

	texture->Bind(GL_TEXTURE_2D, 2, true);
	surface = std::shared_ptr<stu::Square>(new stu::Square(GetPosition(), texture->GetWidth(), texture->GetHeight(), shaderProgram, texture, 2));


	surface->Create();	
	
}
