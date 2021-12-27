
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
GLuint vertex_shader;
GLuint fragment_shader;
GLuint program;

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{        
    RECT dimensions = m_gameWindow.GetDimensions();

    int width = dimensions.right - dimensions.left;
    int height = dimensions.bottom - dimensions.top;
    
    // ========================
    // = START Rendering Code =
    // ========================

    // glCreateShader() - creates a shader object in memory
    // glShaderSource() - uploads source to the shader object
    // glCompileShader() - compiles the shader object in memory (requires source to have been uploaded)
    // glCreateProgram() - create program object in memory
    // glAttachShader() - attaches a shader to the program object
    // glLinkProgram() - links the program object
    // glDeleteShader() - deletes the shader object
    // glDeleteProgram() - deletes the program
    // glCreateVertexArrays() - creates a vertex array object (vao) (this represents the vertex fetch stage of the pipeline)
    // glBindVertexArray() - binds the vertex array object to the current OpenGL context
    
    // Define the source of the vertex shader
    static const GLchar* vertext_shader_source[] =
    {
        // Sets the position of each processed vertex to same position, right in the middle of clip space.
        //
        //                   1.0
        //                         
        //         -1.0     (0.0)      1.0
        //
        //
        //                  -1.0
        //
        // Note: Everytime we send a vertex with glDrawArrays, this vertex shader is run once
        "#version 450 core\n"
        "\n"
        "// 'offset' and 'color' are input vertex attributes - they will automatically be filled in by the Vertex Fetch Stage\n"
        "layout (location = 0) in vec4 offset;\n"
        "layout (location = 1) in vec4 color;\n"
        "\n"
        "// declare an output\n"
        "out vec4 vs_color;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "  const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),\n"
        "                                   vec4(-0.25, -0.25, 0.5, 1.0),\n"
        "                                   vec4(0.25, 0.25, 0.5, 1.0));\n"
        "  // Add offset' to our hard-coded vertex position\n"
        "  gl_Position = vertices[gl_VertexID] + offset;\n"
        "  // Output a fixed value to fragment shader\n"
        "  vs_color = color;"
        "}\n"
    };

    // Define the source of the fragment shader
    static const GLchar* fragment_shader_source[] = 
    {
      // Sets the vertex color for each processed vertex fragment to same color
      "#version 450 core\n" // Use version 4.5 of the shader language and use the core profile of OpenGL
        "\n"
        "out vec4 color;\n"
        "\n"
        "// input from the vertex shader\n"
        "in vec4 vs_color;\n"
        "void main(void)\n"
        "{\n"
        "  color = vs_color;\n"
        "}\n"
    };

    std::string message;
    
    

    // Create and compile vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertext_shader_source, nullptr);
    glCompileShader(vertex_shader);

    // Create and compile fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    // Create the program, attach shaders to it and link it
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    // Delete the shaders as the program has them now
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

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

    const GLfloat attrib[] = 
    {
        (float) sin(m_dt) * 0.5f,
        (float) cos(m_dt) * 0.6f,
        0.0f, 0.0f    
    };

    auto positionAttributeLayoutNumber = 0;
    auto colorAttributeLayoutNumber = 1;

    // Updates the vaue of the input attribute 0 each time we call it
    glVertexAttrib4fv(positionAttributeLayoutNumber, attrib);

    // Update the value of the input attribute 1 (color) each time we call it
    glVertexAttrib4fv(colorAttributeLayoutNumber, blue);

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
