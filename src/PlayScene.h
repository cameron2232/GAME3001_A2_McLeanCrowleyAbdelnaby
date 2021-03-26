#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "DecisionTree.h"
#include "Label.h"
#include "Obstacle.h"
#include "ship.h"
#include "SpaceShip.h"
#include "Target.h"
#include "MapNode.h"
#include "MeleeAttack.h"
#include "Bullet.h"

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
	void CollisionsUpdate();

	

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	Target* m_pTarget;
	Ship* m_pShip;
	Obstacle* m_pObstacle[3];
	std::vector<Node*> m_pNode;
	void m_CheckShipLOS(DisplayObject* object);
	void m_CheckShipDetection(DisplayObject* object);
	void m_setDebugMode(bool state);
	bool m_getDebugMode() const;
	int cooldown = 20;
	DecisionTree* decisionTree;
	const float m_playerSpeed = 5.0f;
	MeleeAttack* m_meleeActtack;
	int meleeCoolDown = -10;
	std::vector<Bullet*> m_pPlayerBullets;
};



#endif /* defined (__PLAY_SCENE__) */
