#pragma once
#include "stulib/GameObject.h"
#include "stulib/Renderable.h"

/// <summary>
/// A lightweight instance to another game Object.
/// The instance contains instance specific overrides and uses the underlying game object it refers to as its subject
/// </summary>
class InstanceOf : public GameObject, public stu::Renderable
{
	/// <summary>
	/// Object we are referring to
	/// </summary>
	std::shared_ptr<GameObject> underlyingGameObject;
	

public:
	InstanceOf(std::shared_ptr<GameObject> gameObject, std::string name, std::shared_ptr<gamelib::event_manager> event_admin, glm::vec3 initialTranslationOffset = glm::vec3(0.0f,0.0f,0.0f));
	
	/// <summary>
	/// We override set own own transformations for the underlying object
	/// </summary>
	/// <param name="modelViewMatrixStack"></param>
	virtual glutil::MatrixStack Transform(glutil::MatrixStack &modelViewMatrixStack) override;

	/// <summary>
	/// We will render the underlying object
	/// </summary>
	virtual void Render() override;

	GameObject::GameObjectType GetType() override;

	void Create() override{}
	void Update(float delta) override {}

	std::shared_ptr<GameObject> GeUnderlyingObject() const;
};

