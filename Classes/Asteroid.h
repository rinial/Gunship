#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "Target.h"
#include "cocos2d.h"

// Represents an asteroid that can be destroyed
class Asteroid : public Target
{

public:
	// Constructors
	explicit Asteroid(const cocos2d::Vec2& pos, const float& scale = 1);
	Asteroid(const cocos2d::Vec2& pos, std::unique_ptr<PhysMovement> movement, const float& scale = 1);
	// Important for cleaning memory using base class pointer
	virtual ~Asteroid();

private:
	cocos2d::Sprite* asteroid_;
};

#endif // __ASTEROID_H__
