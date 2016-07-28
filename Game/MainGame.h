#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include "GLSLProgram.h"

#include "Sprite.h"
#include "GLTexture.h"

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

	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;

	GameState _gameState;

	float _mouseX;
	float _mouseY;

	std::vector<Sprite*> _sprites;

	GLSLProgram _colorProgram;

	float _time;

	float _fps;
	float _frameTime;

	float _maxFPS;
};

