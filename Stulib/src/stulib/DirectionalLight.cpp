#include "pch.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(std::string name,
	std::shared_ptr<stu::ShaderProgram> shaderProgram, 
	std::shared_ptr<gamelib::event_manager> event_admin, 
	glm::vec3 start,
	glm::vec3 end, 
	glm::vec3 colour,
	bool normaliseDirection,
	bool performTransformation)
	: GameObject(name, shaderProgram, event_admin, start)
{
	this->end = end;
	this->colour = colour;
	this->normalizeDirection = normaliseDirection;
	this->performTransformation = performTransformation;
	
}

glutil::MatrixStack DirectionalLight::Transform(glutil::MatrixStack& modelViewMatrixStack)
{
	return modelViewMatrixStack;
	// This is so that we align our position values correctly in wordspace
	auto result = modelViewMatrixStack;
	auto transformed = glm::vec4(this->offset,1) * result.Top();
	this->offset = glm::vec3(transformed.x, transformed.y, transformed.z);
	
	auto transformedEnd = glm::vec4(this->end,1) * result.Top();
	this->end = glm::vec3(transformedEnd.x, transformedEnd.y, transformedEnd.z);
	return modelViewMatrixStack;
}

void DirectionalLight::Create()
{
	GameObject::Create();

	auto factor = 10.0f;
	auto from = this->offset;
	auto to =  normalizeDirection 
		?  this->offset + glm::normalize(end) * factor 
		: end;
	
	directionLine = shared_ptr<stu::Line>(new stu::Line(from, to, colour));
	directionLine->Create();
}

void DirectionalLight::Render()
{
	GameObject::Render();
	directionLine->Render();
}

