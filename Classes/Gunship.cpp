#include "Gunship.h"
#include "Physics/Physics.h"
#include "Definitions.h"

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

// Change where the gun 'looks'
void Gunship::lookAt(const Vec2& position)
{
	auto direction = position - getPosition();
	if (direction.isZero())
		return; // Don't do anything
	lookInDirection(direction);
}
void Gunship::lookInDirection(const Vec2& direction)
{
	if (direction.isZero())
		return; // Don't do anything
	gunDirection_ = direction.getNormalized();

	// Update gun sprite
	gun_->setPosition(gunDirection_ * gun_->getContentSize().width / 2);
	gun_->setRotation(-CC_RADIANS_TO_DEGREES(gunDirection_.getAngle()));
}

// Shooting functions
void Gunship::startShooting()
{
	// TODO
}
void Gunship::stopShooting()
{
	// TODO
}
void Gunship::shoot()
{
	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(SHOOT_NORMAL_SOUND_EFFECT);
	// TODO change to other sound effect for powerful shots

	// TODO
}

// Adds game object to scene
// Also create projectiles for future shooting
void Gunship::addToScene(Scene* scene, const int zLevel)
{
	GameObject::addToScene(scene, zLevel);

	// TODO create projectiles for future use
}

// Constructor
Gunship::Gunship(const Vec2& pos) : GameObject(pos, GUNSHIP_MASS, GUNSHIP_BOUNCINESS)
{
	hull_ = Sprite::create();
	hull_->initWithFile(GUNSHIP_SPRITE);
	hull_->setPosition(0, 0);

	gun_ = Sprite::create();
	gun_->initWithFile(GUN_SPRITE);
	lookInDirection(Vec2(1, 0)); // Initial gun direction

	rootNode_->addChild(hull_, 0);
	rootNode_->addChild(gun_, -1); // gun is below the hull

	addCollider(std::make_unique<PhysCircleCollider>(hull_->getContentSize().width / 2, GUNSHIP_BITMASKS));
	setMovement(std::make_unique<PhysMovement>());
}
// Important for cleaning memory using base class pointe
Gunship::~Gunship() = default;
