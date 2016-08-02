#include "MainGame.h"

#include <iostream>
#include <string>
#include <BorEngine\Errors.h>

MainGame::MainGame() :
	_screenWidth(480),
	_screenHeight(320),
	_gameState(GameState::PLAY),
	_time(0),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	float spritePixelSize_width = 210;
	float spritePixelSize_height = 210;

	_sprites.push_back(new BorEngine::Sprite());
	_sprites.back()->init(210.0f, 180.0f, 2.0f * spritePixelSize_width, 2.0f * spritePixelSize_height, "./Textures/test_png_icon/icon_transparency_fades.png"); // just testing

	_sprites.push_back(new BorEngine::Sprite());
	_sprites.back()->init(110.0f, 110.0f, 1.0f * spritePixelSize_width, 1.0f * spritePixelSize_height, "./Textures/test_png_icon/icon_transparency_fades.png");


	/*for (int i = 0; i < 100; i++)
	{
		_sprites.push_back(new BorEngine::Sprite());
		_sprites.back()->init(0.0f, 0.0f, (float)_screenWidth + 0.001f*i, (float)_screenHeight + 0.001f*i, "./Textures/test_png_icon/icon.png");
	}*/


	//_playerTexture = ImageLoader::loadPNG("./Textures/test_png_icon/icon.png");
	//_playerTexture = ImageLoader::loadPNG("./Textures/test_png_icon/icon_transparency.png");
	//_playerTexture = ImageLoader::loadPNG("./Textures/test_png_icon/icon_transparency_fades.png");

	gameLoop();
}

void MainGame::initSystems()
{
	BorEngine::init();

	_window.create("Bor Engine", _screenWidth, _screenHeight, BorEngine::DEFAULT);

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
		float startTicks = (float)SDL_GetTicks();

		processInput();
		_time += 0.001f; // arbitrary time for now...

		_camera.update();
		drawGame();
		calculateFPS();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10)
		{
			std::cout << "\r FPS: " << _fps << "                                          ";
			frameCounter = 0;
		}

		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000.0f / _maxFPS > frameTicks)
		{
			SDL_Delay((Uint32)(1000.0f / _maxFPS - frameTicks));
		}
	}
}
void MainGame::processInput()
{
	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 0.1f;

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
			std::cout << "\r Mouse Position: " << evnt.motion.x << " " << evnt.motion.y << "                     ";
			_mouseX = (float)evnt.motion.x;
			_mouseY = (float)evnt.motion.y;
			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_w:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAMERA_SPEED));
				break;
			case SDLK_a:
				_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
				break;
			case SDLK_s:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -CAMERA_SPEED));
				break;
			case SDLK_d:
				_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
				break;
			case SDLK_q:
				_camera.setScale(_camera.getScale() + SCALE_SPEED);
				break;
			case SDLK_e:
				_camera.setScale(_camera.getScale() - SCALE_SPEED);
				break;
			default:
				break;
			}
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
	glUniform1f(timeLocation, _time*300);

	// set the camera matrix
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));


	for (int i = 0; i < (int)_sprites.size(); i++)
	{
		_sprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	_window.swapBuffer(); // for swapping the double buffer (against flickering)
}


void MainGame::calculateFPS()
{
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;
	static float prevTicks = (float)SDL_GetTicks();
	float currentTicks;
	currentTicks = (float)SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;

	frameTimes[currentFrame%NUM_SAMPLES] = _frameTime;

	prevTicks = currentTicks;

	int count;

	currentFrame++;

	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}
	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++)
	{
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if (frameTimeAverage > 0)
	{
		_fps = 1000.0f / frameTimeAverage;
	}
	else
	{
		_fps = 0.0f;
	}

}
