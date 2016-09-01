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
	void initShaders();
	void calculateFPS();

	BorEngine::Window _window;
	int _screenWidth;
	int _screenHeight;

	GameState _gameState;

	float _mouseX;
	float _mouseY;
	BorEngine::InputManager _inputManager;

	float _fps;
	float _maxFPS;
	float _time;
	float _frameTime;

	BorEngine::GLSLProgram _colorProgram;
	BorEngine::Camera2D _camera;
	BorEngine::SpriteBatch _spriteBatch;
};
