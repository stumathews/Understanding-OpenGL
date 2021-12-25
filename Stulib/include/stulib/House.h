#pragma once
#include "stulib/GameObject.h"
#include "stulib/events/Event.h"
#include "Shapes.h"
#include "stulib/events/event_manager.h"
#include "stulib/StuTexture.h"
#include "stulib/Triangle.h"
#include "stulib/Renderable.h"

class House : public GameObject
{
	Shapes shapes;
	glm::vec3 initialPoint = glm::vec3(-0.5f, -0.5f, 0.0f);
	std::shared_ptr<stu::Texture> wallTexture;
	std::shared_ptr<stu::Texture> roofTexture;
	stu::VertexLayout vertexLayout;
	bool showNormals = false;
public:
	House(std::string name,
		std::shared_ptr<stu::ShaderProgram> shaderProgram,
		std::shared_ptr<gamelib::event_manager> event_admin, 
		std::shared_ptr<stu::Texture> wallTexture,
		std::shared_ptr<stu::Texture> roofTexture, 
		glm::vec3 initialOffset = glm::vec3(0.0f, 0.0f, 5.0f));
	
	virtual void Create() override;
	virtual void Update(float delta) override;
	virtual void Render() override;

	virtual glutil::MatrixStack Transform(glutil::MatrixStack& modelViewMatrixStack) override;

	// Inherited via GameObject
	virtual std::vector<std::shared_ptr<gamelib::event>> handle_event(std::shared_ptr<gamelib::event> evt) override;
	virtual std::string get_subscriber_name() override;


	GameObjectType GetType() override;

	void SetShowNormals(bool yesNo);
};

