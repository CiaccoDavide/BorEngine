#include "MainGame.h"

#include <iostream>
#include <string>
#include <BorEngine\Errors.h>
#include <BorEngine\ResourcesManager.h>

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
	gameLoop(); // will return only when the game ends!
}

static BorEngine::GLTexture texture;

void MainGame::initSystems()
{
	BorEngine::init();

	_window.create("Bor Engine", _screenWidth, _screenHeight, BorEngine::DEFAULT);

	initShaders();

	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);

	texture = BorEngine::ResourcesManager::getTexture("./Textures/test_png_icon/icon_transparency_fades.png");
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
		_fpsLimiter.begin();

		processInput();
		_time += 0.001f; // arbitrary time for now...

		_camera.update();
		drawGame();

		_fps = _fpsLimiter.end();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10)
		{
			std::cout << "\r FPS: " << _fps << "                                          ";
			frameCounter = 0;
		}


	}
}
void MainGame::processInput()
{
	const float CAMERA_SPEED = 2.0f;
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
			_inputManager.keyDown(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.keyUp(evnt.key.keysym.sym);
			break;
		}
	}

	if (_inputManager.isKeyDown(SDLK_w))
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, CAMERA_SPEED));

	if (_inputManager.isKeyDown(SDLK_a))
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));

	if (_inputManager.isKeyDown(SDLK_s))
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0, -CAMERA_SPEED));

	if (_inputManager.isKeyDown(SDLK_d))
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));

	if (_inputManager.isKeyDown(SDLK_q))
		_camera.setScale(_camera.getScale() - SCALE_SPEED);

	if (_inputManager.isKeyDown(SDLK_e))
		_camera.setScale(_camera.getScale() + SCALE_SPEED);

}

void MainGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // bit-wise or | 

	_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = _colorProgram.getUniformLocation("tex");
	glUniform1i(textureLocation, 0);

	/*GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time * 300);*/

	// set the camera matrix
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));


	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 100.0f, 100.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//_playerTexture = ImageLoader::loadPNG("./Textures/test_png_icon/icon.png");
	//_playerTexture = ImageLoader::loadPNG("./Textures/test_png_icon/icon_transparency.png");
	//_playerTexture = ImageLoader::loadPNG("./Textures/test_png_icon/icon_transparency_fades.png");
	BorEngine::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	//for (int i = 0; i < 1000; i++) {
		_spriteBatch.draw(pos - glm::vec4(50, 50, 0, 0), uv, texture.id, 0.0f, color);
		_spriteBatch.draw(pos - glm::vec4(75, 75, 0, 0), uv, texture.id, 0.0f, color);
		_spriteBatch.draw(pos - glm::vec4(100, 100, 0, 0), uv, texture.id, 0.0f, color);
		//std::cout << "\nDRAWing SPRITE "<< i;
	//}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	_window.swapBuffer(); // for swapping the double buffer (against flickering)
}

