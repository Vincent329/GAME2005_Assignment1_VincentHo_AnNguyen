#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Target.h"
#include "Label.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	// Getter and Setter
	float getGravityFactor();
	void setGravityFactor(float gFactor);
	float getPixelsPerMeter();
	void setPixelsPerMeter(float ppm);

	bool isMoving = false;

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Target* m_pBall;
	bool m_playerFacingRight;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;

	// Physics variables to pass into the Target/Detonator
	float m_gravityFactor = 9.8f; // earth gravity, positive because going downwards is positive
	float m_PPM = 10.0f; // Pixels per meter scale
	float m_Angle = 0.0f; // Launch Angle
	float m_velocity = 0.0f; // initial velocity
	/*float m_velocityX = 0.0f;
	float m_velocityY = 0.0f;*/

	// timing variables
	const int FPS = 60;
	const float deltaTime = 1 / FPS; // adjust by time
	void resetValues();
};

#endif /* defined (__PLAY_SCENE__) */