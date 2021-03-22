#include "Agent.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"

Agent::Agent() {}

Agent::~Agent() = default;

bool Agent::getDebugState() const
{
	return m_DebugEnabled;
}

void Agent::setDebug(bool state)
{
	m_DebugEnabled = state;
}

glm::vec2 Agent::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Agent::getCurrentDirection() const
{
	return m_currentDirection;
}

float Agent::getLOSDistance() const
{
	return m_LOSDistance;
}

bool Agent::hasLOS() const
{
	return m_hasLOS;
}

float Agent::getCurrentHeading() const
{
	return m_currentHeading;
}

glm::vec4 Agent::getLOSColour() const
{
	return m_LOSColour;
}

float Agent::getDetectionDistance() const
{
	return m_DetectionDistance;
}

bool Agent::hasDetection() const
{
	return m_hasDetection;
}

glm::vec4 Agent::getDetectionColor() const
{
	return m_DetectionColour;
}

void Agent::setLOSDistance(const float distance)
{
	m_LOSDistance = distance;
}

void Agent::setHasLOS(const bool state)
{
	m_hasLOS = state;
	m_LOSColour = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void Agent::setCurrentHeading(const float heading)
{
	m_currentHeading = heading;
	m_changeDirection();
}

void Agent::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;

}

void Agent::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void Agent::setLOSColour(glm::vec4 colour)
{
	m_LOSColour = colour;
}

void Agent::setDetectionDistance(float distance)
{
	m_DetectionDistance = distance;
}

void Agent::setHasDetection(bool state)
{
	m_hasDetection = state;
	m_DetectionColour = (m_hasDetection) ? glm::vec4(0, 1, 0, 1) : glm::vec4(0, 0, 1, 1);
}

void Agent::setDetectionColor(glm::vec4 colour)
{
	m_DetectionColour = colour;
}

void Agent::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);

	glm::vec2 size = TextureManager::Instance()->getTextureSize("ship");
}

