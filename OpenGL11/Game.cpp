
#include "game.h"
#include "stulib2\TestClass.h"
#include "cgtlib\HighResolutionTimer.h"
#include "GameWindow.h"
#include "gamelib\Functions.h"
#include "stulib\TestClass.h"
#include "Camera.h"
#include <memory>
#include <algorithm>
#include "stulib\ShaderErrorHandling.h"

std::shared_ptr<Game> game = std::make_shared<Game>();

Game::Game()
{     
    m_pHighResolutionTimer = NULL;  
    m_dt = 0.0;
    m_framesPerSecond = 0;
    m_frameCount = 0;
    m_elapsedTime = 0.0f;

}

GLuint vao;

GLuint program;
GLuint compute_shader;

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{        
    RECT dimensions = m_gameWindow.GetDimensions();

    int width = dimensions.right - dimensions.left;
    int height = dimensions.bottom - dimensions.top;
    
    // ========================
    // = START Rendering Code =
    // ========================

    static const GLchar* compute_shader_source[] = 
    {
        "#version 450 core \n"
        "layout (local_size_x = 32, local_size_y = 32) in;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "   // Do nothing!\n"
        "}\n"
    };

    std::string message;

    // Create ad compile the compute shader
    compute_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute_shader, 1, compute_shader_source, nullptr);
    glCompileShader(compute_shader);

    if(stu::ShaderErrorHandling::is_gl_error(GL_COMPILE_STATUS, message, compute_shader, false))
        SayHello(message.c_str());

    // Create the program, attach shaders to it and link it
    program = glCreateProgram();
    glAttachShader(program, compute_shader);
    glLinkProgram(program);

    if(stu::ShaderErrorHandling::is_gl_error(GL_LINK_STATUS, message, program, true))
        SayHello(message.c_str());

    // Delete the shaders as the program has them now
    glDeleteShader(compute_shader);

    // Create the vertex array object which represents the Vertex Fetch Stage in the OpenGL pipeline. This is required in order to draw anything.
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao); // Let OpenGL know we want to use the vertex array object, by binding it to the context

    // Note about vertex array objects: They maintain the state related to the input to the OpenGL pipeline.
}


Game::~Game() 
{ 
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
    delete m_pHighResolutionTimer;
}


/// <summary>
/// Render method runs repeatedly in a loop
/// </summary>
void Game::Render() 
{	
    static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat green[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat blue[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, red);  

    // Use our shaders that are part of the program
    glUseProgram(program);

    // Send vertices into the OpenGL pipeline
    auto numVerticesToSend = 3; // This will invoke our vertex shader 3 times

    // What type of primative do we want to draw? Draw Traingles this time
    auto mode = GL_TRIANGLES;

    // OpenGL Drawing command
    glDrawArrays(mode, 0, numVerticesToSend);
        
    // Show
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
