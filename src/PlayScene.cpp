#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("Background",
		0, getTransform()->position.y, 0, 255, false);
	
	for (int i = 0; i < m_pPlayerBullets.size(); i++)
	{
		addChild(m_pPlayerBullets[i]);
	}
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();	
	}

	drawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	//std::cout << m_pPlayerBullets.size() << std::endl;
	meleeCoolDown--;
	CollisionsUpdate();
	updateDisplayList();
	m_CheckShipLOS(m_pEnemy);
	m_CheckShipDetection(m_pEnemy);
	if(m_getPatrolMode())
		decisionTree->Update();
	if(m_meleeActtack != nullptr)
		m_meleeActtack->getTransform()->position = m_pShip->getTransform()->position - glm::vec2(-10.0f, 10.f);
	for (int i = 0; i < m_pPlayerBullets.size(); i++)
	{
		m_pPlayerBullets[i]->setRotation(m_pShip->getCurrentHeading());
	}
	m_meleeActtack->setDirection(m_pShip->getCurrentHeading() + 90);

	if(CollisionManager::AABBCheck(m_pEnemy, m_pNode[currentMapNode]))
	{
		if (currentMapNode == 19)
			currentMapNode = 0;
		else
		{
			currentMapNode++;
		}
		m_pEnemy->setTargetPosition(glm::vec2(m_pNode[currentMapNode]->getTransform()->position.x - (m_pNode[currentMapNode]->getWidth() / 2), 
			m_pNode[currentMapNode]->getTransform()->position.y - (m_pNode[currentMapNode]->getHeight() / 2)));
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	cooldown--;
	EventManager::Instance().update();

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

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_H))
	{
		m_pShip->setDebug(!m_pShip->getDebugState());
		for (auto node : m_pNode)
			node->setDebug(!node->getDebugState());
		m_pEnemy->setDebug(!m_pEnemy->getDebugState());
		for(auto obstacle: m_pObstacle)
		{
			obstacle->setDebug(!obstacle->getDebug());
		}
	}
	
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_M))
	{
		if (cooldown <= -20)
		{
			if(m_pShip->getHealth() != 3)
				m_pShip->setHealth(m_pShip->getHealth() + 1);
			std::cout << m_pShip->getHealth() << std::endl;
			cooldown = 20;
		}
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_K))
	{
		if (cooldown <= -20)
		{
			if (m_pEnemy->getHealth() != 0)
				m_pEnemy->setHealth(m_pEnemy->getHealth() - 1);
			std::cout << m_pEnemy->getHealth() << std::endl;
			cooldown = 20;
		}
	}
	
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_G))
	{
		if (cooldown <= -20)
		{
			m_pShip->setHealth(m_pShip->getHealth() - 1);
			std::cout << m_pShip->getHealth() << std::endl;
			cooldown = 20;
		}
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_P))
	{
		m_setPatrolMode(!m_getPatrolMode());
	}
	
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_W) || EventManager::Instance().isKeyDown(SDL_SCANCODE_S) 
		|| EventManager::Instance().isKeyDown(SDL_SCANCODE_A) || EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		if(m_pShip->getAnimationState() != PLAYER_SHOOT && m_pShip->getAnimationState() != PLAYER_MELEE)
			m_pShip->setAnimationState(PLAYER_RUN);
		m_pShip->setMoving(true);
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			m_pShip->setYMoving(true);
			m_pShip->setCurrentDirection(glm::vec2(m_pShip->getCurrentDirection().x, -1.0f));
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			m_pShip->setYMoving(true);
			m_pShip->setCurrentDirection(glm::vec2(m_pShip->getCurrentDirection().x, 1.0f));
		}
		else
		{
			m_pShip->setYMoving(false);
		}
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pShip->setXMoving(true);
			m_pShip->setCurrentDirection(glm::vec2(-1.0f, m_pShip->getCurrentDirection().y));
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pShip->setXMoving(true);
			m_pShip->setCurrentDirection(glm::vec2(1.0f, m_pShip->getCurrentDirection().y));
		}
		else
		{
			if(m_pShip->getAnimationState() == PLAYER_RUN)
				m_pShip->setAnimationState(PLAYER_IDLE);
			m_pShip->setXMoving(false);
		}

	}

	else
	{
		if (m_pShip->getAnimationState() != PLAYER_SHOOT && m_pShip->getAnimationState() != PLAYER_MELEE)
			m_pShip->setAnimationState(PLAYER_IDLE);
		m_pShip->setMoving(false);
	}

	if (EventManager::Instance().getMouseButton(1))
	{
		m_pShip->setAnimationState(PLAYER_MELEE);
		addChild(m_meleeActtack);
		m_meleeActtack->setEnabled(true);
		meleeCoolDown = -10;
	}
	else
	{
		m_meleeActtack->setEnabled(false);
	}
	if(cooldown <= 0 && m_pShip->getMoving() == false)
	{
		m_pShip->setAnimationState(PLAYER_IDLE);
	}
	if (EventManager::Instance().getMouseButton(2))
	{
		if (cooldown <= -20)
		{
			m_pShip->setAnimationState(PLAYER_SHOOT);
			m_pPlayerBullets.push_back(new Bullet(m_pShip->getTransform()->position.x - 10, m_pShip->getTransform()->position.y + 10, m_pShip->getCurrentHeading()));
			cooldown = 20;
		}
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	TextureManager::Instance()->load("../Assets/textures/BackgroundFramework.png", "Background");

	// add the ship to the scene as a start point
	m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(50.0f, 550.0f);
	addChild(m_pShip, 2);


	// add the Obstacle to the scene as a start point
	m_pObstacle[0] = new Obstacle(268, 241, 79.0f, 79.0f); //top left obstacle
	addChild(m_pObstacle[0]);

	// add the Obstacle to the scene as a start point
	m_pObstacle[1] = new Obstacle(134, 47, 0, 441);
	addChild(m_pObstacle[1]);

	// add the Obstacle to the scene as a start point
	m_pObstacle[2] = new Obstacle(61, 160, 229, 440);
	addChild(m_pObstacle[2]);

	m_pObstacle[3] = new Obstacle(192, 178, 536, 292);
	addChild(m_pObstacle[3]);

	m_pObstacle[4] = new Obstacle(249, 59, 503, 52);
	addChild(m_pObstacle[4]);
	
	// added the target to the scene a goal

	m_pNode.push_back(new Node(40, 40)); //Top Nodes
	m_pNode.push_back(new Node(110, 40));
	m_pNode.push_back(new Node(170, 40));
	m_pNode.push_back(new Node(230, 40));
	m_pNode.push_back(new Node(290, 40));
	m_pNode.push_back(new Node(365, 40));
	m_pNode.push_back(new Node(365, 100)); //Right Nodes
	m_pNode.push_back(new Node(365, 160));
	m_pNode.push_back(new Node(365, 220));
	m_pNode.push_back(new Node(365, 280));
	m_pNode.push_back(new Node(365, 340));
	m_pNode.push_back(new Node(290, 340)); //Bottom Nodes
	m_pNode.push_back(new Node(230, 340));
	m_pNode.push_back(new Node(170, 340));
	m_pNode.push_back(new Node(110, 340));
	m_pNode.push_back(new Node(40, 340));
	m_pNode.push_back(new Node(40, 280)); //Left Nodes
	m_pNode.push_back(new Node(40, 220));
	m_pNode.push_back(new Node(40, 160));
	m_pNode.push_back(new Node(40, 100));
	for (auto node : m_pNode)
		addChild(node);

	m_pEnemy = new Enemy();
	m_pEnemy->getTransform()->position = glm::vec2(10.0f, 30.0f);
	m_pEnemy->setTargetPosition(m_pNode[0]->getTransform()->position);
	addChild(m_pEnemy);
	// Create Decision Tree
	decisionTree = new DecisionTree();
	decisionTree->setAgent(m_pEnemy);
	decisionTree->Display();


	m_meleeActtack = new MeleeAttack(m_pShip->getCurrentHeading());


	std::cout << "------------------------" << std::endl;
	std::cout << decisionTree->MakeDecision() << std::endl;
	std::cout << "------------------------\n" << std::endl;
}

void PlayScene::CollisionsUpdate()
{
	for (auto& obj : m_pObstacle)
	{
		if (CollisionManager::AABBCheck(m_pShip, obj))
		{
			//std::cout << m_pShip->getTransform()->position.x - m_playerSpeed << std::endl;
			if (int(m_pShip->getTransform()->position.x + m_pShip->getWidth() - m_pShip->getRigidBody()->velocity.x) <= (obj->getTransform()->position.x))
			{
				m_pShip->getTransform()->position.x -= m_playerSpeed * 2;
			}
			else if (int(m_pShip->getTransform()->position.x - m_pShip->getRigidBody()->velocity.x) >= (obj->getTransform()->position.x + obj->getWidth()))
			{
				m_pShip->getTransform()->position.x += m_playerSpeed * 2;
			}
			else if ((m_pShip->getTransform()->position.y + m_pShip->getHeight() - m_playerSpeed) <= (obj->getTransform()->position.y))
			{
				m_pShip->getTransform()->position.y -= m_playerSpeed * 2;
			}
			else if ((m_pShip->getTransform()->position.y + m_playerSpeed) >= (obj->getTransform()->position.y + obj->getHeight()))
			{
				m_pShip->getTransform()->position.y += m_playerSpeed * 2;
			}
		}
	}

	for (auto& obj : m_pObstacle)
	{
		if (CollisionManager::AABBCheck(m_pEnemy, obj))
		{
			//std::cout << m_pShip->getTransform()->position.x - m_playerSpeed << std::endl;
			if (int(m_pEnemy->getTransform()->position.x + m_pEnemy->getWidth() - m_pEnemy->getRigidBody()->velocity.x) <= (obj->getTransform()->position.x))
			{
				m_pEnemy->getTransform()->position.x -= m_playerSpeed * 2;
			}
			else if (int(m_pEnemy->getTransform()->position.x - m_pEnemy->getRigidBody()->velocity.x) >= (obj->getTransform()->position.x + obj->getWidth()))
			{
				m_pEnemy->getTransform()->position.x += m_playerSpeed * 2;
			}
			else if ((m_pEnemy->getTransform()->position.y + m_pEnemy->getHeight() - m_playerSpeed) <= (obj->getTransform()->position.y))
			{
				m_pEnemy->getTransform()->position.y -= m_playerSpeed * 2;
			}
			else if ((m_pEnemy->getTransform()->position.y + m_playerSpeed) >= (obj->getTransform()->position.y + obj->getHeight()))
			{
				m_pEnemy->getTransform()->position.y += m_playerSpeed * 2;
			}
		}
	}

	if (m_pEnemy->getTargetDistance() < 50)
		m_pEnemy->setMaxSpeed(1.0f);

	else m_pEnemy->setMaxSpeed(3.0f);

	for (int i = 0; i < m_pPlayerBullets.size(); i++)
	{
		if (m_pPlayerBullets[i] != nullptr)
		{
			if (m_pPlayerBullets[i]->getTransform()->position.x >= 900)
			{
				removeChild(m_pPlayerBullets[i]);
				m_pPlayerBullets[i] = nullptr;
				m_pPlayerBullets.erase(m_pPlayerBullets.begin() + i);
				m_pPlayerBullets.shrink_to_fit();
				break;
			}
			if (m_pPlayerBullets[i]->getTransform()->position.x <= -100)
			{
				removeChild(m_pPlayerBullets[i]);
				m_pPlayerBullets[i] = nullptr;
				m_pPlayerBullets.erase(m_pPlayerBullets.begin() + i);
				m_pPlayerBullets.shrink_to_fit();
				break;
			}
			if (m_pPlayerBullets[i]->getTransform()->position.y <= -100)
			{
				removeChild(m_pPlayerBullets[i]);
				m_pPlayerBullets[i] = nullptr;
				m_pPlayerBullets.erase(m_pPlayerBullets.begin() + i);
				m_pPlayerBullets.shrink_to_fit();
				break;
			}
			if (m_pPlayerBullets[i]->getTransform()->position.y >= 700)
			{
				removeChild(m_pPlayerBullets[i]);
				m_pPlayerBullets[i] = nullptr;
				m_pPlayerBullets.erase(m_pPlayerBullets.begin() + i);
				m_pPlayerBullets.shrink_to_fit();
				break;
			}

			for(int i = 0; i < m_pPlayerBullets.size(); i++)
			{
				if(CollisionManager::AABBCheck(m_pPlayerBullets[i], m_pEnemy))
				{
					m_pEnemy->setHealth(m_pEnemy->getHealth() - 1);
				}
			}
			for(auto obstacle : m_pObstacle)
			{
				if(CollisionManager::AABBCheck(m_pPlayerBullets[i], obstacle))
				{
					removeChild(m_pPlayerBullets[i]);
					m_pPlayerBullets[i] = nullptr;
					m_pPlayerBullets.erase(m_pPlayerBullets.begin() + i);
					m_pPlayerBullets.shrink_to_fit();
					break;
				}
			}
		}
	}
}

void PlayScene::GUI_Function() 
{
	//TODO: We need to deal with this
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Lab 7", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	// allow ship rotation
	static int angle;
	if(ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pShip->setCurrentHeading(angle);
	}
	
	ImGui::Separator();

	static int shipPosition[] = { m_pShip->getTransform()->position.x, m_pShip->getTransform()->position.y };
	if (ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pShip->getTransform()->position.x = shipPosition[0];
		m_pShip->getTransform()->position.y = shipPosition[1];

		std::cout << "------------------------" << std::endl;
		std::cout << decisionTree->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;
	}
	
	static int targetPosition[] = { m_pEnemy->getTransform()->position.x, m_pEnemy->getTransform()->position.y };
	if(ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pEnemy->getTransform()->position.x = targetPosition[0];
		m_pEnemy->getTransform()->position.y = targetPosition[1];
	}
	
	ImGui::Separator();
	
	if (ImGui::Button("Start"))
	{

	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		// reset everything back to initial values
		
	}

	ImGui::Separator();

	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::m_CheckShipLOS(DisplayObject* target_object)
{
	// if ship to target distance is less than or equal to LOS Distance
	auto ShipToTargetDistance = Util::distance(m_pShip->getTransform()->position, target_object->getTransform()->position);
	if (ShipToTargetDistance <= m_pShip->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			// check if object is farther than than the target
			auto ShipToObjectDistance = Util::distance(m_pShip->getTransform()->position, object->getTransform()->position);

			if (ShipToObjectDistance <= ShipToTargetDistance)
			{
				if ((object->getType() != m_pShip->getType()) && (object->getType() != target_object->getType()))
				{
					contactList.push_back(object);
				}
			}
		}
		contactList.push_back(target_object); // add the target to the end of the list
		auto hasLOS = CollisionManager::LOSCheck(m_pShip->getTransform()->position,
			m_pShip->getTransform()->position + m_pShip->getCurrentDirection() * m_pShip->getLOSDistance(), contactList, target_object);

		m_pShip->setHasLOS(hasLOS);
	}
}

void PlayScene::m_CheckShipDetection(DisplayObject* target_object)
{
	// if ship to target distance is less than or equal to detection Distance
	auto ShipToTargetDistance = Util::distance(m_pShip->getTransform()->position, target_object->getTransform()->position);
	if (ShipToTargetDistance <= m_pShip->getDetectionDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			// check if object is farther than than the target
			auto ShipToObjectDistance = Util::distance(m_pShip->getTransform()->position, object->getTransform()->position);

			if (ShipToObjectDistance <= ShipToTargetDistance)
			{
				if ((object->getType() != m_pShip->getType()) && (object->getType() != target_object->getType()))
				{
					contactList.push_back(object);
				}
			}
		}
		contactList.push_back(target_object); // add the target to the end of the list
		auto hasDetection = CollisionManager::detectionCheck(m_pShip->getTransform()->position, m_pShip->getDetectionDistance(), contactList, target_object);

		m_pShip->setHasDetection(hasDetection);
	}
}

void PlayScene::m_setDebugMode(bool state)
{
	m_isGridEnabled = state;
}

bool PlayScene::m_getDebugMode() const
{
	return m_isGridEnabled;
}

void PlayScene::m_setPatrolMode(bool state)
{
	m_isPatrolling = state;
}

bool PlayScene::m_getPatrolMode() const
{
	return m_isPatrolling;
}
