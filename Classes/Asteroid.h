#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "Target.h"
#include "cocos2d.h"

// Represents an asteroid that can be destroyed
class Asteroid : public Target
{

public:
	// Constructors
	explicit Asteroid(const cocos2d::Vec2& pos, const float& scale = 1, const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);
	Asteroid(const cocos2d::Vec2& pos, std::unique_ptr<PhysMovement> movement, const float& scale = 1, const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);
	// Important for cleaning memory using base class pointer
	virtual ~Asteroid();

protected:
	// Called on being hit (overlaped) by a Projectile
	virtual void onBeingHit(Projectile* projectile, const cocos2d::Vec2& toProjectile) override;

private:
	cocos2d::Sprite* asteroid_;

	// Number of times asteroid has to be hit
	unsigned int healthPoints_;

	// Color of the sprite
	cocos2d::Color3B color_;
};

#endif // __ASTEROID_H__
