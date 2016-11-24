#include "MainGame.h"

#include <iostream>
#include <string>
#include <BorEngine\Errors.h>
#include <BorEngine\ResourcesManager.h>

#include <algorithm>

#include <BorEngine\SpriteFont.h>

MainGame::MainGame()
{
	_camera.init(_screenWidth, _screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);
	//_hudCamera.setPosition(glm::vec2(_screenWidth/2,0));
	_hudCamera.setPosition(glm::vec2(0, 0));
	//_textBuffer[256];
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
	_hudSpriteBatch.init();

	// initialize sprite font
	_spriteFont = new BorEngine::SpriteFont("Fonts/chau.ttf", 32);

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
	const int  MAX_PHYSICS_STEPS = 6;
	const float DESIRED_FPS = 60;
	const float MS_PER_SEC = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SEC / DESIRED_FPS;
	float prevTicks = SDL_GetTicks();
	const float MAX_DELTA_TIME = 1.0f;

	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();


		float newTicks = SDL_GetTicks();
		float frameTime = SDL_GetTicks() - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		_inputManager.update();
		processInput();

		int i = 0;
		while (totalDeltaTime > 0 && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			totalDeltaTime -= deltaTime;

			// here you can update the deltaTime dependant functions!

			i++;
		}

		_camera.update();

		_hudCamera.update();
		drawGame();

		_fps = _fpsLimiter.end();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 50)
		{
			std::cout << "\n [ INFO  ] FPS: " << _fps << "                                          ";
			frameCounter = 0;
		}


	}
}
void MainGame::processInput()
{
	float CAMERA_SPEED = 2.0f;
	float SCALE_SPEED = 0.1f;

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
			//std::cout << "\r Mouse Position: " << evnt.motion.x << " " << evnt.motion.y << "                     ";
			_mouseX = (float)evnt.motion.x;
			_mouseY = (float)evnt.motion.y;
			_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.keyDown(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.keyUp(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.keyDown(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.keyUp(evnt.button.button);
			break;
		}
	}

	if (_inputManager.isKeyDown(SDLK_LSHIFT)) {
		CAMERA_SPEED = 12.0f;
		SCALE_SPEED = 0.8f;
	}
	else {
		CAMERA_SPEED = 2.0f;
		SCALE_SPEED = 0.1f;
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


	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.screenToWorldCoords(mouseCoords);
		std::cout << "\n [ INPUT ] Click on (" << mouseCoords.x << ", " << mouseCoords.y << ")";
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

	BorEngine::ColorRGBA8 color = BorEngine::ColorRGBA8(255, 255, 255, 255);
	/*color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;*/

	glm::vec2 spritePosition(pos.x, pos.y);
	glm::vec2 spriteSize(pos.z, pos.w);

	for (int spawn_y = 0; spawn_y < 10000; spawn_y += 120) {
		for (int spawn_x = 0; spawn_x < 10000; spawn_x += 120) {
			spritePosition.x = pos.x + spawn_x;
			spritePosition.y = pos.y + spawn_y;
			spriteSize.x = pos.z;
			spriteSize.y = pos.w;
			glm::vec4 rokt(spritePosition.x, spritePosition.y, 100.0f, 100.0f);
			if (_camera.isBoxInView(spritePosition, spriteSize))
				_spriteBatch.draw(rokt, uv, texture.id, 0.0f, color);
			//std::cout << "\nDRAWing SPRITE "<< i;
		}
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();
	_updatesCount++;
	drawHUD();

	glBindTexture(GL_TEXTURE_2D, 0);

	_colorProgram.unuse();

	_window.swapBuffer(); // for swapping the double buffer (against flickering)
}

void MainGame::drawHUD() {

	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _hudCamera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_hudSpriteBatch.begin();

	//char buffer[256];
	// string, position, scale, color, horizontal-alignment
	writeText("Hello, HUD!", glm::vec2(-1, -1), glm::vec2(1.0), BorEngine::ColorRGBA8(0, 0, 0, 255), BorEngine::Justification::MIDDLE);
	writeText("Hello, HUD!", glm::vec2(0, 0), glm::vec2(1.0), BorEngine::ColorRGBA8(255, 255, 255, 255), BorEngine::Justification::MIDDLE);
	writeText("Updates: %d", _updatesCount, glm::vec2(0, -40), glm::vec2(0.4), BorEngine::ColorRGBA8(255, 255, 255, 255), BorEngine::Justification::MIDDLE);
	writeText("FPS: %d", _fps, glm::vec2(0, -60), glm::vec2(0.4), BorEngine::ColorRGBA8(255, 255, 255, 255), BorEngine::Justification::MIDDLE);

	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}

void MainGame::writeText(const char* str, glm::vec2 position, glm::vec2 scale, BorEngine::ColorRGBA8 color, BorEngine::Justification alignment) {
	sprintf_s(_textBuffer, str);
	_spriteFont->draw(
		_hudSpriteBatch,
		_textBuffer,
		position,
		scale,
		0.0f,
		color,
		alignment
	);
}
void MainGame::writeText(const char* str, int data, glm::vec2 position, glm::vec2 scale, BorEngine::ColorRGBA8 color, BorEngine::Justification alignment) {
	sprintf_s(_textBuffer, str, data);
	_spriteFont->draw(
		_hudSpriteBatch,
		_textBuffer,
		position,
		scale,
		0.0f,
		color,
		alignment
	);
}
