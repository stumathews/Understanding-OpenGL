#pragma once

#include "Common.h"
#include "GameWindow.h"
#include "stulib\ShaderProgram.h"
#include "freeimage\FreeImage.h"
#include <functional>
#include <map>


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

class Game
{
private:

	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();	
	void Update();
	void Render();
	CHighResolutionTimer *m_pHighResolutionTimer;

	std::shared_ptr<stu::ShaderProgram> mainShaderProgram;

	
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();


private:
	static const int FPS = 60;

	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
};
