#pragma once
#ifndef __MELEE_ATTACK__
#define __MELEE_ATTACK__

#include "DisplayObject.h"

class MeleeAttack: public DisplayObject
{
public:
	MeleeAttack();
	~MeleeAttack();
	void draw() override;
	void update() override;
	void clean() override;
private:

};
#endif // !__MELEE_ATTACK__

