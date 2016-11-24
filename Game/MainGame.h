#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <BorEngine\BorEngine.h>
#include <BorEngine\GLSLProgram.h>
#include <BorEngine\Sprite.h>
#include <BorEngine\GLTexture.h>
#include <BorEngine\Window.h>
#include <BorEngine\Camera2D.h>
#include <BorEngine\SpriteBatch.h>

#include <vector>

#include <BorEngine\InputManager.h>
#include <BorEngine\Timing.h>

#include <BorEngine\SpriteFont.h>

enum class GameState {
	PLAY,
	EXIT
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void gameLoop();
	void processInput();
	void drawGame();
	void drawHUD();
	void initShaders();
	void writeText(const char* str, glm::vec2 position, glm::vec2 scale, BorEngine::ColorRGBA8 color, BorEngine::Justification alignment);
	void writeText(const char* str, int data, glm::vec2 position, glm::vec2 scale, BorEngine::ColorRGBA8 color, BorEngine::Justification alignment);

	BorEngine::Window _window;
	int p_screenWidth = 1080;
	int p_screenHeight = 640;

	GameState p_gameState = GameState::PLAY;

	float p_mouseX;
	float p_mouseY;
	BorEngine::InputManager p_inputManager;

	BorEngine::FPSLimiter p_fpsLimiter;
	float p_time = 0;
	float p_fps;
	float p_maxFPS = 60.0f;

	BorEngine::GLSLProgram p_colorProgram;
	BorEngine::Camera2D p_camera;
	BorEngine::Camera2D p_hudCamera;
	BorEngine::SpriteBatch p_spriteBatch;

	BorEngine::SpriteBatch p_hudSpriteBatch;
	BorEngine::SpriteFont* p_spriteFont;

	char p_textBuffer[256];
	int p_updatesCount = 0;
};
