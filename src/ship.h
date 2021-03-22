#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "Agent.h"
#include "TextureManager.h"
#include <glm/vec4.hpp>


class Ship final : public Agent
{
public:
	Ship();
	~Ship();

	// Inherited via Agent
	void draw() override;
	void update() override;
	void clean() override;

	// Movement
	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();	
	void move();

	// getters
	float getMaxSpeed() const;

	// setters
	void setMaxSpeed(float newSpeed);

private:
	void m_checkBounds();
	void m_reset();

	// steering behaviours
	float m_maxSpeed;
	float m_turnRate;
};


#endif /* defined (__SHIP__) */

