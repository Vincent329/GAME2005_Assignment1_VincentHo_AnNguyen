#include "Target.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/textures/Circle.png","circle");

	const auto size = TextureManager::Instance()->getTextureSize("circle");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("circle", x, y, 0, 255, true);
}

void Target::update()
{
	m_move();
	m_checkBounds();
}

void Target::clean()
{
}

float Target::getGravityFactor()
{
	return m_gravityFactor;
}

void Target::setGravityFactor(float gFactor)
{
	m_gravityFactor = gFactor;
}

float Target::getPixelsPerMeter()
{
	return m_PPM;
}

void Target::setPixelsPerMeter(float ppm)
{
	m_PPM = ppm;
}

void Target::setIsGravityEnabled(bool check)
{
	m_isGravityEnabled = check;
}

void Target::m_move()
{
	// multiplying velocity and acceleration in the meters
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f) * m_PPM;
	getRigidBody()->acceleration = glm::vec2(0.0f, m_gravityFactor) * m_PPM;

	// affect velocity with acceleration
	if (!m_isGravityEnabled) {
		getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		getRigidBody()->acceleration = glm::vec2(0.0f, 9.8f);
		elapsedTime = 1.0f / 60.0f;
	}
	else {
		/*getRigidBody()->velocity += (getRigidBody()->acceleration + gravity) * deltaTime;
		getTransform()->position += getRigidBody()->velocity * deltaTime;*/
		
		// new logic
		// Pf = Pi + v*t + 1/2a*t^
		getTransform()->position = getTransform()->position
			+ (getRigidBody()->velocity * deltaTime)
			+ ((0.5f * getRigidBody()->acceleration) * (elapsedTime * elapsedTime));
		elapsedTime += deltaTime;

	}
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}

void Target::doThrow()
{
	getTransform()->position = throwPosition;
	getRigidBody()->velocity = throwSpeed;
}
