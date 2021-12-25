/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)
*/


#include "game.h"


// Setup includes
#include "cgtlib\HighResolutionTimer.h"
#include "GameWindow.h"
#include "gamelib\Functions.h"
#include "stulib\TestClass.h"

// Game includes
#include "Camera.h"
#include "cgtlib\Skybox.h"
#include "cgtlib\Plane.h"
#include "cgtlib\Shaders.h"
#include "cgtlib\FreeTypeFont.h"
#include "cgtlib\Sphere.h"
#include "cgtlib\MatrixStack.h"
#include "cgtlib\OpenAssetImportMesh.h"
#include "cgtlib\Audio.h"
#include "stulib\common\MultipleInheritableEnableSharedFromThis.h"
#include "stulib\events\GameObjectEvent.h"
#include <memory>
#include "stulib\GameObject.h"
#include "Shapes.h"
#include "stulib\ShaderProgram.h"
#include "stulib\Shader.h"
#include "stulib\House.h"
#include "stulib\Pickup.h"
#include "stulib\Player.h"
#include <algorithm>
#include "stulib\Square.h"
#include "stulib\TrackSegment.h"
#include "stulib\InstanceOf.h"
#include "stulib\PlayerCollisionEvent.h"
#include "stulib\TNBHelper.h"
#include "stulib\TitleScreen.h"

// Constructor
Game::Game()
{
    m_pSkybox = NULL;
    m_pCamera = NULL;
    m_pShaderPrograms = NULL;
    m_pPlanarTerrain = NULL;
    m_pFtFont = NULL;
    
    m_pHighResolutionTimer = NULL;
    m_pAudio = NULL;

    m_dt = 0.0;
    m_framesPerSecond = 0;
    m_frameCount = 0;
    m_elapsedTime = 0.0f;
    m_cameraRotation = 0.0f;
    event_admin = make_shared<gamelib::event_manager>();
}

// Destructor
Game::~Game() 
{ 
    //game objects
    delete m_pCamera;
    delete m_pSkybox;
    delete m_pPlanarTerrain;
    delete m_pFtFont;	
    delete m_pAudio;
    delete m_pCatmullRom;
    
    delete m_pBarrelMesh;
    delete m_pHorseMesh;

    if (m_pShaderPrograms != NULL) {
        for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
            delete (*m_pShaderPrograms)[i];
    }
    delete m_pShaderPrograms;

    //setup objects
    delete m_pHighResolutionTimer;
}

unsigned char* Game::LoadTextureImage(std::string filePath, int* width, int* height)
{		
        FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
        

        fif = FreeImage_GetFileType(filePath.c_str(), 0); // Check the file signature and deduce its format

        if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
            fif = FreeImage_GetFIFFromFilename(filePath.c_str());
    
        if(fif == FIF_UNKNOWN) // If still unknown, return failure
            return nullptr;

        if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
            dib = FreeImage_Load(fif, filePath.c_str());

        if(!dib) {
            char message[1024];
            sprintf_s(message, "Cannot load image\n%s\n", filePath.c_str());
            //MessageBox(NULL, message, "Error", MB_ICONERROR);
            return nullptr;
        }

        BYTE* pData = FreeImage_GetBits(dib); // Retrieve the image data

        // If somehow one of these failed (they shouldn't), return failure
        if (pData == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
            return nullptr;
    

        GLenum format;
        int bada = FreeImage_GetBPP(dib);
        if(FreeImage_GetBPP(dib) == 32)format = GL_BGRA;
        if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
        if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;

        *width = FreeImage_GetWidth(dib);
        *height = FreeImage_GetHeight(dib);
            
        return pData;

}

bool Game::FreeTextureImageData(unsigned char* image)
{
    FreeImage_Unload(dib);
    return true;
}
// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
    // Set the clear colour and depth
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);

    // Create objects

    m_pCamera = new CCamera(event_admin);
    m_pSkybox = new CSkybox;
    m_pShaderPrograms = new vector <CShaderProgram *>;
    m_pPlanarTerrain = new CPlane;
    
    m_pBarrelMesh = new COpenAssetImportMesh;
    m_pHorseMesh = new COpenAssetImportMesh;
    m_pFtFont = new CFreeTypeFont;
    m_pAudio = new CAudio;
    m_pCatmullRom = new CCatmullRom;
        
    RECT dimensions = m_gameWindow.GetDimensions();

    int width = dimensions.right - dimensions.left;
    int height = dimensions.bottom - dimensions.top;

    // Set the orthographic and perspective projection matrices based on the image size
    m_pCamera->SetOrthographicProjectionMatrix(width, height); 
    m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

    // Load shaders
    vector<CShader> shShaders;
    vector<string> sShaderFileNames;
    //sShaderFileNames.push_back("mainShader.vert");
    //sShaderFileNames.push_back("mainShader.frag");
    sShaderFileNames.push_back("textShader.vert");
    sShaderFileNames.push_back("textShader.frag");
    sShaderFileNames.push_back("textShader.vert");
    sShaderFileNames.push_back("textShader.frag");

    for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
        string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
        int iShaderType;
        if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
        else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
        else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
        else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
        else iShaderType = GL_TESS_EVALUATION_SHADER;
        CShader shader;
        shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
        shShaders.push_back(shader);
    }

    // Create the main shader program
    CShaderProgram *pMainProgram = new CShaderProgram;
    pMainProgram->CreateProgram();
    pMainProgram->AddShaderToProgram(&shShaders[0]);
    pMainProgram->AddShaderToProgram(&shShaders[1]);
    pMainProgram->LinkProgram();
    m_pShaderPrograms->push_back(pMainProgram);

    // Create a shader program for fonts
    CShaderProgram *pFontProgram = new CShaderProgram;
    pFontProgram->CreateProgram();
    pFontProgram->AddShaderToProgram(&shShaders[2]);
    pFontProgram->AddShaderToProgram(&shShaders[3]);
    pFontProgram->LinkProgram();
    m_pShaderPrograms->push_back(pFontProgram);

    // You can follow this pattern to load additional shaders

    // Create the skybox
    // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
    m_pSkybox->Create(2500.0f);
    
    // Create the planar terrain
    m_pPlanarTerrain->Create("resources\\textures\\", "grass_rocky_d.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

    m_pFtFont->LoadFont("resources\\Fonts\\BungeeInline-Regular.ttf", 32);
    //m_pFtFont->LoadSystemFont("BungeeInline-Regular.ttf", 32); //"arial.ttf"
    //m_pFtFont->LoadSystemFont("arial.ttf", 32); //"arial.ttf"
    m_pFtFont->SetShaderProgram(pFontProgram);

    //m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
    //m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
    
    glEnable(GL_CULL_FACE);

    // Initialise audio and play background music
    m_pAudio->Initialise();
    m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
    m_pAudio->LoadMusicStream("resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
    //m_pAudio->PlayMusicStream();

    stu::TestClass testclass;
    SayHello(testclass.GetName().c_str());

    SetupEvents();
    SetupTextures();	
    SetupShaders();
    CreateScene();
}

void Game::SetupShaders()
{
    // Create our programs
    spotlightShaders = std::make_shared<stu::ShaderProgram>(stu::VertexLayout::PosTexNormCol);
    basicLightingShaders = std::make_shared<stu::ShaderProgram>(stu::VertexLayout::PosTexNormCol);	
    withInterimShaderProrgam = std::shared_ptr<stu::ShaderProgram>(new stu::ShaderProgram(stu::VertexLayout::PosNormColTex, "ourTexture1", "useTexture", "projectionMatrix", "modelViewMatrix", "normalMatrix"));
    pickupShaders = std::make_shared<stu::ShaderProgram>(stu::VertexLayout::PosNormColTex, "ourTexture1", "useTexture", "projectionMatrix", "modelViewMatrix", "normalMatrix");
    noLightingShaders = std::make_shared<stu::ShaderProgram>(stu::VertexLayout::PosNormColTex, "ourTexture1", "useTexture", "projectionMatrix", "modelViewMatrix", "normalMatrix");
    screenspaceShader = std::make_shared<stu::ShaderProgram>(stu::VertexLayout::PosTexNormCol, "screenTexture");

    /* Compile and Add shaders to their program */

    spotlightShaders->CopyAddShader(stu::Shader("resources\\shaders\\spotlights.vert", "With Main Vertex Shader", GL_VERTEX_SHADER));
    spotlightShaders->CopyAddShader(stu::Shader("resources\\shaders\\spotlights.frag", "With Main Fragment Shader", GL_FRAGMENT_SHADER));	

    basicLightingShaders->CopyAddShader(stu::Shader("resources\\shaders\\mainBasic.vert", "Main Basic Vertex Shader", GL_VERTEX_SHADER));
    basicLightingShaders->CopyAddShader(stu::Shader("resources\\shaders\\mainBasic.frag", "Main Basic Fragment Shader", GL_FRAGMENT_SHADER));
    
    withInterimShaderProrgam->CopyAddShader(stu::Shader("resources\\shaders\\interim-shader.frag", "With Interim Fragment Shader", GL_FRAGMENT_SHADER));
    withInterimShaderProrgam->CopyAddShader( stu::Shader("resources\\shaders\\interim-shader.vert", "With Interim Vertex Shader", GL_VERTEX_SHADER));

    pickupShaders->CopyAddShader(stu::Shader("resources\\shaders\\pickup.vert", "Pickup Vertex Shader", GL_VERTEX_SHADER));
    pickupShaders->CopyAddShader(stu::Shader("resources\\shaders\\pickup.frag", "Pickup Fragment Shader", GL_FRAGMENT_SHADER));

    noLightingShaders->CopyAddShader(stu::Shader("resources\\shaders\\NoLightingModel.vert", "Pickup Vertex Shader", GL_VERTEX_SHADER));
    noLightingShaders->CopyAddShader(stu::Shader("resources\\shaders\\NoLightingModel.frag", "Pickup Fragment Shader", GL_FRAGMENT_SHADER));
        
    screenspaceShader->CopyAddShader(stu::Shader("resources\\shaders\\framebuffer.vert", "Wih offscreen Vertex Shader", GL_VERTEX_SHADER));
    screenspaceShader->CopyAddShader(stu::Shader("resources\\shaders\\framebuffer.frag", "With ofscreen Interim Fragment Shader", GL_FRAGMENT_SHADER));

    

    /* Link */

    if (!noLightingShaders->Link())
        cout << "Failed to compile pickup shader program" << endl;

    if (!pickupShaders->Link())
        cout << "Failed to compile pickup shader program" << endl;

    if (!withInterimShaderProrgam->Link())
        cout << "Failed to compile interim shader program" << endl;

    if (!basicLightingShaders->Link())
        cout << "Failed to compile main basic shader program" << endl;

    if (!spotlightShaders->Link())
        cout << "Failed to compile main shader program" << endl;

    if(!screenspaceShader->Link())
        std::cout << "Failed to compile screenspace shader program" << std::endl;	
    

    // The main scene shader will be the spotlight shader which will produce the main lighting.
    defaultShaderProgram = spotlightShaders;
}

void Game::SetupEvents()
{		
    // Subscribe to Game Object event
    event_admin->subscribe_to_event(gamelib::event_type::GameObject,  shared_from_this());

    // Subscribe to player collision event
    event_admin->subscribe_to_event(gamelib::event_type::PlayerCollision, shared_from_this());
}

void Game::SetupTextures()
{
            
    dib = nullptr;

    // Local image loading/freeing functions that Textture class will use to load 2D textures (see below)
    getImageData = [&](std::string filePath, int* width, int* height) -> unsigned char* 
    {
        return LoadTextureImage(filePath, width, height);		
    };

    freeImageData = [&](unsigned char* image) -> bool
    {
        return FreeTextureImageData(image);
    };

    // We will re-use a few textures throughout the demo
    roofTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
    wallTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
    roadTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
    pickupTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
    pickup1Texture = std::make_shared<stu::Texture>(getImageData, freeImageData);
    pickup2Texture = std::make_shared<stu::Texture>(getImageData, freeImageData);
    pickup3Texture = std::make_shared<stu::Texture>(getImageData, freeImageData);
    titleTexture =  std::make_shared<stu::Texture>(getImageData, freeImageData);

    roadTexture->Create("resources\\textures\\roadm.bmp", true, true);
    wallTexture->Create("resources\\textures\\wall.jpg", true, true);
    roofTexture->Create("resources\\textures\\roof.jpg", true, true);
    pickupTexture->Create("resources\\textures\\Tile41a.jpg", true, true);
    pickup1Texture->Create("resources\\textures\\pickup1.jpg", true, true);
    pickup2Texture->Create("resources\\textures\\pickup2.jpg", true, true);
    pickup3Texture->Create("resources\\textures\\pickup3.jpg", true, true);
    titleTexture->Create("resources\\textures\\titlescreen.jpg", true, true);
    //titleTexture->Create("resources\\textures\\Tile41a.jpg", false, false);
}

void Game::CreateScene()
{
    // Frame Buffer object
    offscreen = std::shared_ptr<stu::FrameBufferObject>(new stu::FrameBufferObject(800, 600));
    offscreen->Create();

    // Create some arbitary objects for our scene:
    
    // a light
    light = std::shared_ptr<GameObject>(new GameObject("light", "resources\\models\\crystal\\crystals.obj", defaultShaderProgram, event_admin));
    light->Create();
    
    // A traffic cone
    /*cone = std::shared_ptr<GameObject>(new GameObject("light", "resources\\models\\Cone\\low.obj", defaultShaderProgram, event_admin));		
    cone->Create();
    cone->PropertiesFloat["scale"] = 10.0f;*/

    // A pickup
    pickup = std::shared_ptr<Pickup>(new Pickup("generic pickup", defaultShaderProgram, event_admin, pickupTexture));
    pickup->Create();
        
    // Create a plant #1
    auto fernPlant = std::shared_ptr<GameObject>(new GameObject("plant1", "resources\\models\\prefabs\\plant1.obj", defaultShaderProgram, event_admin));
    fernPlant->Create();
    fernPlant->PropertiesFloat["y"] = -26.0f;
    fernPlant->PropertiesFloat["scale"] = 10.0f;
    fernPlant->PropertiesFloat["offsetLeft"] = 30.0f;
    fernPlant->PropertiesFloat["offsetRight"] = 30.0f;
    fernPlant->PropertiesBool["IsPlant"] = true;

    // Create a plan #2
    auto brownFern = std::shared_ptr<GameObject>(new GameObject("plant2", "resources\\models\\prefabs\\plant2.obj", defaultShaderProgram, event_admin));
    brownFern->Create();
    brownFern->PropertiesFloat["y"] = -26.0f;
    brownFern->PropertiesFloat["scale"] = 10.0f;
    brownFern->PropertiesFloat["offsetLeft"] = 30.0f;
    brownFern->PropertiesFloat["offsetRight"] = 30.0f;
    brownFern->PropertiesBool["IsPlant"] = true;

    // Create plant #3
    auto palmPlant = std::shared_ptr<GameObject>(new GameObject("plant3", "resources\\models\\prefabs\\plant3.obj", defaultShaderProgram, event_admin));
    palmPlant->Create();
    palmPlant->PropertiesFloat["y"] = -50.0f;
    palmPlant->PropertiesFloat["scale"] = 20.0f;	
    palmPlant->PropertiesFloat["offsetLeft"] = 10.0f;
    palmPlant->PropertiesFloat["offsetRight"] = 10.0f;
    palmPlant->PropertiesBool["IsPlant"] = true;

    auto plant4 = std::shared_ptr<GameObject>(new GameObject("plant4", "resources\\models\\prefabs\\plant4.obj", defaultShaderProgram, event_admin));
    plant4->Create();
    plant4->PropertiesFloat["y"] = -50.0f;
    plant4->PropertiesFloat["scale"] = 20.0f;	
    plant4->PropertiesFloat["offsetLeft"] = 10.0f;
    plant4->PropertiesFloat["offsetRight"] = 10.0f;
    plant4->PropertiesBool["IsPlant"] = true;


 
        
    // Create our player
    jeep = std::make_shared<Player>("jeep", "resources\\models\\jeep\\jeep3.obj", defaultShaderProgram, event_admin);
    jeep->PropertiesFloat["FirstPersonB"] = 9.0f;
    jeep->PropertiesFloat["FirstPersonT"] = 3.0f;
    jeep->PropertiesFloat["FirstPersonN"] = 2.0f;
    jeep->PropertiesFloat["FirstPersonViewT"] = 1.0f;	
    jeep->PropertiesFloat["FirstPersonRearCamT"] = -20;
    jeep->PropertiesFloat["FirstPersonRearViewT"] = -20;			
    jeep->PropertiesFloat["TopB"] = 50.0f;
    jeep->PropertiesFloat["TopT"] = 1.0f;
    jeep->PropertiesFloat["ThirdT"] = -30.0f;
    jeep->PropertiesFloat["ThirdB"] = 20.0f;
    jeep->PropertiesFloat["ThirdViewT"] = 10.0f;
    jeep->PropertiesFloat["ThirdViewB"] = 10.0f;								
    jeep->PropertiesFloat["LeftN"] = -20.0f;
    jeep->PropertiesFloat["LeftB"] = 11.0f;
    jeep->PropertiesFloat["RightN"] = 20.0f;
    jeep->PropertiesFloat["RightB"] = 11.0f;
    jeep->SetScaleFactor(2.5f);

    audi = std::make_shared<Player>("audi", "resources\\models\\Audi\\Audi.obj", defaultShaderProgram, event_admin);
    audi->PropertiesFloat["FirstPersonB"] = 9.0f;
    audi->PropertiesFloat["FirstPersonT"] = -9.0f;
    audi->PropertiesFloat["FirstPersonN"] = 0.0f;
    audi->PropertiesFloat["FirstPersonViewT"] = 10.0f;
    audi->PropertiesFloat["FirstPersonRearCamT"] = -1;
    audi->PropertiesFloat["FirstPersonRearViewT"] = -1;			
    audi->PropertiesFloat["TopB"] = 50.0f;
    audi->PropertiesFloat["TopT"] = 1.0f;
    audi->PropertiesFloat["ThirdT"] = -20.0f;
    audi->PropertiesFloat["ThirdB"] = 10.0f;
    audi->PropertiesFloat["ThirdViewT"] = 1.0f;
    audi->PropertiesFloat["ThirdViewB"] = 5.0f;								
    audi->PropertiesFloat["LeftN"] = -20.0f;
    audi->PropertiesFloat["LeftB"] = 11.0f;
    audi->PropertiesFloat["RightN"] = 20.0f;
    audi->PropertiesFloat["RightB"] = 11.0f;

    player = audi;
    jeep->SetActive(false);
        
    // Create our house
    auto house = std::make_shared<House>("house", defaultShaderProgram, event_admin, wallTexture, roofTexture);
    house->SetRotationAngle(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
    house->SetScaleFactor(30.f);
    house->SetTranslationOffset(glm::vec3(150, 10.0f,230.0f));
    house->SetShowNormals(false);		

    // Create a cube
    auto cube = std::shared_ptr<Pickup>(new Pickup("cube", defaultShaderProgram, event_admin, titleTexture));
    cube->SetScaleFactor(100.0f);
    cube->SetTranslationOffset(glm::vec3(490.0f, 50.0f, 500.0f));
    //cube->SetRotationAngle(glm::vec3(0.5f, 0.5f, 0.5f), 45.0f);
    //cube->SetRotationAngle(glm::vec3(0.0f, 1.0f, 0.0f), 1.0);

    
    // Create a Crate
    //auto crate = std::shared_ptr<GameObject>(new GameObject("crate1", "resources\\models\\Crate\\Crate.obj", defaultShaderProgram, event_admin));	
   
    
    // Create a computer
    //auto pc1 = std::shared_ptr<GameObject>(new GameObject("pc1", "resources\\models\\prefabs\\pc1.obj", defaultShaderProgram, event_admin));		
    
    // Spotlight 1
    spotlight1 = std::shared_ptr<InstanceOf>(new InstanceOf(light, "spotlight1", event_admin, glm::vec3(30, 5, 10)));
    spotlight1->PropertiesVec3["La"] =  green;
    spotlight1->PropertiesVec3["Ld"] =  green;
    spotlight1->PropertiesVec3["Ls"] =  green;
    spotlight1->PropertiesFloat["exponent"] =  20.0f;
    spotlight1->PropertiesFloat["cutoff"] = 30.0f;
    spotlight1->PropertiesVec3["position"] = spotlight1->GetPosition();  // in world co-ordinates
    spotlight1->PropertiesVec4["position"] = glm::vec4(spotlight1->PropertiesVec3["position"], 1);
    spotlight1->PropertiesVec3["direction"] =  glm::vec3(0, -1, 0); // DOWN - will be normalized later
    spotlight1->PropertiesBool["IsLight"] = true;
    spotlight1->PropertiesFloat["radius"] = 100;

    // Spotlight 2
    spotlight2 = std::shared_ptr<InstanceOf>(new InstanceOf(light, "spotlight2", event_admin, glm::vec3(300, 5, 20)));	
    spotlight2->PropertiesVec3["La"] = yellow;
    spotlight2->PropertiesVec3["Ld"] = yellow;
    spotlight2->PropertiesVec3["Ls"] = yellow;	
    spotlight2->PropertiesFloat["exponent"] =  20.0f;
    spotlight2->PropertiesFloat["cutoff"] =  30.0f;
    spotlight2->PropertiesVec3["position"] = spotlight2->GetPosition();  // in world co-ordinates
    spotlight2->PropertiesVec4["position"] = glm::vec4(spotlight2->PropertiesVec3["position"], 1);
    spotlight2->PropertiesVec3["direction"] = glm::vec3(0, -1, 0); // DOWN - will be normalized later
    spotlight2->PropertiesBool["IsLight"] = true;
    spotlight2->PropertiesFloat["radius"] = 100;
    spotlight2->SetMaterialProperties(yellow, yellow, yellow, 200);

    // SUN
    auto pos3 = glm::vec3(0, 100, 0);
    sunlightDirectionLine = std::shared_ptr<DirectionalLight>(new DirectionalLight("sunDirection", noLightingShaders, event_admin, pos3, glm::vec3(pos3.x, 0.0f, pos3.z),glm::vec3(255.0f, 255.0f, 0.0f), false)); // normalize Direction?
    sunlightDirectionLine->SetActive(false);	

    auto titleScreen = std::shared_ptr<GameObject>(new TitleScreen("titleScreen", pickup1Texture, screenspaceShader, event_admin, 5));

    // Collect the list of spotlights
    spotlights.push_back(spotlight1);
    spotlights.push_back(spotlight2);
    
    // Move, Scale the models to place them appropriately around the scene.
    player->SetScaleFactor(2.5f);
    //crate->SetScaleFactor(5.0f);
    //crate->SetTranslationOffset(glm::vec3(15.0f, 0.0f, 15.0f));	
    //pc1->SetTranslationOffset(glm::vec3(10.0f, 0.0f, -40.0f));
    //pc1->SetRotationAngle(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
    light->SetTranslationOffset(glm::vec3(-15.0f, 0.0f, 25.0f));

    

    // Objects that will be places round the road
    roadMarkers.push_back(fernPlant);
    roadMarkers.push_back(palmPlant);
    roadMarkers.push_back(brownFern);
    roadMarkers.push_back(plant4);


    SetupRoute();
    PlaceRouteMarkers();
    PlacePickups();		
    
    CreateAndAddToScene(house);	
    CreateAndAddToScene(cube);	
    CreateAndAddToScene(jeep);
    CreateAndAddToScene(audi);
    //CreateAndAddToScene(crate);
   // CreateAndAddToScene(pc1);
    CreateAndAddToScene(spotlight1);
    CreateAndAddToScene(spotlight2);
    CreateAndAddToScene(sunlightDirectionLine); 
    //CreateAndAddToScene(titleScreen); 
}

void Game::CreateAndAddToScene(std::shared_ptr<GameObject> obj)
{
    obj->Create();
    AddToScene(obj);	
}

void Game::AddToScene(std::shared_ptr<GameObject> obj)
{
    auto inSceneAlready = scene.find(obj->GetName()) != scene.end();
    if(inSceneAlready)
        throw std::string("Scene should only have unique items in it. Copy paste error?");

    scene[obj->GetName()] = obj;
}

void Game::RemoveFromScene(std::shared_ptr<GameObject> obj)
{
    scene.erase(obj->GetName());
}


float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void Game::PlacePickups()
{
    auto controlPoints = m_pCatmullRom->GetControlPoints();
    const int numPickups = 20;
    auto pickupInterval = controlPoints.size() / numPickups;
    auto max_pickup_types = 3;
    int count = 0;
    int place = 0;
    static int pickup_type = 1;
    for( int i = 0 ; i < controlPoints.size(); i++)
    {
        if(count++ % pickupInterval == 0)
        {
            auto point = controlPoints[i];
            auto name = std::string("pickup") + std::string(std::to_string(place++));
            auto tnb = stu::TNBHelper(point, controlPoints[count]);
            auto newPoint = glm::vec3(point.x, point.y+5 , point.z);
            newPoint = tnb.MoveN(RandomFloat(5.0f, 10.0f), newPoint);
            shared_ptr<Pickup> pickup;
            auto rotatex = 0.05;
            auto rotatey = 0.05;
            auto rotatez = 0.05;
            switch(pickup_type)
            {
            case 1:
                pickup = std::shared_ptr<Pickup>(new Pickup(name, defaultShaderProgram, event_admin, pickup1Texture, newPoint));
                pickup->PropertiesInt[Pickup::WellKnownPropertyKeys[Pickup::PickupProperties::PickupValue]] = 1; 
                //pickup->SetRotationAngle(glm::vec3(rotatex, rotatey, rotatez), 1.0f);
                break;
                case 2:
                pickup = std::shared_ptr<Pickup>(new Pickup(name, defaultShaderProgram, event_admin, pickup2Texture, newPoint));
                pickup->PropertiesInt[Pickup::WellKnownPropertyKeys[Pickup::PickupProperties::PickupValue]] = 2; 
                //pickup->SetRotationAngle(glm::vec3(rotatex, rotatey, rotatez), 1.0f);
                break;
                case 3:
                pickup = std::shared_ptr<Pickup>(new Pickup(name, defaultShaderProgram, event_admin, pickup3Texture, newPoint));
                pickup->PropertiesInt[Pickup::WellKnownPropertyKeys[Pickup::PickupProperties::PickupValue]] = 3;
                //pickup->SetRotationAngle(glm::vec3(rotatex, rotatey, rotatez), 10.0f);
                break;                
            }
            pickup_type++;
            if(pickup_type > 3)
                pickup_type = 1;
            pickup->SetScaleFactor(3.0f);
            
            // if can have bad pickups with negative values to simulate pickups you want to avoid!

            CreateAndAddToScene(pickup);
        }
    }
}


void Game::PlaceRouteMarkers()
{

    auto leftPoints =  m_pCatmullRom->GetLeftPoints();
    auto rightPoints =  m_pCatmullRom->GetRightPoints();
    auto controlPoints =  m_pCatmullRom->GetControlPoints();
    const int numMarkers = 15;
    auto pickupInterval = controlPoints.size() / numMarkers;

    int count = 0;
    int place = 0;
    srand((unsigned) time(0));
    for(size_t i = 0; i < controlPoints.size() - 1; i++)
    {	
        auto left =  leftPoints[i];
        auto right = rightPoints[i];

        auto Tindex = 0;
        if(i+1 > controlPoints.size() )
            Tindex = i-1;
        else
            Tindex = i+1;

        auto leftTNB = stu::TNBHelper(left, leftPoints[Tindex]);		
        auto rightTNB = stu::TNBHelper(right, rightPoints[Tindex]);	

        if(count++ % pickupInterval == 0)
        {
            auto instance = roadMarkers[(rand() % roadMarkers.size())];
            auto il = roadMarkers[(rand() % roadMarkers.size())];

            auto height = instance->PropertiesFloat["y"];
            auto scale = instance->PropertiesFloat["scale"] == 0.0f ? 1.0 : instance->PropertiesFloat["scale"];
            auto offsetLeft = instance->PropertiesFloat["offsetLeft"];
            auto offsetRight = instance->PropertiesFloat["offsetRight"] ;
                        
            // modify the left marker position
            left = leftTNB.MoveB(height, left);
            left = leftTNB.MoveN(-offsetLeft, left);

            // modify the right marker position
            right = rightTNB.MoveB(height, right);
            right = rightTNB.MoveN(offsetRight, right);


            auto leftInstance = std::shared_ptr<InstanceOf>(new InstanceOf(instance, std::string("leftInstance") + std::to_string(i), event_admin, left));
            auto rightInstance = std::shared_ptr<InstanceOf>(new InstanceOf(instance, std::string("rightInstance") + std::to_string(i), event_admin, right));

            // scale the left marker
            leftInstance->SetScaleFactor(scale);

            //scale the right marker
            rightInstance->SetScaleFactor(scale);
            
            AddToScene(leftInstance);
            AddToScene(rightInstance);

        }
    }
}

void Game::SetupRoute()
{
    m_pCatmullRom->CreateCentreline();	
    m_pCatmullRom->CreateOffsetCurves();
    m_currentDistance = 0.0f;
    
    auto leftPoints =  m_pCatmullRom->GetLeftPoints();
    auto rightPoints =  m_pCatmullRom->GetRightPoints();
    auto controlPoints =  m_pCatmullRom->GetControlPoints();
    
    auto countControlPoints = controlPoints.size();
    auto portion = 35; // divide the control points for the route in portions - these will be the basis for the track segment
    const int numSegments = countControlPoints / portion;
    auto SegmentInterval = controlPoints.size() / numSegments;
    
    static int roadSpotlightCount = 0;
    int count = 0;

    for(size_t i = 0; i < countControlPoints - 1; i++)
    {
        if(count++ % SegmentInterval == 0)
        {
            auto left =  leftPoints[i];
            auto right = rightPoints[i];
                    
            
            // only create track segment if we have access to the last two points B and C
            auto nextControlPointIndex = i + portion;
            if(nextControlPointIndex <= countControlPoints -1)
            {
                
                auto A = left;
                auto D = right;
                auto B = leftPoints[nextControlPointIndex];
                auto C = rightPoints[nextControlPointIndex];
                
                
                // , A, D, B, C
                // chop off the outer parts of the road texture as we want only the road to be mapped  
                auto a = glm::vec2(0.3f, 0.0);
                auto d = glm::vec2(0.7f, 0.0);
                auto c = glm::vec2(0.7f, 1.0);
                auto b = glm::vec2(0.3f, 1.0);
                auto trackSegment = std::make_shared<TrackSegment>(std::string("track") + std::to_string(i) , A, D, B, C, roadTexture, defaultShaderProgram, event_admin);
                trackSegment->SetTextureCoordinates(a, d, c, b);
                
                CreateAndAddToScene(trackSegment);

                // Put lights up around over our tracks that are near to the light
                for(auto &spotlight : spotlights)
                {
                    if(glm::distance(trackSegment->GetPosition(), spotlight->GetPosition()) < spotlight->PropertiesFloat["radius"])
                    {
                         auto height = 30;
                         auto from = glm::vec3(A.x, height, A.z);
                         auto to = glm::vec3(D.x, height, D.z);
                         auto l =	std::shared_ptr<DirectionalLight>(new DirectionalLight(std::string("road-spotlight-")+ std::to_string(roadSpotlightCount),
                                                                                                    noLightingShaders, 
                                                                                                    event_admin, 
                                                                                                    from, // position
                                                                                                    to, // direction
                                                                                                    spotlight->PropertiesVec3["La"], 
                                                                                                    false,
                                                                                                    true)); //color of line
                         l->PropertiesString["BelongsTo"] = spotlight->GetName();
                         CreateAndAddToScene(l);
                         roadSpotlightCount++;
                    }
                }
            }
        }
    }	
}

void Game::DrawScene(size_t pass)
{
    // Clear the buffers and enable depth testing (z-buffering)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Set up a matrix stack
    glutil::MatrixStack modelViewMatrixStack;
    modelViewMatrixStack.SetIdentity();

    // Use the main shader program 
    //CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
    std::shared_ptr<stu::ShaderProgram> mainShader;

    mainShader = defaultShaderProgram;

    mainShader->Use();
    mainShader->SetUniform("bUseTexture", true);
    mainShader->SetUniform("bUseFog", m_useFog);
    mainShader->SetUniform("sampler0", 0);
    // Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
    int cubeMapTextureUnit = 10; 
    mainShader->SetUniform("CubeMapTex", cubeMapTextureUnit);
    

    // Set the projection matrix
    mainShader->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix(), 1);

    // Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
    // Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
    modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
    glm::mat4 viewMatrix = modelViewMatrixStack.Top();
    glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

    // Render the skybox and terrain with full ambient reflectance 
    modelViewMatrixStack.Push();
        mainShader->SetUniform("renderSkybox", true);
        // Translate the modelview matrix to the camera eye point so skybox stays centred around camera
        glm::vec3 vEye = m_pCamera->GetPosition();
        modelViewMatrixStack.Translate(vEye);
        mainShader->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        mainShader->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pSkybox->Render(cubeMapTextureUnit);
        mainShader->SetUniform("renderSkybox", false);
    modelViewMatrixStack.Pop();
    
    
    // Start Scene Rendering:		
    
    //glm::vec4 sunPosition = glm::vec4(0, 100, 0, 1); // Position of light source *in world coordinates*		

    /*auto pos3 = glm::vec3(sunPosition.x, sunPosition.y, sunPosition.z);
    auto directionSun = std::shared_ptr<DirectionalLight>(new DirectionalLight("direction2", noLightingShaders, event_admin, pos3, glm::vec3(0)));
    directionSun->SetColour(glm::vec3(255.0f, 255.0f, 0.0f));*/
    
    auto &spotlight1 = scene["spotlight1"];
    auto &spotlight2 = scene["spotlight2"];
        
    //whiteLight->PropertiesVec3["direction"] =  glm::vec3(0, -1, 1); // // will be normalized later

    // Use the sun light?
    SetSunLighting(mainShader, 
                  viewMatrix, 
                  viewNormalMatrix, 
                  "sun", 
                  glm::vec4(sunlightDirectionLine->GetPosition(), 1), // position of sun
                  m_sunlightOn);

    // Turn on diffuse + specular materials
    mainShader->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
    mainShader->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
    mainShader->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
    
    //// By default there is no lighlight on materials caused by viewer position changing
    mainShader->SetUniform("material1.shininess", 15.0f);	// Shininess material property

    // Render the planar terrain
    modelViewMatrixStack.Push();
        mainShader->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        mainShader->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pPlanarTerrain->Render();
    modelViewMatrixStack.Pop();

    mainShader->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
    mainShader->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
    mainShader->SetUniform("material1.Ms", glm::vec3(0.2f));	// Specular material reflectance

    
    // Render the horse 
    /*
    modelViewMatrixStack.Push();
        auto pos = glm::vec3(spotlight1->PropertiesVec4["position"].x, spotlight1->PropertiesVec4["position"].y, spotlight1->PropertiesVec4["position"].z);
        modelViewMatrixStack.Translate(pos*0.5f);
        //modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 40.0f));
        modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 180.0f);
        modelViewMatrixStack.Scale(2.5f);
        mainShader->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        mainShader->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pHorseMesh->Render();
    modelViewMatrixStack.Pop();

        
    // Render the barrel 
    modelViewMatrixStack.Push();
        modelViewMatrixStack.Translate(glm::vec3(100.0f, 0.0f, 0.0f));
        modelViewMatrixStack.Scale(5.0f);
        mainShader->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
        mainShader->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
        m_pBarrelMesh->Render();
    modelViewMatrixStack.Pop();
        */

    // Render our own scene objects		
    for(auto &sceneItem : scene)
    {
        auto gameObjectId = sceneItem.first;
        auto gameObject = sceneItem.second;

        if(gameObject == nullptr )
            continue;

        modelViewMatrixStack.Push();
            gameObject->Transform(modelViewMatrixStack); // allow object to move itself.
            gameObject->shaderProgram->Use(); // Activate the object's shader

            SetSpotLighting(spotlight1, defaultShaderProgram, 
                            viewMatrix, viewNormalMatrix, "spotlight1",
                            m_useSpotlight1, gameObject );
            SetSpotLighting(spotlight2, defaultShaderProgram, 
                            viewMatrix, viewNormalMatrix, "spotlight2",
                            m_useSpotlight2, gameObject);
                
            // Standard stuff we need to perform the transformations in our shader
            gameObject->shaderProgram->SendModelViewMatrix(modelViewMatrixStack.Top());
            gameObject->shaderProgram->SendProjectionMatrix(*m_pCamera->GetPerspectiveProjectionMatrix());
            gameObject->shaderProgram->SendNormalMatrix(m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));

            if(gameObject->IsActive())
                gameObject->Render();

        modelViewMatrixStack.Pop();
    }

    if(pass == 2 && m_isMultiPass) // Draw the offscreen buffer texture at top of screen on 2nd pass - the rear view mirror
        offscreen->Render();	

       // scene["titleScreen"]->Render();
}

/// <summary>
/// Render method runs repeatedly in a loop
/// </summary>
void Game::Render() 
{		
    auto reset = true; // Reset the framebuffer back to default after writing/recording to it
    size_t pass = 0;
    if(m_isMultiPass)
    {		
        offscreen->Record([&]()
        {		
            view = Views::FirstPersonRearView; 	// Render Rear view first
            SetPlayerView(0);
            
            DrawScene(++pass);			

            view = Views:: FirstPersonView; 	// Reset to first Person view
            SetPlayerView(0);
            
        }, reset);
    }
    
    DrawScene(++pass);	
    DisplayFrameRate(); // Show the HUD after all passes are complete	
    SwapBuffers(m_gameWindow.Hdc()); // Swap buffers to show the rendered image
}

void Game::SetSunLighting(std::shared_ptr<stu::ShaderProgram> shaderProgram, glm::mat4 viewMatrix, glm::mat3 viewNormalMatrix, std::string lightname, glm::vec4 lightPosition, bool on)
{
    std::string lightId = lightname + std::string(".");
    auto position = on ? viewMatrix * lightPosition : glm::vec4(0);

    // if one light contributes to all elements of the reflectance model, ambiant, diffuse and specular colour, otherwise none
    auto La = on ? glm::vec3(1.0f) : glm::vec3(0);
    auto Ld = on ? glm::vec3(1.0f) : glm::vec3(0);
    auto Ls = on ? glm::vec3(1.0f) : glm::vec3(0);

    shaderProgram->SetUniform(lightId + "position", viewMatrix * position); // Light position in eye coordinates
    shaderProgram->SetUniform(lightId + "La", La); // Ambient colour of light
    shaderProgram->SetUniform(lightId + "Ld", Ld); // Diffuse colour of light
    shaderProgram->SetUniform(lightId + "Ls", Ls); // Specular colour of light

    shaderProgram->SetUniform("useSpotlights", false); // be clear
    shaderProgram->SetUniform("useConstantSunlight", !m_useRealisticLighting);
}

void Game::SetSpotLighting(std::shared_ptr<GameObject> light,
    std::shared_ptr<stu::ShaderProgram> shaderProgram,
    glm::mat4 viewMatrix, 
    glm::mat3 viewNormalMatrix, 
    std::string lightname, 
    bool on,
    std::shared_ptr<GameObject> candidateForLighting)
{
    std::string lightId = lightname + std::string(".");

    // Use light's properties in shader or set shader to 0 if not using light ie it is off:

    if(on)
    {
        int i = 0;
    }
    auto position = on 
        ? viewMatrix * light->PropertiesVec4["position"]  // note multiplied by the viewMatrix to get into eye co-ordinates
        : glm::vec4(0);

    auto La = on 
        ? light->PropertiesVec3["La"] 
        : glm::vec3(0.4);

    auto Ld = on 
        ? light->PropertiesVec3["Ld"] 
        : glm::vec3(0.0);

    auto Ls = on 
        ? light->PropertiesVec3["Ls"] 
        : glm::vec3(0);

    auto direction = on
        ? glm::normalize(viewNormalMatrix * light->PropertiesVec3["direction"]) // use the view normal to perform normal transformation and void scaling issue
        : glm::vec3(0);

    auto exponent = on 
        ? light->PropertiesFloat["exponent"] 
        : 0.0f;

    auto cutoff = on 
        ? light->PropertiesFloat["cutoff"]
        : 0.0f;

    // Check how far this object is from the light in question...

    auto distance_to_light = !candidateForLighting->PropertiesBool["IsPlant"]
        ? glm::distance(candidateForLighting->GetPosition(), light->GetPosition())
        : glm::distance(glm::vec3(candidateForLighting->GetPosition().x,
                        0.0f, // we interpret the positoin of the plant as starting at y=0 but in reality it starts below surface of ground ie -y something
                        candidateForLighting->GetPosition().z), 
                        light->GetPosition());
    
    auto normalThreshold = light->PropertiesFloat["radius"];
    auto plantThreshold = 400;
    
    // If its too far, then dont light it with this light
    if(distance_to_light > normalThreshold)
    {
        // Turn off the light by simulating low light for the object 
       // candidateForLighting->shaderProgram->SendMaterialProperties(glm::vec3(1.0), glm::vec3(0.1), glm::vec3(0.0), 15.0f);
        
        // The light is 
        La = Ld = Ls = glm::vec3(0.1); // show a little light
    }

    


    shaderProgram->SetUniform(lightId + "position", position); // Light position in eye coordinates
    shaderProgram->SetUniform(lightId + "La", La);
    shaderProgram->SetUniform(lightId + "Ld", Ld);
    shaderProgram->SetUniform(lightId + "Ls", Ls);
    shaderProgram->SetUniform(lightId + "direction", direction);
    shaderProgram->SetUniform(lightId + "exponent", exponent);
    shaderProgram->SetUniform(lightId + "cutoff", cutoff);
    shaderProgram->SetUniform("useSpotlights", on);
    
}

void Game::SetPlayerAndCameraView(glm::vec3 currentPosition, glm::vec3 nextPosition, float delta, bool setCamera)
{
        stu::TNBHelper tnb(currentPosition, nextPosition);
        auto T = glm::normalize(nextPosition - currentPosition); // forward vector
        auto y = glm::vec3(0.0f, 1.0f, 0.0);
        auto N = glm::normalize(glm::cross(T, y)); // side vector
        auto B = glm::normalize(glm::cross(N, T)); // up vector

        glm::vec3 viewPoint(0);
        glm::vec3 upVector = y;
        glm::vec3 cameraPosition = currentPosition; // initially the camera position is set to each control point

        auto width = m_pCatmullRom->GetWidth();
        auto leftPoint = currentPosition - (width/2)* N;
        auto rightPoint = currentPosition + (width/2)* N;		
        auto currentView = m_pCamera->GetView();
        auto playerPosition = currentPosition;
        cameraPosition = playerPosition;
        
        
        // we want the car to move independantly of the camere - not so realistic in 1st person but helps with gameplay
        playerPosition = tnb.MoveN((player->centerDeviation * 1.3f), currentPosition);

        if(view == Game::Views::FirstPersonView || view == Game::Views::FirstPersonRearView)
        {	
            
            // move the camera a bit higher to see the track
            cameraPosition = tnb.MoveB(player->PropertiesFloat["FirstPersonB"], cameraPosition); 
            cameraPosition = tnb.MoveN(player->PropertiesFloat["FirstPersonN"], cameraPosition);
            cameraPosition = tnb.MoveT(player->PropertiesFloat["FirstPersonT"], cameraPosition);
            
            // look towards the front vector, T
            viewPoint = !useFreeView 
                ? tnb.MoveT(player->PropertiesFloat["FirstPersonViewT"] , cameraPosition) 
                : currentView;							

            if(view == Game::Views::FirstPersonRearView && !useFreeView)
            {
                // Invert view by looking down the negative T-axis				
                cameraPosition = tnb.MoveT(player->PropertiesFloat["FirstPersonRearCamT"] , cameraPosition);
                viewPoint = tnb.MoveT(player->PropertiesFloat["FirstPersonRearViewT"], cameraPosition); 
            }
            
        }
        else if(view == Game::Views::TopView)
        {
        
            cameraPosition = tnb.MoveB(player->PropertiesFloat["TopB"], currentPosition);
            viewPoint = !useFreeView 
                ? tnb.MoveT(player->PropertiesFloat["TopT"], currentPosition)
                : currentView;	
        }
        else if(view == Game::Views::ThirdPersonView)
        {
        
            cameraPosition = tnb.MoveT(player->PropertiesFloat["ThirdT"], currentPosition); // move up the T axis by 10 units
            cameraPosition = tnb.MoveB(player->PropertiesFloat["ThirdB"], cameraPosition); // move up the B axis by 10

            auto theView = tnb.MoveT(player->PropertiesFloat["ThirdViewT"], currentPosition);
            theView = tnb.MoveB(player->PropertiesFloat["ThirdViewB"], theView);
            viewPoint = !useFreeView 
                ? theView
                : currentView;	// move 5 units from the camera down T axis
        }
        else if(view == Game::Views::LeftSideView)
        {
            cameraPosition = leftPoint;
            cameraPosition = tnb.MoveN(player->PropertiesFloat["LeftN"], cameraPosition);
            cameraPosition = tnb.MoveB(player->PropertiesFloat["LeftB"], cameraPosition);
            viewPoint = currentPosition; // look at the current position on the track
        }
        else if(view == Game::Views::RightSideView)
        {
        
            cameraPosition = rightPoint;
            cameraPosition = tnb.MoveN(player->PropertiesFloat["RightN"], cameraPosition);
            cameraPosition = tnb.MoveB(player->PropertiesFloat["RightB"], cameraPosition);
            viewPoint = !useFreeView 
                ? currentPosition 
                : currentView; // look at the current position on the track			
        }

        if(setCamera)
        {
            m_pCamera->Set(cameraPosition, viewPoint, upVector);
        }


        auto matrix = glm::mat4(glm::mat3(T, B, N));
        
        
        // Update the player position
        player->SetTranslationOffset(playerPosition);
        player->SetOrientation(matrix, tnb);
}

void Game::SetPlayerView(float delta)
{
    if(MovementMode::Simulation == movementMode)
    {
        m_currentDistance += delta * m_speed;	

        glm::vec3 p; // current control point
        m_pCatmullRom->Sample(m_currentDistance, p);

        glm::vec3 pNext; //next control point
        m_pCatmullRom->Sample(m_currentDistance + 0.1f, pNext);

        SetPlayerAndCameraView(p, pNext, delta, !m_detatchCamera);
    }
}

// Update method runs repeatedly with the Render method
void Game::Update(size_t pass) 
{
    // Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
    m_pCamera->Update(m_dt);
    

    // Update our scene objects
    for(const auto &sceneEntry : scene)
    {
        auto object = sceneEntry.second;
        if(object == nullptr)
            continue;

        // Update the game object
        object->Update(m_dt);

        // Look for any collisions between player and pickups
        DetectCollisionsWithPlayer(object, collisions);		
    }

    for(auto &collision : collisions)
    {
        auto object = collision.second;
        RemoveFromScene(object);
    }
    collisions.clear();

    SetPlayerView(m_dt);

    m_pAudio->Update();
    event_admin->process_all_events();
}

void Game::DetectCollisionsWithPlayer(shared_ptr<GameObject> candidateObject, std::map<std::string, std::shared_ptr<GameObject>> collisions)
{
    // check for colliions with player
    auto name = candidateObject->GetName();
    auto has_name = name.size() > 0;

    if(candidateObject->GetType() == GameObject::GameObjectType::Player)
        return; // dont want to detect collisions with ourself

    if(candidateObject->GetType() == GameObject::GameObjectType::Pickup)
    {
        // check if the player has collided with the object
        if(player->CollidedWith(candidateObject, m_speed))
        {			
            event_admin->raise_event(std::make_shared<PlayerCollisionEvent>(candidateObject));	 
        }
    }
}

// Handle events coming from the event manager
std::vector<std::shared_ptr<gamelib::event>> Game::handle_event(std::shared_ptr<gamelib::event> event)
{
    std::vector<std::shared_ptr<gamelib::event>> generatedEvents; // we might generated events that we want to put onto the event queue...
     
    
    // Handle subscribed events
    if(event->type == gamelib::event_type::GameObject)
    {
        const auto gameObjectEvent = dynamic_pointer_cast<gamelib::GameObjectEvent>(event);
        switch(gameObjectEvent->context)
        {
            case gamelib::GameObjectEventContext::TopView:
                view = Game::Views::TopView;
                m_isMultiPass = false; 
                break;
            case gamelib::GameObjectEventContext::FirstPersonView:
                view = Game::Views::FirstPersonView;
                m_isMultiPass = true;  // we turn on the rear view mirror for 1st person mode
                break;
            case gamelib::GameObjectEventContext::ThirdPersonView:
                view = Game::Views::ThirdPersonView;
                m_isMultiPass = false; 
                break;
            case gamelib::GameObjectEventContext::LeftSideView:
                view = Game::Views::LeftSideView;
                m_isMultiPass = false; 
                break;
            case gamelib::GameObjectEventContext::RightSideView:
                view = Game::Views::RightSideView;
                m_isMultiPass = false; 
                break;
            case gamelib::GameObjectEventContext::UseCatmulRom:
                movementMode = movementMode == MovementMode::Simulation
                    ? MovementMode::FreeCamera 
                    : MovementMode::Simulation;

                if(movementMode == MovementMode::FreeCamera)
                    m_isMultiPass = false;
            break;
            case gamelib::GameObjectEventContext::FreePlayerWithCamera:
                movementMode = movementMode == MovementMode::FreePlayerWithCamera
                    ? MovementMode::FreeCamera 
                    : MovementMode::FreePlayerWithCamera;
            break;
            case gamelib::GameObjectEventContext::FreeView:
                useFreeView = !useFreeView;
            break;
            case gamelib::GameObjectEventContext::Level1:
                /* Not implemented */
                break;
            case gamelib::GameObjectEventContext::Level2:
                /* Not implemented */
                break;
            case gamelib::GameObjectEventContext::MoveUp:
            case gamelib::GameObjectEventContext::IncreaseSpeed:
                    m_speed += speed_factor * 30/m_dt;
                break;
            case gamelib::GameObjectEventContext::MoveDown:
            case gamelib::GameObjectEventContext::DecreaseSpeed:
                    m_speed -= speed_factor  * 30/m_dt;				
                break;
            case gamelib::GameObjectEventContext::MainSceneLighting:
                m_sunlightOn = !m_sunlightOn;
                if(!m_sunlightOn)
                    m_useFog = true;
                break;
            case gamelib::GameObjectEventContext::Spotlight1Lighting:
                m_useSpotlight1 = !m_useSpotlight1;
                SetSkylightsActiveStatus(spotlight1, m_useSpotlight1);
                break;
            case gamelib::GameObjectEventContext::Spotlight2Lighting:
                m_useSpotlight2 = !m_useSpotlight2;
                SetSkylightsActiveStatus(spotlight2, m_useSpotlight2);
                break;
            case gamelib::GameObjectEventContext::DetachCamera:
                m_detatchCamera = !m_detatchCamera;
                break;
            case gamelib::GameObjectEventContext::RealisticLighting:
                m_useRealisticLighting = !m_useRealisticLighting;
                break;
            case gamelib::GameObjectEventContext::MultiPass:
                m_isMultiPass = !m_isMultiPass;
                break;
            case gamelib::GameObjectEventContext::UseFog:
                m_useFog = !m_useFog;
            break;
            case gamelib::GameObjectEventContext::ChangeVehicle:
                useNewVehicle = !useNewVehicle;
                if(useNewVehicle){
                    player = jeep;
                    audi->SetActive(false);
                    jeep->SetActive(true);
                }
                else
                {
                    player = audi;
                    audi->SetActive(true);
                    jeep->SetActive(false);
                }
                break;

        }
    }

    // If we collided with a pickup, then add it to the list of objects that we have collided with in this frame (we'll remove them in Update())
    if(gamelib::event_type::PlayerCollision == event->type)
    {
        const auto positionChangedEvent = std::dynamic_pointer_cast<PlayerCollisionEvent>(event);

        auto collidedWith = positionChangedEvent->collider;
        auto isAlreadyCollided = collisions.find(collidedWith->GetName());
        auto notFound = isAlreadyCollided == collisions.end();

        if(notFound /*collision not recorded yet*/)
        {
            collisions[collidedWith->GetName()] = collidedWith;	
            // hey, you got a point
            //auto point_value = collidedWith->PropertiesInt[Player::WellKnownPropertyKeys[Player::PickupProperties::PickupValue]];
            //numPoints += point_value;
        }
    }

    return generatedEvents;
}

void Game::SetSkylightsActiveStatus(shared_ptr<GameObject> spotlight, bool active)
{
    for(auto &item : scene)
    {
        if(item.second->PropertiesString["BelongsTo"] == spotlight->GetName())
            item.second->SetActive(active);
    }
}

void Game::DisplayFrameRate()
{

    CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

    RECT dimensions = m_gameWindow.GetDimensions();
    int height = dimensions.bottom - dimensions.top;

    // Increase the elapsed time and frame counter
    m_elapsedTime += m_dt;
    m_frameCount++;

    // Now we want to subtract the current time by the last time that was stored
    // to see if the time elapsed has been over a second, which means we found our FPS.
    if (m_elapsedTime > 1000)
    {
        m_elapsedTime = 0;
        m_framesPerSecond = m_frameCount;

        // Reset the frames per second
        m_frameCount = 0;
    }

    if (m_framesPerSecond > 0) {
        // Use the font shader program and render the text
        fontProgram->UseProgram();
        glDisable(GL_DEPTH_TEST);
        fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
        fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());

        // Default colour
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        
        // FPS
        auto fps_x = 20;
        auto fps_y = height - 20;
        auto pixelSize = 20;
        m_pFtFont->Render(fps_x, fps_y, pixelSize, "FPS:", "");
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        m_pFtFont->Render(fps_x+50, fps_y, pixelSize, "%d", m_framesPerSecond);
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        
        // Points
        auto points_x = dimensions.right - 60;
        auto points_y = dimensions.top+10;	
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        m_pFtFont->Render(points_x, points_y, pixelSize*2, "%d", player->GetPoints());
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        
        // Speed
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        auto speed_x = dimensions.left + 90;
        auto speed_y = dimensions.top+10;
        m_pFtFont->Render(speed_x, speed_y, pixelSize, "Speed:", "");
        fontProgram->SetUniform("vColour", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        m_pFtFont->Render(speed_x+80, speed_y, pixelSize, "%.2f", m_speed);
        

        // My objects
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        auto objects_x = 20;
        auto objects_y = height - 40;
        m_pFtFont->Render(objects_x, objects_y, pixelSize, "Objects:", "");
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        m_pFtFont->Render(objects_x+110, objects_y, pixelSize, "%d", scene.size());

        // Lap
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        auto lap_x = dimensions.left + 10;
        auto lap_y = dimensions.top+10;
        m_pFtFont->Render(lap_x, lap_y, pixelSize, "Lap:", "");
        fontProgram->SetUniform("vColour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        m_pFtFont->Render(lap_x+50, lap_y, pixelSize, "%d", m_pCatmullRom->CurrentLap(m_currentDistance));
        
    }
        glEnable(GL_DEPTH_TEST);
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
    /*
    // Fixed timer
    dDt = pHighResolutionTimer->Elapsed();
    if (dDt > 1000.0 / (double) Game::FPS) {
        pHighResolutionTimer->Start();
        Update();
        Render();
    }
    */
    
    
    // Variable timer
    m_pHighResolutionTimer->Start();
    Update();
    Render();
    m_dt = m_pHighResolutionTimer->Elapsed();
    

}

WPARAM Game::Execute() 
{
    m_pHighResolutionTimer = new CHighResolutionTimer;
    m_gameWindow.Init(m_hInstance);

    if(!m_gameWindow.Hdc()) {
        return 1;
    }

    Initialise();

    m_pHighResolutionTimer->Start();

    
    MSG msg;

    while(1) {													
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
            if(msg.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&msg);	
            DispatchMessage(&msg);
        } else if (m_appActive) {
            GameLoop();
        } 
        else Sleep(200); // Do not consume processor power if application isn't active
    }

    m_gameWindow.Deinit();

    return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
    LRESULT result = 0;

    switch (message) {


    case WM_ACTIVATE:
    {
        switch(LOWORD(w_param))
        {
            case WA_ACTIVE:
            case WA_CLICKACTIVE:
                m_appActive = true;
                m_pHighResolutionTimer->Start();
                break;
            case WA_INACTIVE:
                m_appActive = false;
                break;
        }
        break;
        }

    case WM_SIZE:
            RECT dimensions;
            GetClientRect(window, &dimensions);
            m_gameWindow.SetDimensions(dimensions);
        break;

    case WM_PAINT:
        PAINTSTRUCT ps;
        BeginPaint(window, &ps);
        EndPaint(window, &ps);
        break;

    case WM_KEYDOWN:
        switch(w_param) 
        {
            case VK_ESCAPE:
                PostQuitMessage(0);
                break;
            case '0':
                m_pAudio->PlayEventSound();
                break;
            // first person view
            case '1':
                event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::FirstPersonView));
            break;
             // 3rd person view
            case '2':
                event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::ThirdPersonView));
            break;
            case '3':
                event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::LeftSideView));
            break;
            case '4':
                event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::RightSideView));
            break;
            case '5':
                event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::TopView));
            break;
            // level 1
            case VK_F1:
                //m_pAudio->PlayEventSound();
                event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::Level1));
                break;
            // level 2
            case VK_F2:	
                event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::Level2));
                break;
            // level 3
            case VK_F3:	
                event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::Level3));
            break;
            // Misc keyboard ipnut events
            default:
                if( GetKeyState('T') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::TopView));				
                if( GetKeyState('L') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::LeftSideView));				
                if( GetKeyState('R') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::RealisticLighting));				
                if(GetKeyState('C') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::UseCatmulRom));	
                if(GetKeyState('P') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::FreePlayerWithCamera));				
                if(GetKeyState('F') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::FreeView));				
                if(GetKeyState('H') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::TestEvent));				
                if( GetKeyState('Y') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::MainSceneLighting));	
                if( GetKeyState('U') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::Spotlight1Lighting));	
                if( GetKeyState('I') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::Spotlight2Lighting));				
                if (GetKeyState(VK_UP) & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::MoveUp));	
                if (GetKeyState(VK_DOWN) & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::MoveDown));
                if (GetKeyState(VK_LEFT) & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::MoveLeft));
                if (GetKeyState(VK_RIGHT) & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::MoveRight));				
                if (GetKeyState('X') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::DetachCamera));
                if (GetKeyState('Q') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::MultiPass));
                if (GetKeyState('G') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::UseFog));
                if (GetKeyState('V') & 0x80)
                    event_admin->raise_event(std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::ChangeVehicle));
                break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:		
        //event_admin->process_all_events();
        result = DefWindowProc(window, message, w_param, l_param);
        break;
    }

    return result;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
    m_hInstance = hinstance;
}

std::shared_ptr<Game> game = std::make_shared<Game>();

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    return game->ProcessEvents(window, message, w_param, l_param);
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{	
    game->SetHinstance(hinstance);
    return game->Execute();
}

std::string Game::get_subscriber_name()
{
    return "Game";
}

int Game::get_subscriber_id()
{
    return 1;
}
