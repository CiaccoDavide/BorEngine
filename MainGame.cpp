#include <iostream>
#include <string>
#include "MainGame.h"
#include "Sprite.h"
#include "Errors.h"

MainGame::MainGame() :
	_window(nullptr),
	_screenWidth(480),
	_screenHeight(320),
	_gameState(GameState::PLAY),
	_time(0)
{



}


MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	_sprite.init(0.0f, 0.0f, 1.9f, 1.9f); // just testing

	gameLoop();
}

void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	_window = SDL_CreateWindow("Bor Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);

	if (_window == nullptr)
	{
		fatalError("SDL window could not be created!");
	}

	// we need an openGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr)
	{
		fatalError("SDL_GL context could not be created!");
	}

	if (glewInit() != GLEW_OK)
	{
		fatalError("Could not initialize Glew!");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // against flickering!

	glClearColor(0.569f, 0.204f, 0.192f, 1.0f); // set the background color, will be applied whenever GL_COLOR_BUFFER_BIT will be called

	initShaders();
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		_time += 0.001; // arbitrary time for now...
		drawGame();
	}
}
void MainGame::processInput()
{
	SDL_Event evnt;
	// mettiamo & perche' vuole ricevere un pointer!
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			_mouseX = evnt.motion.x;
			_mouseY = evnt.motion.y;
			break;
		}
	}
}

void MainGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // bit-wise or | 

	_colorProgram.use();

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	_sprite.draw();

	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window); // for swapping the double buffer (against flickering)
}