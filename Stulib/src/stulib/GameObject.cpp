#include "pch.h"
#include "cgtlib\MatrixStack.h"
#include "GameObject.h"
#include "events/GameObjectEvent.h"

GameObject::GameObject(std::string name,std::string meshPath, std::shared_ptr<stu::ShaderProgram> shaderProgram, std::shared_ptr<gamelib::event_manager> event_admin, glm::vec3 initialTranslationOffset) 
	: GameObject(name, shaderProgram, event_admin, initialTranslationOffset)
{
	this->meshPath = meshPath;
	this->hasMesh = true;
	SetDefaults();
}

GameObject::GameObject(std::string name, std::shared_ptr<stu::ShaderProgram> shaderProgram, std::shared_ptr<gamelib::event_manager> event_admin, glm::vec3 initialTranslationOffset)
	: GameObject(name, event_admin, initialTranslationOffset)
{	
	this->shaderProgram = shaderProgram;
	this->hasShaderProgram = true;
	hasMesh = false;
	SetDefaults();
}

GameObject::GameObject(std::string name, std::string meshPath, std::shared_ptr<gamelib::event_manager> event_admin, glm::vec3 initialTranslationOffset)
	: GameObject(name, event_admin, initialTranslationOffset)
{		
	hasMesh = true;
	this->meshPath = meshPath;
	SetDefaults();
}

GameObject::GameObject(std::string name, std::shared_ptr<gamelib::event_manager> event_admin, glm::vec3 initialTranslationOffset)
{
	this->name = name;	
	this->event_admin = event_admin;
	this->offset = initialTranslationOffset;
	this->hasMesh = false;
	this->hasShaderProgram = false;
	SetDefaults();
}

void GameObject::SetDefaults()
{
	float shininess = 500.0f; // Material is shiny by default

	//glm::vec3 AmbiantReflectance =  glm::vec3(0.3f); // Material needs light
	//glm::vec3 diffuseReflectance = glm::vec3(0.5f); // Material will reflect whitelight as the angle of light changes 
	//glm::vec3 specularReflectance = glm::vec3(0.0f); 


	//glm::vec3 AmbiantReflectance =  glm::vec3(0.0f, 0.0f, 0.2f); // Material needs light
	//glm::vec3 diffuseReflectance = glm::vec3(0.0f, 0.0f, 1.0f); // Material will reflect whitelight as the angle of light changes 
	//glm::vec3 specularReflectance = glm::vec3(0.0f); 

	glm::vec3 AmbiantReflectance =  glm::vec3(0.5f, 0.5f, 0.5f); // Material needs light
	glm::vec3 diffuseReflectance = glm::vec3(0.3f); // Material will reflect whitelight as the angle of light changes 
	glm::vec3 specularReflectance = glm::vec3(0.2f); 

	SetMaterialProperties(AmbiantReflectance, diffuseReflectance, specularReflectance, shininess);
}

void GameObject::SetMaterialProperties(glm::vec3 AmbiantReflectance, glm::vec3 diffuseReflectance, glm::vec3 specularReflectance, float shininess)
{
	materialProperties.AmbiantReflectance = AmbiantReflectance;
	materialProperties.diffuseReflectance = diffuseReflectance;
	materialProperties.specularReflectance = specularReflectance;
	materialProperties.shininess = shininess;	
}

bool GameObject::CollidedWith(std::shared_ptr<GameObject> other, float speed)
{
	auto yourPosition = other->GetPosition();
	auto myPosition =  this->GetPosition();    

	// Simple distance measure
	auto distance = glm::distance(myPosition, yourPosition);
	auto threshold = 100.0f * 1 * speed; // base the distance threshold on speed. Ie increase it if you got faster. Faster you go the more distance you cover and the greater threshhold required to catch objects within threshhold

	if(distance <  threshold-1 )
		return true;
	else
		return false;
}

/// <summary>
/// Get out position - transaltion offset
/// </summary>
/// <returns></returns>

inline glm::vec3 GameObject::GetPosition() const
{
	return offset;
}


// Game objects will by default translate by there position vector and then scale by scale vector - unless they override this
glutil::MatrixStack GameObject::Transform(glutil::MatrixStack& modelViewMatrixStack)
{
	// transalte
	modelViewMatrixStack.Translate(offset);

	// rotate
	modelViewMatrixStack.Rotate(rotationAxis, rotationAngle);

	// scale
	modelViewMatrixStack.Scale(scaleFactor);

	// The modified modelViewMatrix for this gameObject
	return modelViewMatrixStack;
}

void GameObject::Create()
{
	if(hasMesh)
		mesh.Load(meshPath);
	isCreated = true;
}

void GameObject::Update(float delta)
{
	// Not implemented yet
}

void GameObject::Render()
{
	shaderProgram->SetUniform("mixWithVertexColour", true); // We will assume that we only want the texture's colour. This can be overrided in derived class	
	shaderProgram->EnableUseTextureUniform(true);	

	// Use the object's meterial settings	
	shaderProgram->SendMaterialProperties(materialProperties.AmbiantReflectance, materialProperties.diffuseReflectance, materialProperties.specularReflectance, materialProperties.shininess);

	if(hasMesh)
		mesh.Render();
}

/// <summary>
/// Tell us which type you are
/// </summary>
/// <returns></returns>

GameObject::GameObjectType GameObject::GetType() 
{
	return GameObjectType::Unknown;
}

std::vector<std::shared_ptr<stu::Renderable>> GameObject::GetRenderables()
{
	return renderables;
}

bool GameObject::IsActive()
{
	return this->isActive;
}

void GameObject::SetActive(bool yesno)
{
	this->isActive = yesno;
}

struct MaterialProperties GameObject::GetMaterialProperties() 
{ 
	return this->materialProperties;; 
}

std::vector<std::shared_ptr<gamelib::event>> GameObject::handle_event(std::shared_ptr<gamelib::event> evt)
{
	// Not handling any generic events yet
	return std::vector<std::shared_ptr<gamelib::event>>();
}

std::string GameObject::get_subscriber_name()
{
	return name.size() == 0
		? name 
		: "Generic Game Object";
}

void GameObject::SetTranslationOffset(glm::vec3 offset)
{
	this->offset = offset;
}

void GameObject::SetScaleFactor(float scaleFactor) { this->scaleFactor = scaleFactor; }

void GameObject::SetRotationAngle(glm::vec3 rotationAxis, float angle)
{
	this->rotationAxis = rotationAxis;
	this->rotationAngle = angle;
}

std::string GameObject::GetName() { return name; }
