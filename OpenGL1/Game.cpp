#include "game.h"
#include "cgtlib\HighResolutionTimer.h"
#include "GameWindow.h"
#include "gamelib\Functions.h"
#include "stulib\TestClass.h"
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

std::shared_ptr<Game> game = std::make_shared<Game>();

Game::Game()
{     
    m_pHighResolutionTimer = NULL;  
    m_dt = 0.0;
    m_framesPerSecond = 0;
    m_frameCount = 0;
    m_elapsedTime = 0.0f;
}

Game::~Game() 
{ 
    delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
        
    RECT dimensions = m_gameWindow.GetDimensions();

    int width = dimensions.right - dimensions.left;
    int height = dimensions.bottom - dimensions.top;
    
    glEnable(GL_CULL_FACE);    
}

/// <summary>
/// Render method runs repeatedly in a loop
/// </summary>
void Game::Render() 
{	
    static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat green[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat blue[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, green);
        

    SwapBuffers(m_gameWindow.Hdc());
}

/// <summary>
/// Update method runs repeatedly with the Render method
/// </summary>
void Game::Update() { }

/// <summary>
/// The game loop runs repeatedly until game over
/// </summary>
void Game::GameLoop()
{
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
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
            if(msg.message == WM_QUIT) 
            {
                break;
            }

            TranslateMessage(&msg);	
            DispatchMessage(&msg);
        } 
        else if (m_appActive) 
        {
            GameLoop();
        } 
        else 
        {
            Sleep(200); // Do not consume processor power if application isn't active
        }
    }

    m_gameWindow.Deinit();

    return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
    LRESULT result = 0;

    switch (message) 
    {
        case WM_ACTIVATE:        
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
                    // do something
                    break;           
                default:
                   break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            result = DefWindowProc(window, message, w_param, l_param);
            break;
    }

    return result;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
    m_hInstance = hinstance;
}



LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    return game->ProcessEvents(window, message, w_param, l_param);
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{	
    game->SetHinstance(hinstance);
    return game->Execute();
}
