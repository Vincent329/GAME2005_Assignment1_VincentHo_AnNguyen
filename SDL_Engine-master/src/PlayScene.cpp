#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 400.0f, 300.0f, 0, 255, true, SDL_FLIP_NONE);

	drawDisplayList();
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	if (isMoving)
	{
		m_pBall->m_move();
	}
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}
float PlayScene::getGravityFactor()
{
	return m_gravityFactor;
}

void PlayScene::setGravityFactor(float gFactor)
{
	m_gravityFactor = gFactor;
}

float PlayScene::getPixelsPerMeter()
{
	return m_PPM;
}

void PlayScene::setPixelsPerMeter(float ppm)
{
	m_PPM = ppm;
}

void PlayScene::resetValues()
{
	m_gravityFactor = 9.8f;
	m_PPM = 10.0f;
	m_Angle = 0.0f;
}

void PlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/Background.png", "background");

	// Set GUI Title
	m_guiTitle = "Play Scene";	

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Ball sprite
	m_pBall = new Target();
	m_pBall->getTransform()->position = m_pPlayer->getTransform()->position;
	m_pBall->getTransform()->position.x += m_pBall->getWidth();
	addChild(m_pBall);


	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(50.0f, 550.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(750.0f, 550.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);
	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	ImGui::ShowDemoWindow();
	
	ImGui::Begin("Physics Controls", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if(ImGui::Button("Throw"))
	{
		isMoving = (isMoving) ? false : true;
		std::cout << "Is moving: " << isMoving << std::endl;
	}

	ImGui::Separator();

	static bool isGravityEnabled = false;
	if (ImGui::Checkbox("Gravity Enabled", &isGravityEnabled)) // toggling gravity with a checkbox
	{
		m_pBall->setIsGravityEnabled(isGravityEnabled);
		
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset All"))
	{
		isGravityEnabled = false;
		isMoving = false;
		m_pBall->setIsGravityEnabled(isGravityEnabled);
		resetValues();
		m_pPlayer->getTransform()->position = glm::vec2(100.0f, 400.0f);
		m_pBall->getTransform()->position = m_pPlayer->getTransform()->position;
		m_pBall->getTransform()->position.x += m_pBall->getWidth();
		m_pBall->setGravityFactor(9.8f);
		m_pBall->setPixelsPerMeter(10.0f);
	}

	if (ImGui::SliderFloat("Pixels Per Meter", &m_PPM, 0.1f, 30.0f, "%.1f"))
	{
		m_pBall->setPixelsPerMeter(m_PPM);
		std::cout << "Pixels Per Meter: " << m_pBall->getPixelsPerMeter() << std::endl;

	}

	if (ImGui::SliderFloat("Gravity", &m_gravityFactor, 0.1f, 30.0f, "%.1f"))
	{
		m_pBall->setGravityFactor(m_gravityFactor);
		std::cout << "Gravity Factor: " << m_pBall->getGravityFactor() << std::endl;
	}

	if (ImGui::SliderFloat("Kick Angle", &m_Angle, 0.0f, 90.0f, "%.1f"))
	{

	}

	// slider for person 
	// CHANGE NOTES: turn this into a stormtrooper instead of player
	static int xPlayerPos = 300;
	if (ImGui::SliderInt("Player Position X", &xPlayerPos, 0, 800)) {
		m_pPlayer->getTransform()->position.x = xPlayerPos;
		m_pBall->getTransform()->position = glm::vec2(xPlayerPos+m_pBall->getWidth(), 400);
	}

	// change this to an angle slider between 0 and 180
	static float velocity[2] = { 0,0 };
	if (ImGui::SliderFloat2("Throw Speed", velocity, 0, 500))
	{
		m_pBall->throwPosition = m_pPlayer->getTransform()->position;
		// CHANGE THIS LOGIC, NEEDS TO THROW BY ANGLE
		m_pBall->throwSpeed = glm::vec2(velocity[0], -velocity[1]);
		
	}
	
	/*static float angle = 0;
	if (ImGui::SliderFloat("Angle", &angle, 0, 90.0f))
	{
	}*/

	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsClassic();
}
