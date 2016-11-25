#include "MainGame.h"

#include <iostream>
#include <string>
#include <BorEngine\BorEngineErrors.h>
#include <BorEngine\ResourcesManager.h>

#include <algorithm>

#include <BorEngine\SpriteFont.h>
//#include <BorEngine\AudioEngine.h>
#include <BorEngine\ParticleBatch2D.h>
#include <BorEngine\ParticleEngine2D.h>

#include <random>
#include <time.h>
#include <glm\gtx\rotate_vector.hpp>

MainGame::MainGame()
{
	p_camera.init(p_screenWidth, p_screenHeight);
	p_hudCamera.init(p_screenWidth, p_screenHeight);
	//_hudCamera.setPosition(glm::vec2(_screenWidth/2,0));
	p_hudCamera.setPosition(glm::vec2(0, 0));
	//_textBuffer[256];
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	BorEngine::Music music = p_audioEngine.loadMusic("Sound/soundtrack.wav");
	music.play();
	p_soundEffect = p_audioEngine.loadSoundEffect("Sound/effect.wav");

	gameLoop(); // will return only when the game ends!
}

static BorEngine::GLTexture texture;

void MainGame::initSystems()
{
	BorEngine::init();

	p_audioEngine.init();

	_window.create("Bor Engine", p_screenWidth, p_screenHeight, BorEngine::DEFAULT);

	initShaders();

	p_spriteBatch.init();
	p_hudSpriteBatch.init();

	// initialize sprite font
	p_spriteFont = new BorEngine::SpriteFont("Fonts/chau.ttf", 32);

	p_fpsLimiter.init(p_maxFPS);

	texture = BorEngine::ResourcesManager::getTexture("./Textures/test_png_icon/icon_transparency_fades.png");

	p_clickedParticleBatch = new BorEngine::ParticleBatch2D;
	p_clickedParticleBatch->init(10000, 0.01f, BorEngine::ResourcesManager::getTexture("./Textures/star.png"));
	p_particleEngine.addParticleBatch(p_clickedParticleBatch);
}

void MainGame::initShaders()
{
	p_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	p_colorProgram.addAttribute("vertexPosition");
	p_colorProgram.addAttribute("vertexColor");
	p_colorProgram.addAttribute("vertexUV");

	p_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	const int  MAX_PHYSICS_STEPS = 6;
	const float DESIRED_FPS = 60;
	const float MS_PER_SEC = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SEC / DESIRED_FPS;
	float prevTicks = SDL_GetTicks();
	const float MAX_DELTA_TIME = 1.0f;

	while (p_gameState != GameState::EXIT)
	{
		p_fpsLimiter.begin();


		float newTicks = SDL_GetTicks();
		float frameTime = SDL_GetTicks() - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		p_inputManager.update();
		processInput();

		int i = 0;
		while (totalDeltaTime > 0 && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			totalDeltaTime -= deltaTime;

			// here you can update the deltaTime dependant functions!

			p_particleEngine.update(deltaTime);

			i++;
		}

		p_camera.update();

		p_hudCamera.update();
		drawGame();

		p_fps = p_fpsLimiter.end();

		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 50)
		{
			std::cout << "\n [ INFO  ] FPS: " << p_fps << "                                          ";
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
			p_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//std::cout << "\r Mouse Position: " << evnt.motion.x << " " << evnt.motion.y << "                     ";
			p_mouseX = (float)evnt.motion.x;
			p_mouseY = (float)evnt.motion.y;
			p_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			p_inputManager.keyDown(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			p_inputManager.keyUp(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			p_inputManager.keyDown(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			p_inputManager.keyUp(evnt.button.button);
			break;
		}
	}

	if (p_inputManager.isKeyDown(SDLK_LSHIFT)) {
		CAMERA_SPEED = 4.0f;
		SCALE_SPEED = 0.01f;
	}
	else {
		CAMERA_SPEED = 0.5f;
		SCALE_SPEED = 0.005f;
	}

	if (p_inputManager.isKeyDown(SDLK_w))
		p_camera.setPosition(p_camera.getPosition() + glm::vec2(0.0, CAMERA_SPEED));

	if (p_inputManager.isKeyDown(SDLK_a))
		p_camera.setPosition(p_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));

	if (p_inputManager.isKeyDown(SDLK_s))
		p_camera.setPosition(p_camera.getPosition() + glm::vec2(0.0, -CAMERA_SPEED));

	if (p_inputManager.isKeyDown(SDLK_d))
		p_camera.setPosition(p_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));

	if (p_inputManager.isKeyDown(SDLK_q))
		p_camera.setScale(p_camera.getScale() - SCALE_SPEED);

	if (p_inputManager.isKeyDown(SDLK_e))
		p_camera.setScale(p_camera.getScale() + SCALE_SPEED);


	if (p_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = p_inputManager.getMouseCoords();
		mouseCoords = p_camera.screenToWorldCoords(mouseCoords);
		std::cout << "\n [ INPUT ] Click on (" << mouseCoords.x << ", " << mouseCoords.y << ")";

		p_soundEffect.play();

		int numParticles = 1000;
		for (int i = 0; i < numParticles; i++)
		{

			// draw particles!
			static std::mt19937 randEngine(time(nullptr));
			static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f/* 2.0f*M_PI*/);
			float r =1+ static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			glm::vec2 velocity(r,0);
			velocity = glm::rotate(velocity, randAngle(randEngine));
			BorEngine::ColorRGBA8 color(255, 255, 255, 255);

			p_clickedParticleBatch->addParticle(mouseCoords, velocity, color, 80.0f);
		}
	}


}

void MainGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // bit-wise or | 

	p_colorProgram.use();

	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = p_colorProgram.getUniformLocation("tex");
	glUniform1i(textureLocation, 0);

	/*GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time * 300);*/

	// set the camera matrix
	GLuint pLocation = p_colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = p_camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));


	p_spriteBatch.begin();

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
			if (p_camera.isBoxInView(spritePosition, spriteSize))
				p_spriteBatch.draw(rokt, uv, texture.id, 0.0f, color);
			//std::cout << "\nDRAWing SPRITE "<< i;
		}
	}

	p_spriteBatch.end();
	p_spriteBatch.renderBatch();

	p_particleEngine.draw(&p_spriteBatch);

	p_updatesCount++;
	drawHUD();

	glBindTexture(GL_TEXTURE_2D, 0);

	p_colorProgram.unuse();

	_window.swapBuffer(); // for swapping the double buffer (against flickering)
}

void MainGame::drawHUD() {

	GLuint pLocation = p_colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = p_hudCamera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	p_hudSpriteBatch.begin();

	//char buffer[256];
	// string, position, scale, color, horizontal-alignment
	writeText("Hello, HUD!", glm::vec2(-1, -1), glm::vec2(1.0), BorEngine::ColorRGBA8(0, 0, 0, 255), BorEngine::Justification::MIDDLE);
	writeText("Hello, HUD!", glm::vec2(0, 0), glm::vec2(1.0), BorEngine::ColorRGBA8(255, 255, 255, 255), BorEngine::Justification::MIDDLE);
	writeText("Updates: %d", p_updatesCount, glm::vec2(-1, -41), glm::vec2(0.6), BorEngine::ColorRGBA8(0, 0, 0, 255), BorEngine::Justification::MIDDLE);
	writeText("FPS: %d", p_fps, glm::vec2(-1, -81), glm::vec2(0.6), BorEngine::ColorRGBA8(0, 0, 0, 255), BorEngine::Justification::MIDDLE);
	writeText("Updates: %d", p_updatesCount, glm::vec2(0, -40), glm::vec2(0.6), BorEngine::ColorRGBA8(255, 255, 255, 255), BorEngine::Justification::MIDDLE);
	writeText("FPS: %d", p_fps, glm::vec2(0, -80), glm::vec2(0.6), BorEngine::ColorRGBA8(255, 255, 255, 255), BorEngine::Justification::MIDDLE);

	p_hudSpriteBatch.end();
	p_hudSpriteBatch.renderBatch();
}

void MainGame::writeText(const char* str, glm::vec2 position, glm::vec2 scale, BorEngine::ColorRGBA8 color, BorEngine::Justification alignment) {
	sprintf_s(p_textBuffer, str);
	p_spriteFont->draw(
		p_hudSpriteBatch,
		p_textBuffer,
		position,
		scale,
		0.0f,
		color,
		alignment
	);
}
void MainGame::writeText(const char* str, int data, glm::vec2 position, glm::vec2 scale, BorEngine::ColorRGBA8 color, BorEngine::Justification alignment) {
	sprintf_s(p_textBuffer, str, data);
	p_spriteFont->draw(
		p_hudSpriteBatch,
		p_textBuffer,
		position,
		scale,
		0.0f,
		color,
		alignment
	);
}
