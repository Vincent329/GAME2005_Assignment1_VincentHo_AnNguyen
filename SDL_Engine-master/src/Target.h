#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;


	glm::vec2 throwSpeed;
	glm::vec2 throwPosition;
	void doThrow();
	void m_move();

	// Getter and Setter
	float getGravityFactor();
	void setGravityFactor(float gFactor);
	float getPixelsPerMeter();
	void setPixelsPerMeter(float ppm);
	void setIsGravityEnabled(bool check);

private:
	// Variables for physics
	float m_gravityFactor = 9.8f;
	float m_PPM = 10.0f; // Pixels per meter scale

	bool m_isGravityEnabled = false;
	
	float deltaTime = 1.0f / 60.0f;
	float elapsedTime = 1.0f / 60.0f;

	glm::vec2 m_finalPosition = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_velocity = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_acceleration = glm::vec2(0.0f, 0.0f);

	void m_checkBounds();
	void m_reset();
};


#endif /* defined (__TARGET__) */