#include "Bullet.h"
#include "TextureManager.h"
Bullet::Bullet(float x, float y)
{
	TextureManager::Instance()->load("../Assets/textures/Bullet.png", "Bullet");
	auto size = TextureManager::Instance()->getTextureSize("Bullet");
	getTransform()->position = glm::vec2(x, y);
	setWidth(size.x);
	setHeight(size.y);
}

Bullet::~Bullet()
= default;

void Bullet::draw()
{
	TextureManager::Instance()->draw("Bullet", getTransform()->position.x, getTransform()->position.y, m_rotation, 255, false);
}

void Bullet::update()
{
}

void Bullet::clean()
{
}
