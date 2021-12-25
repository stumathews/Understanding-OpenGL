#pragma once

#include "Common.h"
#include "GameWindow.h"
#include "cgtlib\CatmullRom.h"
#include "stulib\events\EventSubscriber.h"
#include "stulib\events\event_manager.h"
#include "stulib\GameObject.h"
#include "stulib\PerFrame.h"
#include "stulib\ShaderProgram.h"
#include "stulib\Player.h"
#include "include\freeimage\FreeImage.h"
#include <functional>
#include "stulib\StuTexture.h"
#include <map>
#include "stulib\Pickup.h"
#include "stulib\StuFrameBufferObject.h"
#include "stulib\Box.h"
#include "stulib\InstancedRenderer.h"
#include "stulib\DirectionalLight.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;

class Game : public gamelib::EventSubscriber, public inheritable_enable_shared_from_this<Game> {
private:

	// Player cemera views
	enum class Views
	{
		FirstPersonView,
		TopView,
		ThirdPersonView,
		LeftSideView,
		RightSideView,
		FreeView,
		FirstPersonRearView,
	};

	// Movement modes 
	enum class MovementMode
	{
		Simulation,
		FreeCamera,
		FreePlayerWithCamera,
	};

	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void SetupShaders();
	void SetupEvents();
	void SetupTextures();
	void CreateScene();
	void Update(size_t pass = 0);
	void Render();

	// Set the player position views based on the View being used
	void SetPlayerView(float delta);

	// Set the player and camera view at the same time
	void SetPlayerAndCameraView(glm::vec3 currentPosition, glm::vec3 nextPosition, float delta, bool setCamera = true);

	// Draw route on the catmulrom spline
	void SetupRoute();

	void DrawScene(size_t pass);

	// place pickup on the centerline
	void PlacePickups();

	// place cones on the edges of the road to demarcate the route
	void PlaceRouteMarkers();

	// Create() a game object and add it to the scene ie call its Create() function prior to adding to scene
	void CreateAndAddToScene(std::shared_ptr<GameObject> obj);

	// turn skylights on/off 
	void SetSkylightsActiveStatus(shared_ptr<GameObject> spotlight, bool active);

	// Setup the main lighting in the scene
	void SetSunLighting(std::shared_ptr<stu::ShaderProgram> shaderProgram, 
		glm::mat4 viewMatrix, 
		glm::mat3 viewNormalMatrix, 
		std::string lightname, 
		glm::vec4 lightPosition,
		bool on);
	
	// Setup the spotlighting
	void SetSpotLighting(std::shared_ptr<GameObject> light,
		std::shared_ptr<stu::ShaderProgram> shaderProgram, 
		glm::mat4 viewMatrix,
		glm::mat3 viewNormalMatrix,
		std::string lightname, 
		bool on,		
		std::shared_ptr<GameObject> candidateForLighting);
	
	// Add game object directly to scene (without creating it first)
	void AddToScene(std::shared_ptr<GameObject> obj);

	// Remove from the scene
	void RemoveFromScene(std::shared_ptr<GameObject> obj);

	// Detect collsion between object and the player
	void DetectCollisionsWithPlayer(
		shared_ptr<GameObject> object, 
		std::map<std::string, std::shared_ptr<GameObject>> collisions);

	// Load an image into memory and return a pointer to it's raw bytes
	unsigned char * LoadTextureImage(std::string filePath, int * width, int * height);

	// Free a previously loaded image's raw bytes in memory
	bool FreeTextureImageData(unsigned char* image);

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;	
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	CFreeTypeFont *m_pFtFont;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CCatmullRom *m_pCatmullRom;

	// Used for image loading
	FIBITMAP* dib;
	std::function<unsigned char*(std::string, int*, int*)> getImageData;
	std::function<bool(unsigned char*)> freeImageData;

	// Textures we'll re-use so lets have pointers to them
	std::shared_ptr<stu::Texture> pickupTexture;
	std::shared_ptr<stu::Texture> pickup1Texture;
	std::shared_ptr<stu::Texture> pickup2Texture;
	std::shared_ptr<stu::Texture> pickup3Texture;
	std::shared_ptr<stu::Texture> roofTexture;
	std::shared_ptr<stu::Texture> wallTexture;
	std::shared_ptr<stu::Texture> roadTexture;
	std::shared_ptr<stu::Texture> titleTexture;

	// Some Game objects will be replicated multiple in the scene using InstancesOf() these objects
	// So we'll need references to these objects when we replicate them (see InstanceOf class)
	std::shared_ptr<GameObject> light;
	std::shared_ptr<GameObject> spotlight1;
	std::shared_ptr<GameObject> spotlight2;
	std::shared_ptr<GameObject> cone;
	std::shared_ptr<Pickup> pickup;
	std::shared_ptr<stu::Box> rearViewMirror;
	std::vector<shared_ptr<GameObject>> roadMarkers;
	std::shared_ptr<DirectionalLight> sunlightDirectionLine;
	std::shared_ptr<DirectionalLight> spotlight1DirectionLine;
	std::shared_ptr<DirectionalLight> spotlight2DirectionLine;

	// Create our lights
	glm::vec3 magenta = glm::vec3(1.0f, 0.0f, 1.0f);
	glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 brown = glm::vec3(0.65f, 0.42f,0.42f);

	std::shared_ptr<stu::FrameBufferObject> offscreen;

	// Out player
	std::shared_ptr<Player> player;
	std::shared_ptr<Player> audi;
	std::shared_ptr<Player> jeep;

	// Our list of game objects in the scene
	std::map<std::string, std::shared_ptr<GameObject>> scene;

	// List of objects that collided with player - these will be removed after scene is frame is finished
	std::map<std::string, std::shared_ptr<GameObject>> collisions;

	std::vector<std::shared_ptr<GameObject>> spotlights;
	

	// What view we are currently in
	Views view = Views::ThirdPersonView;

	// What mode of movement are we in
	MovementMode movementMode = MovementMode::Simulation;

	// The interime shader is my shader for local object rendering
	std::shared_ptr<stu::ShaderProgram> withInterimShaderProrgam;

	// The main shader is for global scene rendering - light and mesh texturing etc
	std::shared_ptr<stu::ShaderProgram> spotlightShaders;
	std::shared_ptr<stu::ShaderProgram> basicLightingShaders;
	std::shared_ptr<stu::ShaderProgram> defaultShaderProgram;
	std::shared_ptr<stu::ShaderProgram> offscreenShaderProgram;
	std::shared_ptr<stu::ShaderProgram> pickupShaders;
	std::shared_ptr<stu::ShaderProgram> noLightingShaders;
	std::shared_ptr<stu::ShaderProgram> screenspaceShader;
	
	// Number of points the player has accumulated
	int numPoints = 0;

	// Speed of the player
	float m_speed = 0.08f;

	// Speed factor to influence rate of change of speed
	const float speed_factor = 0.01f; // scale speed 

	// camera rotation in 1st person view
	float m_cameraRotation;

	bool useNewVehicle = false;

	// allow the player to roam around the scene 
	bool useFreeView = false;

	// allow to detech the camera from the simulation (car continues to move while camera is free to roam)
	bool m_detatchCamera = false;

	// main shader lighting control
	bool m_sunlightOn = true;

	// Use Phong model for sunlight or ue constant Ambiant, Diffuse, Specular components for unlight
	bool m_useRealisticLighting = true;

	// interim shader lighting control
	bool m_useSpotlight1 = true;
	bool m_useSpotlight2 = true;

	// Rear view mirror mode 
	bool m_isMultiPass = true;

	bool m_useFog = true;

	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;
	float m_currentDistance;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

	// We can subscribe to the event bus and get notification of events we subscribed to
	std::vector<std::shared_ptr<gamelib::event>> handle_event(std::shared_ptr<gamelib::event> evt);

	// we need to have a subscriber name
	std::string get_subscriber_name();

	// we need t have a subscriber id
	int get_subscriber_id();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;

	// Is the camera roaming around independantly of the player 
	// ie can the player move around freely with mouse (ie affects camera view-point only)?
	bool isFreeView = true;

	// We join the event bus for events that we are interested in.
	shared_ptr<gamelib::event_manager> event_admin;	// We will use a shared event manager 
};
