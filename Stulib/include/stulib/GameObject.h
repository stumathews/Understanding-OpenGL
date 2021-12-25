#pragma once
#include "PerFrame.h"
#include <string>
#include "cgtlib\OpenAssetImportMesh.h"
#include "ShaderProgram.h"
#include "events\EventSubscriber.h"
#include "common\MultipleInheritableEnableSharedFromThis.h"
#include "events\event_manager.h"
#include <vector>
#include "Renderable.h"
#include "cgtlib\MatrixStack.h"


struct MaterialProperties 
{
	glm::vec3 AmbiantReflectance;
	glm::vec3 diffuseReflectance;
	glm::vec3 specularReflectance;
	float shininess;
};

/// <summary>
/// All objects in the scene are game objects or derivatives thereof.
/// This class provides base functionality for derived classes that want to act as objects in the scene
/// </summary>

class GameObject : public stu::IPerFrame,
				   public gamelib::EventSubscriber,
				   public inheritable_enable_shared_from_this<GameObject> // I can get a pointer to myself
{
	/// <summary>
	/// Sets the Game Objects Defaults
	/// </summary>
	void SetDefaults();
protected:
	 
	/// <summary>
	/// Every game object has a name that we can identify it by
	/// </summary>
	std::string name;	

	/// <summary>
	/// We use a shared event manager
	/// </summary>
	std::shared_ptr<gamelib::event_manager> event_admin;

	/// <summary>
	/// How much we move ourselves by - aka our position
	/// </summary>
	glm::vec3 offset;

	/// <summary>
	/// The modelView matrix
	/// </summary>
	glm::mat4 currentMatrix;

	/// <summary>
	/// List of underlying renderables
	/// </summary>
	std::vector<std::shared_ptr<stu::Renderable>> renderables;

	/// <summary>
	/// Indication if this Game Object has a visible mesh or not
	/// </summary>
	bool hasMesh = true;

	/// <summary>
	/// Indication if this game Object has a shader program associated with it or not
	/// </summary>
	bool hasShaderProgram = false;

	/// <summary>
	/// The Path to the GameObject's Mesh
	/// </summary>
	std::string meshPath;	

	/// <summary>
	/// Indication if the Object is created yet
	/// </summary>
	bool isCreated = false;

	/// <summary>
	/// Storage of the Mesh
	/// </summary>
	COpenAssetImportMesh mesh;

	/// <summary>
	/// Objects default scale
	/// </summary>
	float scaleFactor = 1.0f;

	/// <summary>
	/// Object's default rotation angle
	/// </summary>
	float rotationAngle = 0.0f;

	/// <summary>
	/// Objects rotation axis
	/// </summary>
	glm::vec3 rotationAxis = glm::vec3(0, 1, 0);

	/// <summary>
	/// Indication if the Objects is active
	/// </summary>
	bool isActive = true;

	// All objects have the same material properties by default		
	struct MaterialProperties materialProperties;

public:

	// Game Object can be hard-coded types
	enum class GameObjectType 
	{ 
		Pickup, 
		Player, 
		House, 
		TrackSegment, 
		Unknown 
	};
	
	/// <summary>
	/// Construct a Game Object that has its own associated mesh
	/// </summary>
	/// <param name="name">Name of the game object to identify it from others</param>
	/// <param name="meshPath">The filepath to the mesh file</param>
	/// <param name="shaderProgram">The shader program we'll use to render ourselves with</param>
	/// <param name="event_admin">Shared event bus</param>
	/// <param name="initialTranslationOffset">How much we should translate ourselves to our final world-coorindate position</param>
	GameObject(std::string name, 
		std::string meshPath, 
		std::shared_ptr<stu::ShaderProgram> shaderProgram, 
		std::shared_ptr<gamelib::event_manager> event_admin, 
		glm::vec3 initialTranslationOffset  = glm::vec3(0.0f,0.0f,0.0f));

	/// <summary>
	/// Construct a game object that has no associted mesh. Could represent some other game element
	/// </summary>
	/// <param name="name">Name of the game object to identify it from others</param>
	/// <param name="meshPath">The filepath to the mesh file</param>
	/// <param name="shaderProgram">The shader program we'll use to render ourselves with</param>
	/// <param name="event_admin">Shared event bus</param>
	/// <param name="initialTranslationOffset">How much we should translate ourselves to our final world-coorindate position</param>
	GameObject(std::string name, 
		std::shared_ptr<stu::ShaderProgram> shaderProgram, 
		std::shared_ptr<gamelib::event_manager> event_admin, 
		glm::vec3 initialTranslationOffset  = glm::vec3(0.0f,0.0f,0.0f));

	/// <summary>
	/// Construct a Game object that has no associated shader but has a mesh
	/// </summary>
	/// <returns></returns>
	GameObject(std::string name, 
		std::string meshPath, 
		std::shared_ptr<gamelib::event_manager> event_admin, 
		glm::vec3 initialTranslationOffset  = glm::vec3(0.0f,0.0f,0.0f));

	/// <summary>
	/// Construct a Game object that has no shader or mesh associated
	/// </summary>
	/// <param name="name">Name of game object</param>
	/// <param name="event_admin">event admin</param>
	/// <param name="initialTranslationOffset">initial position of game object</param>
	GameObject(std::string name, 
		std::shared_ptr<gamelib::event_manager> event_admin, 
		glm::vec3 initialTranslationOffset = glm::vec3(0.0f,0.0f,0.0f));
	
	/// <summary>
	/// Shader program to render the game object with
	/// </summary>
	std::shared_ptr<stu::ShaderProgram> shaderProgram; 

	/// <summary>
	/// All game objects have rudimentary collision support
	/// </summary>
	/// <param name="other">Other object to inspect</param>
	/// <returns>true, if this object collided with other object, false otherwise</returns>
	virtual bool CollidedWith(std::shared_ptr<GameObject> other, float speed);
	
	/// <summary>
	/// Get out position - transaltion offset
	/// </summary>
	/// <returns></returns>
	virtual glm::vec3 GetPosition() const;

	/// <summary>
	/// Set our position - translation offset
	/// </summary>
	/// <param name="offset"></param>
	void SetTranslationOffset(glm::vec3 offset);

	/// <summary>
	/// Set the scale factor that this game object will use during transformation
	/// </summary>
	/// <param name="scaleFactor"></param>
	void SetScaleFactor(float scaleFactor);

	/// <summary>
	/// Sets how much the object shoould be rotated in the transform function
	/// </summary>
	/// <param name="angle"></param>
	void SetRotationAngle(glm::vec3 rotationAxis, float angle);

	/// <summary>
	/// Get the name of this game object - good for identifying objects in the scene and removing them from scene etc
	/// </summary>
	/// <returns></returns>
	std::string GetName();

	/// <summary>
	/// Easily set the Object's material properties
	/// </summary>
	/// <param name="AmbiantReflectance">AmbiantReflectance</param>
	/// <param name="diffuseReflectance">diffuseReflectance</param>
	/// <param name="specularReflectance">specularReflectance</param>
	/// <param name="shininess">shininess</param>
	void SetMaterialProperties(glm::vec3 AmbiantReflectance, glm::vec3 diffuseReflectance, glm::vec3 specularReflectance, float shininess);
	

	// Inherited via IPerFrame
	
	/// <summary>
	/// All Game objects can specify there own transformations prior to rendering.
	/// This is used to create objects with transformation criteria at creation time and have them used during rendering
	/// </summary>
	/// <param name="modelViewMatrixStack">modelViewMatrixStack</param>
	/// <returns></returns>
	virtual glutil::MatrixStack Transform(glutil::MatrixStack &modelViewMatrixStack);

	/// <summary>
	/// All game objects can create themselves or initialize their internals
	/// </summary>
	virtual void Create() override;

	/// <summary>
	/// All game objects can be updated consistantly
	/// </summary>
	virtual void Update(float delta) override;	

	/// <summary>
	/// All game objects can have a say in how they are rendered
	/// </summary>
	virtual void Render() override;

	/// <summary>
	/// Tell us which type you are
	/// </summary>
	/// <returns></returns>
	virtual GameObjectType GetType();

	/// <summary>
	/// Can have a list of renderables that represents this object
	/// </summary>
	/// <returns></returns>
	std::vector<std::shared_ptr<stu::Renderable>> GetRenderables();

	bool IsActive();

	void SetActive(bool yesno);


	// Game objects can store arbitary information about themselves without having to 
	// create a new derived instance to hold those properties
	// we can store things like light properties etc if this game objects was associated with a light mesh for example

	std::map<std::string, std::string> PropertiesString;
	std::map<std::string, glm::vec3> PropertiesVec3;
	std::map<std::string, glm::vec4> PropertiesVec4;
	std::map<std::string, glm::vec2> PropertiesVec2;
	std::map<std::string, float> PropertiesFloat;
	std::map<std::string, int> PropertiesInt;
	std::map<std::string, bool> PropertiesBool;

	struct MaterialProperties GetMaterialProperties();

	// Inherited via EventSubscriber:
	
	// We are a subscriber of events so we can handle incomming events when they are generated on the event bus
	// by other game objects
	virtual std::vector<std::shared_ptr<gamelib::event>> handle_event(std::shared_ptr<gamelib::event> evt) override;

	/// <summary>
	/// As a subscriber we need a unique subscriber name
	/// </summary>
	virtual std::string get_subscriber_name() override;
};

