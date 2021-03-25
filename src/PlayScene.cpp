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
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();	
	}

	drawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	CollisionsUpdate();
	updateDisplayList();

	m_CheckShipLOS(m_pTarget);

	m_CheckShipDetection(m_pTarget);
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

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_G))
	{
		if (cooldown <= -20)
		{
			m_pShip->setHealth(m_pShip->getHealth() - 1);
			std::cout << m_pShip->getHealth() << std::endl;
			cooldown = 20;
		}
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
	{
		m_pShip->getTransform()->position.y -= m_playerSpeed;
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		m_pShip->getTransform()->position.x -= m_playerSpeed;
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
	{
		m_pShip->getTransform()->position.y += m_playerSpeed;
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		m_pShip->getTransform()->position.x += m_playerSpeed;
	}
	
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// add the ship to the scene as a start point
	m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(200.0f, 300.0f);
	addChild(m_pShip, 2);


	// add the Obstacle to the scene as a start point
	m_pObstacle[0] = new Obstacle();
	m_pObstacle[0]->getTransform()->position = glm::vec2(300.0f, 250.0f);
	addChild(m_pObstacle[0]);

	// add the Obstacle to the scene as a start point
	m_pObstacle[1] = new Obstacle();
	m_pObstacle[1]->getTransform()->position = glm::vec2(400.0f, 50.0f);
	addChild(m_pObstacle[1]);

	// add the Obstacle to the scene as a start point
	m_pObstacle[2] = new Obstacle();
	m_pObstacle[2]->getTransform()->position = glm::vec2(200.0f, 450.0f);
	addChild(m_pObstacle[2]);
	
	// added the target to the scene a goal
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(600.0f, 300.0f);
	addChild(m_pTarget);

	m_pNode.push_back(new Node(50, 50));
	for (auto node : m_pNode)
		addChild(node);

	// Create Decision Tree
	decisionTree = new DecisionTree();
	decisionTree->setAgent(m_pShip);
	decisionTree->Display();


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
			if ((m_pShip->getTransform()->position.x + m_playerSpeed) <= (obj->getTransform()->position.x))
			{
				m_pShip->getTransform()->position.x -= m_playerSpeed *2;
			}
			else if ((m_pShip->getTransform()->position.x + m_playerSpeed) >= (obj->getTransform()->position.x + obj->getWidth()))
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
	
	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if(ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];
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
