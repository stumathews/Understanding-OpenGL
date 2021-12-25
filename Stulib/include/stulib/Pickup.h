#pragma once
#include "stulib/GameObject.h"
#include "stulib/Cube.h"
#include "Shapes.h"
#include "stulib/events/event_manager.h"
#include "stulib/StuTexture.h"

class Pickup : public GameObject
{
private:
	Shapes shapes;
	std::vector<std::shared_ptr<IPerFrame>> renderables;
	std::shared_ptr<stu::Texture> texture;
public:
	Pickup(std::string name, 
		shared_ptr<stu::ShaderProgram> program, 
		std::shared_ptr<gamelib::event_manager> event_admin,
		std::shared_ptr<stu::Texture> texture, 
		glm::vec3 initialOffset = glm::vec3(0.0f, 0.0f, 0.0f));

	virtual void Create() override;
	// Inherited via GameObject
	virtual glutil::MatrixStack Transform(glutil::MatrixStack& modelViewMatrixStack) override;
	virtual void Render() override;
	virtual void Update(float delta) override;

	// Inherited via GameObject
	virtual std::vector<std::shared_ptr<gamelib::event>> handle_event(std::shared_ptr<gamelib::event> evt) override;
	virtual std::string get_subscriber_name() override;

	GameObjectType GetType();

    enum class PickupProperties { PickupValue };

    static std::map<PickupProperties, std::string> WellKnownPropertyKeys;

};

