
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
GLuint buffer;
GLuint buffer2;

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{        
    RECT dimensions = m_gameWindow.GetDimensions();

    int width = dimensions.right - dimensions.left;
    int height = dimensions.bottom - dimensions.top;
    
    // ========================
    // = START Rendering Code =
    // ========================

    // glVertexArrayAttribBinding() - Set the vertex buffer binding that the vertex attribute uses to reference a buffer. Each vertex attribute fetches its data from a buffer bound to one of several vertex buffer bindings.
    // glVertexArrayVertexBuffer() - Tell OpenGL which buffer is associated with binding, and where in it is our data
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
        "#version 450 core                                                                                                      \n"
        "                                                                                                                       \n"
        "// 'offset' and 'color' are input vertex attributes - they will automatically be filled in by the Vertex Fetch Stage   \n"
        "layout (location = 0) in vec4 position;                                                                                \n"
        "layout (location = 1) in vec4 color;                                                                                   \n"
        "// All these global uniforms live in the 'default block':                                                              \n"
        "layout (location = 0) uniform float fTime;                                                                             \n"
        "layout (location = 1) uniform int iIndex;                                                                              \n"
        "layout (location = 2) uniform vec4 vColorValue;                                                                        \n"
        "layout (location = 3) uniform bool bSomeFlag;                                                                          \n"
        "layout (location = 4) uniform vec4 vColors[3];                                                                         \n"
        "                                                                                                                       \n"
        "// Our first Named uniform block. This will be backed by a buffer object. This uses the easier standard layout         \n"
        "layout(std140) uniform MyUniformBlock                                                                                  \n"
        "{                                                                                                                      \n"
        "  float scale;                                                                                                         \n"
        "  vec3 translation;                                                                                                    \n"
        "  float roation[3];                                                                                                    \n"
        "  mat4 projection_matrix;                                                                                              \n"
        "} myBlock;                                                                                                             \n"
        "                                                                                                                       \n"
        "// declare an interface block as output                                                                                \n"
        "out VS_OUT                                                                                                             \n"
        "{                                                                                                                      \n"
        "   vec4 color;                                                                                                         \n"
        "} vs_out;                                                                                                              \n"
        "                                                                                                                       \n"
        "void main(void)                                                                                                        \n"
        "{                                                                                                                      \n"      
        "  gl_Position = position;                                                                                              \n"
        "  // Output a fixed value to fragment shader                                                                           \n"
        "  vs_out.color = vColors[gl_VertexID];                                                                                 \n"
        "}                                                                                                                      \n"
    };

    // Define the source of the fragment shader
    static const GLchar* fragment_shader_source[] = 
    {
      // Sets the vertex color for each processed vertex fragment to same color
      "#version 450 core\n" // Use version 4.5 of the shader language and use the core profile of OpenGL
        "\n"
        "out vec4 color;\n"
        "\n"
        "// Declare VS_OUT as an input interface block (VS_OUT matches VS_OUT interface block name in vertex shader)\n"
        "// but we can use a diffirent instance name makes it possible to call it fs_in\n"
        "in VS_OUT\n"
        "{\n"
        "   vec4 color;\n"
        "} fs_in;\n"
        "void main(void)\n"
        "{\n"
        "  color = fs_in.color;\n"
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

    // Our vertex data structure - that is the basis of our interleaved data

    struct vertex
    {
        float x;
        float y;
        float z;
        float w;

        // color
        float r;
        float g;
        float b;
        float a;
    
    } vertexStructure;
        
    // This is the vertex data (interleaved) that we are going to upload into the buffer object (Array of Structures)
    static const vertex vertexData[] = 
    {
        vertex { 0.25,   -0.25,   0.5,   1.0,   1.0,   0.0,   0.0,   1.0 }, // vertex1, red
        vertex { -0.25,  -0.25,   0.5,   1.0,   0.0,   1.0,   0.0,   1.0 }, // vertex2, blue
        vertex { 0.25,    0.25,   0.5,   1.0,   0.0,   0.0,   1.0,   1.0 }, // vertex3, green
    };

    // Alternatively, could use this data as a is the same as array of structures above
    static const float allVertexData[] = 
    {
        // =======================================================
        //        vec4(x,y,z,w)     |      vec4(r,g,b,a)
        // =======================================================

        
        //=======================================================
        //x      y       z      w   |  r      g      b      a 
        //======================================================
        0.25,   -0.25,   0.5,   1.0,   1.0,   0.0,   0.0,   1.0,
        -0.25,  -0.25,   0.5,   1.0,   0.0,   1.0,   0.0,   1.0,
        0.25,    0.25,   0.5,   1.0,   0.0,   0.0,   1.0,   1.0 
    };
    
    // ============================================
    // One Buffer for all vertex Attribute's storage  //
    // =============================================

    // Create an arbitary buffer object in memory
    glCreateBuffers(1, &buffer);

    // Allocate space for buffer
    auto dataStoreSize = sizeof(vertexData); // size of the allocation
    auto initialData = nullptr; /// no initial data (otherwise pointer to initial data)
    auto intendedUse = GL_MAP_WRITE_BIT;
    glNamedBufferStorage(buffer, dataStoreSize, initialData, intendedUse);
    
    // Make the buffer available (bind it to context)
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // note this buffer can be re-binded to another binding point/target
    
    // Get pointer to buffer & copy vertexData to it
    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, dataStoreSize, GL_MAP_WRITE_BIT);
    if(ptr)        
        memcpy(ptr, vertexData, sizeof(vertexData)); // copy our vertex data directly into the buffer on the GPU!
    else
        SayHello("Bad range");

    // Tell OpenGL that we're done with the pointer
    glUnmapNamedBuffer(buffer);

    /// =================================================================== \\\
    /// Vertex Attribute Buffer Binding - tieing the vertex buffer with vao \\\
    /// =================================================================== \\\

    auto vertexBufferBinding = 12; // This becomes the binding between VAO and buffer, ie it is the 'VertexBufferBinding' (12 is arbitary binding index)
    auto vertexSize = sizeof(glm::vec4) *2; // Expected size of each vertex in the buffer, ie two vec4s side by side eg: position, color
    auto start_within_buffer = 0;        // Of all the vertices in the vertex buffer, start at the first vertex
    glVertexArrayVertexBuffer(vao, vertexBufferBinding, buffer, start_within_buffer, vertexSize);
    
    /// =========================== \\\
    /// 1st Vertex Attribute Format \\\
    /// =========================== \\\

    auto vertexAttributeIndex = 0; // first vertex attribute if what we'll describe here ie: layout (location = 0) in vec4 position;
    auto numComponents = 4;        // How many components make up one item in this vertex attribute? 4 floats ie x, y, z, w
    auto componentType = GL_FLOAT; // each component is a float
    auto firstElementOfVertex = 0 ; 
    glVertexArrayAttribFormat( vao, vertexAttributeIndex, numComponents, componentType, GL_FALSE, firstElementOfVertex);
    // link vertex array attribute 0 with the vertex buffer via vertex buffer binding
    glVertexArrayAttribBinding(vao, vertexAttributeIndex, vertexBufferBinding);
    glEnableVertexArrayAttrib(vao, vertexAttributeIndex);
 
    /// ============================
    /// 2nd Vertex Attribute Format
    /// ============================
    
    vertexAttributeIndex = 1; // first vertex attribute if what we'll describe here ie: layout (location = 0) in vec4 position;
    numComponents = 4;        // How many components make up one item in this vertex attribute? 4 floats ie r, g, b, a
    componentType = GL_FLOAT; // each component is a float
    firstElementOfVertex = offsetof(vertex, r); // this is cool but could have done sizeof(float) * 4
    glVertexArrayAttribFormat( vao, vertexAttributeIndex, numComponents, componentType, GL_FALSE, firstElementOfVertex);
    // link vertex array attribute 0 with the vertex buffer via vertex buffer binding
    glVertexArrayAttribBinding(vao, vertexAttributeIndex, vertexBufferBinding); // you could associate this vertex attribute with another buffer, ie switch it out (assuming the data in the buffer is in the same format at the vertex attribute)
    glEnableVertexArrayAttrib(vao, vertexAttributeIndex);
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

    const GLfloat vColors[] = 
    {
         1.0f, 1.0f, 1.0f, 1.0f, // numVector1
         1.0f, 0.0f, 0.0f, 1.0f  // numVector2
    };

    GLfloat vColors2[3][4] = 
    {
        {1.0f, 0.0f, 0.0f, 1.0f}, // numVector1
        {0.0f, 1.0f, 0.0f, 1.0f},  // numVector2
        {0.0f, 0.0f, 1.0f, 1.0f} // numVector3
    };

    glm::mat4 myMat4 = 
    {
        {1.0f, 0.0, 0.0, 1.0},
        {0.0f, 1.0, 0.0, 1.0},
        {0.0f, 0.0, 1.0, 1.0},
        {0.0f, 0.0, 0.0, 1.0}
    };

    glm::mat4 myMat42[] = 
    {
        // mat[0]
        {{1.0f, 0.0, 0.0, 1.0},
        {0.0f, 1.0, 0.0, 1.0},
        {0.0f, 0.0, 1.0, 1.0},
        {0.0f, 0.0, 0.0, 1.0}},
        // mat[1]
        {{1.0f, 0.0, 0.0, 1.0},
        {0.0f, 1.0, 0.0, 1.0},
        {0.0f, 0.0, 1.0, 1.0},
        {0.0f, 0.0, 0.0, 1.0}}
    };

    glm::mat4 myMat43[] = 
    {
        // mat[0]
        {myMat4},
        // mat[1]
        {myMat4}
    };

    glClearBufferfv(GL_COLOR, 0, red);  

    // Use our shaders that are part of the program
    glUseProgram(program);

    // Set uniforms...

    // Set 1 float uniform value
    GLfloat fortyTwo = 42.0f;
    glUniform1f(0, fortyTwo);
    glUniform1fv(0, 1, &fortyTwo);

    // Set 1 integer uniform value
    glUniform1i(1, 42);

    // Set 4 float uniform values
    // glUniform4f(2, 1.0f, 0.0f, 0.0f, 1.0f);
    auto numVectors = 1;
    glUniform4fv(2, numVectors, green);
    
    // Set 1 integer (boolean) uniform value
    glUniform1i(3, GL_FALSE);

    // Set 3, 4-float uniform vectors...
    numVectors = 3;
    glUniform4fv(4, numVectors, (float*)vColors2);

    // Set matrix uniform value
    auto numMatrices = 1;
    glUniformMatrix4fv(5, numMatrices, GL_FALSE, (float*) &myMat4);

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
