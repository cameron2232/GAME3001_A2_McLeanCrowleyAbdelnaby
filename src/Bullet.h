#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "DisplayObject.h"

class Bullet : public DisplayObject
{
public:
	Bullet(float x, float y);
	~Bullet();
	void draw() override;
	void update() override;
	void clean() override;
private:
	int m_rotation;
};
#endif // !__BULLET__