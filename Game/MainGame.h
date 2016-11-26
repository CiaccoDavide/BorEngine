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
#include <BorEngine\AudioEngine.h>
#include <BorEngine\ParticleEngine2D.h>
#include <BorEngine\ParticleBatch2D.h>

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
	//void updateClickParticles(BorEngine::Particle2D& particle, float deltaTime);
	BorEngine::Window _window;
	int p_screenWidth = 720;
	int p_screenHeight = 480;

	GameState p_gameState = GameState::PLAY;

	float p_mouseX;
	float p_mouseY;
	BorEngine::InputManager p_inputManager;

	BorEngine::FPSLimiter p_fpsLimiter;
	float p_time = 0;
	float p_fps;
	float p_maxFPS = 60000.0f;

	BorEngine::GLSLProgram p_colorProgram;
	BorEngine::Camera2D p_camera;
	BorEngine::Camera2D p_hudCamera;
	BorEngine::SpriteBatch p_spriteBatch;

	BorEngine::SpriteBatch p_hudSpriteBatch;
	BorEngine::SpriteFont* p_spriteFont;

	BorEngine::AudioEngine p_audioEngine;
	BorEngine::SoundEffect p_soundEffect;

	char p_textBuffer[256];
	int p_updatesCount = 0;

	BorEngine::ParticleEngine2D p_particleEngine;
	BorEngine::ParticleBatch2D* p_clickedParticleBatch;
};
