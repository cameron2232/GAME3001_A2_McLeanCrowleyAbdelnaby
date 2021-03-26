#include "ship.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"

Ship::Ship() : m_maxSpeed(10.0f)
{
	TextureManager::Instance()->load("../Assets/textures/ship3.png","ship");

	auto size = TextureManager::Instance()->getTextureSize("ship");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SHIP);
	
	setCurrentHeading(0.0f); // current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 5.0f; // 5 degrees per frame

	setMaxSpeed(5.0f);
	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));
	setHasLOS(false);
	setDetectionDistance(60.0f);
	setDetectionColor(glm::vec4(0, 0, 1, 1));
	setHasDetection(false);
	setDebug(false);
	setHealth(3);
	setHealthPostion(getTransform()->position - glm::vec2(40.0f, 25.0f));

	//getHealth()->getTransform()->position = getTransform()->position + glm::vec2(0.0f, 25.0f);
}


Ship::~Ship()
= default;

void Ship::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance()->draw("ship", x, y, getCurrentHeading(), 255, false);


	if (getDebugState())
	{
		//Draw LOS
		Util::DrawLine(glm::vec2(getTransform()->position.x + getWidth() / 2, getTransform()->position.y + getHeight() / 2),
			(glm::vec2(getTransform()->position.x + getWidth() / 2, getTransform()->position.y + getHeight() / 2)) + getCurrentDirection() * getLOSDistance(), getLOSColour());
		
		// draw detection radius
		Util::DrawCircle(glm::vec2(getTransform()->position.x + getWidth() / 2, getTransform()->position.y + getHeight() / 2), getDetectionDistance(), getDetectionColor());
	}
	drawHeath();
}


void Ship::update()
{
	
	if(getMoving())
		move();
	m_checkBounds();
	setHealthPostion(getTransform()->position - glm::vec2(40.0f, 25.0f));
	auto angle = (atan2(EventManager::Instance().getMousePosition().y - getTransform()->position.y, EventManager::Instance().getMousePosition().x - getTransform()->position.x)
		* 180.00 / 3.1415926);
	setCurrentHeading(angle);
}

void Ship::clean()
{
}

void Ship::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate );
	if (getCurrentHeading() >= 360) 
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);;
	}
}

void Ship::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Ship::moveForward()
{
	getRigidBody()->velocity = getCurrentDirection() * m_maxSpeed;
}

void Ship::moveBack()
{
	getRigidBody()->velocity = getCurrentDirection() * -m_maxSpeed;
}

void Ship::move()
{
	/*getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= 0.9f;*/
	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, getMaxSpeed());
	if(getXMoving())
	{
		getRigidBody()->velocity.x = getCurrentDirection().x * m_maxSpeed;
		getTransform()->position.x += getRigidBody()->velocity.x;

	}
	if(getYMoving())
	{

		getRigidBody()->velocity.y = getCurrentDirection().y * m_maxSpeed;
		getTransform()->position.y += getRigidBody()->velocity.y;
	}

}

float Ship::getMaxSpeed() const
{
	return m_maxSpeed;
}

void Ship::setMaxSpeed(const float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Ship::m_checkBounds()
{

	if (getTransform()->position.x + getWidth() > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(800.0f - getWidth(), getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y + getHeight() > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f - getHeight());
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}
}

void Ship::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

