#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Ship.h"
#include "Button.h"
#include "Target.h"
#include "Reticle.h"
#include "Label.h"
#include "Util.h"

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
	//float getGravityFactor();
	//void setGravityFactor(float gFactor);
	//float getPixelserMeter();
	//void setPixelsPerMeter(float ppm);

	bool isMoving = false;

	// Horizontal Range for Reticle aiming;
	// Distance
	float reticleDistance(float velocity, float angle);

	// Angle
	// float angleChange(float distance, float velocity);

	// Velocity
	// float velocityChange(float distance, float angle);

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Reticle* m_pReticle;
	Ship* m_pShip; // for testing purposes
	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Target* m_pBall;
	bool m_playerFacingRight;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
	Label* m_PPMdisplay;

	// Physics variables to pass into the Target/Detonator
	float m_gravityFactor = 9.8f; // earth gravity, positive because going downwards is positive
	float m_PPM = 5.0f; // Pixels per meter scale
	float m_Angle = 0.0f; // Launch Angle
	float m_velocity = 0.0f; // initial velocity
	/*float m_velocityX = 0.0f;
	float m_velocityY = 0.0f;*/

	// timing variables
	void resetValues();
};

#endif /* defined (__PLAY_SCENE__) */