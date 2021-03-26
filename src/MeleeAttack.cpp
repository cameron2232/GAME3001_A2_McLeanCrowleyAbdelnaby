#include "MeleeAttack.h"
#include "TextureManager.h"

MeleeAttack::MeleeAttack()
{
	TextureManager::Instance()->load("../Assets/textures/knife.png", "knife");
	auto size = TextureManager::Instance()->getTextureSize("knife");
	getTransform()->position = glm::vec2(300.0f, 300.0f);
	setWidth(size.x);
	setHeight(size.y);
}

MeleeAttack::~MeleeAttack()
= default;

void MeleeAttack::draw()
{
	TextureManager::Instance()->draw("knife", getTransform()->position.x, getTransform()->position.y, 90, 255, false);
}

void MeleeAttack::update()
{
}

void MeleeAttack::clean()
{
}
