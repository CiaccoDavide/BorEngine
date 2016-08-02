#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <BorEngine\BorEngine.h>
#include <BorEngine\GLSLProgram.h>
#include <BorEngine\Sprite.h>
#include <BorEngine\GLTexture.h>
#include <BorEngine\Window.h>
#include <BorEngine\Camera2D.h>

#include <vector>

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

	std::vector<BorEngine::Sprite*> _sprites;

	BorEngine::GLSLProgram _colorProgram;

	float _time;

	float _fps;
	float _frameTime;

	float _maxFPS;

	BorEngine::Camera2D _camera;
};
