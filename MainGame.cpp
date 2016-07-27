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

	float spritePixelSize_width = 210;
	float spritePixelSize_height = 210;

	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.5f, 0.0f, 2.0f * spritePixelSize_width / (float)_screenWidth, 2.0f * spritePixelSize_height / (float)_screenHeight, "./Textures/test_png_icon/icon.png"); // just testing

	_sprites.push_back(new Sprite());
	_sprites.back()->init(-0.5f, 0.0f, 1.0f * spritePixelSize_width / (float)_screenWidth, 1.0f * spritePixelSize_height / (float)_screenHeight, "./Textures/test_png_icon/icon.png");


	for (int i = 0; i < 1000; i++)
	{
		_sprites.push_back(new Sprite());
		_sprites.back()->init(-0.9f+0.001f*i, -0.8f + 0.001f*i, 1.0f * spritePixelSize_width / (float)_screenWidth + 0.001f*i, 1.0f * spritePixelSize_height / (float)_screenHeight + 0.001f*i, "./Textures/test_png_icon/icon.png");
	}


	//_playerTexture = ImageLoader::loadPNG("./Textures/test_png_icon/icon.png");
	//_playerTexture = ImageLoader::loadPNG("./Textures/test_png_icon/icon_transparency.png");
	//_playerTexture = ImageLoader::loadPNG("./Textures/test_png_icon/icon_transparency_fades.png");

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
	_colorProgram.addAttribute("vertexUV");

	_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		_time += 0.001f; // arbitrary time for now...
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
			std::cout << "\r Mouse Position: " << evnt.motion.x << " " << evnt.motion.y;
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

	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorProgram.getUniformLocation("tex");
	glUniform1i(textureLocation, 0);

	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);

	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	SDL_GL_SwapWindow(_window); // for swapping the double buffer (against flickering)
}