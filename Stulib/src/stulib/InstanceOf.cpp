#include "pch.h"
#include "InstanceOf.h"

InstanceOf::InstanceOf(std::shared_ptr<GameObject> gameObject, std::string name, std::shared_ptr<gamelib::event_manager> event_admin, glm::vec3 initialTranslationOffset)
	: GameObject(name, event_admin, initialTranslationOffset)
{
	this->underlyingGameObject = gameObject;
	this->shaderProgram = underlyingGameObject->shaderProgram;
	this->renderables = underlyingGameObject->GetRenderables();
}

glutil::MatrixStack InstanceOf::Transform(glutil::MatrixStack& modelViewMatrixStack)
{
	modelViewMatrixStack.Translate(offset);
	modelViewMatrixStack.Scale(scaleFactor);
	return modelViewMatrixStack;
}

void InstanceOf::Render()
{
	// Render the underlying object. Note tha instance specific transformation is done before rendering.
	underlyingGameObject->Render();
}

GameObject::GameObjectType InstanceOf::GetType()
{
	// Pretend to be the underlying object
	return this->underlyingGameObject->GetType();
}

std::shared_ptr<GameObject> InstanceOf::GeUnderlyingObject() const
{
	return this->underlyingGameObject;
}
