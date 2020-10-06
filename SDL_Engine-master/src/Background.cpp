#include "Background.h"
#include "TextureManager.h"

Background::Background()
{
	TextureManager::Instance()->load("../Assets/textures/Background.png", "background");
	
	const auto size = TextureManager::Instance()->getTextureSize("background");
	
	// set frame width
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(400.0f, 300.0f);
}

Background::~Background()
= default;

void Background::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("background", x, y, 0, 255, true);
}

void Background::update()
{

}

void Background::clean()
{
}