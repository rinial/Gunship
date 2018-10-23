#ifndef __GUNSHIP_H__
#define __GUNSHIP_H__

#include "GameObject.h"
#include "GameObjectEventListener.h"
#include "cocos2d.h"

// Forward declarations
class LaserBall;

// A gunship that can shoot projectiles
class Gunship : public GameObject, GameObjectEventListener
{
public:
	// Change where the gun 'looks'
	void lookAt(const cocos2d::Vec2& position);
	void lookInDirection(const cocos2d::Vec2& direction);

	// Accelerate in direction
	void accelerate(const cocos2d::Vec2& direction);

	// Shooting functions
	void startShooting();
	void stopShooting();
	void shoot();

	// Spawn projectiles
	virtual void step(float dT) override;

	// Handle event from other game object
	// Find a deactivated projectile and add it to pool
	virtual void onGameObjectDeactivated(GameObject* sender) override;

	// Pool a laser ball or create a new one
	LaserBall* spawnLaserBall(const cocos2d::Vec2& pos);

	// Constructor
	explicit Gunship(const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO, float laserSpeed = 100);
	// Important for cleaning memory using base class pointer
	virtual ~Gunship();

private:
	// Direction where the gun looks
	cocos2d::Vec2 gunDirection_;

	// Sprites
	cocos2d::Sprite* hull_;
	cocos2d::Sprite* gun_;

	// For shooting
	float laserSpeed_;
	float sinceLastShot_;
	bool shooting_ = false;
	unsigned int shotCount_ = 0;

	// Pool of laser balls
	std::queue<LaserBall*> laserBallsPool_;

	// Particles for boosters
	cocos2d::ParticleSystemQuad* boosters_;
};

#endif // __GUNSHIP_H__
