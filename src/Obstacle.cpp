#include "Obstacle.h"

Obstacle::Obstacle()
{
	TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle");

	auto size = TextureManager::Instance()->getTextureSize("obstacle");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

}

Obstacle::Obstacle(int w, int h, int x, int y)
{
	getTransform()->position = glm::vec2(x, y);

	setWidth(w);
	setHeight(h);
	
	Rect.w = w;
	Rect.h = h;
	Rect.x = getTransform()->position.x;
	Rect.y = getTransform()->position.y;
}

Obstacle::~Obstacle()
= default;



void Obstacle::draw()
{
	/*TextureManager::Instance()->draw("obstacle", 
		getTransform()->position.x, getTransform()->position.y, 0, 255, false);*/
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 255, 0, 255);
	SDL_RenderFillRect(Renderer::Instance()->getRenderer(), &Rect);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}
