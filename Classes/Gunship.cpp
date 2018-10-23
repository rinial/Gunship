#include "Gunship.h"
#include "Physics/Physics.h"
#include "LaserBall.h"
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
	shooting_ = true;
}
void Gunship::stopShooting()
{
	shooting_ = false;
}
void Gunship::shoot()
{
	if (!sceneNode_)
		return;

	// TODO change to other sound effect for powerful shots
	// TODO powerful = weird movement

	sinceLastShot_ = 0;
	++shotCount_;

	auto laserLocation = getPosition() + gunDirection_ * gun_->getContentSize().width * LASER_BALL_SPAWN_DISTANCE;

	// "Power" shot is a shot with different curved movement
	const auto isPowerShot = shotCount_ % POWER_SHOT_INDEX == 0;

	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(!isPowerShot ? SHOOT_NORMAL_SOUND_EFFECT : SHOOT_POWERFUL_SOUND_EFFECT);

	auto laserSpeed = gunDirection_ * laserSpeed_ * (!isPowerShot ? 1 : POWER_SHOT_SPEED_K);
	auto laserBall = std::make_unique<LaserBall>(laserLocation, !isPowerShot ?
		std::make_unique<PhysMovement>(laserSpeed) :
		std::make_unique<PhysLeftRightMovement>(laserSpeed, CC_DEGREES_TO_RADIANS(POWER_SHOT_ANGULAR_SPEED), POWER_SHOT_CURVE_DURATION, shotCount_ % (2 * POWER_SHOT_INDEX) == 0 ? 1 : -1));
	laserBall->addToScene(sceneNode_, Z_LEVEL_PROJECTILE);
	getWorld()->addBody(std::move(laserBall));
}

// Adds game object to scene
// Also create projectiles for future shooting
void Gunship::addToScene(Scene* scene, const int zLevel)
{
	GameObject::addToScene(scene, zLevel);

	// TODO create projectiles for future use
}

// Spawn projectiles
void Gunship::step(const float dT)
{
	GameObject::step(dT);

	sinceLastShot_ += dT;
	if (shooting_ && sinceLastShot_ >= SHOT_INTERVAL)
		shoot();
}

// Constructor
Gunship::Gunship(const Vec2& pos, const float laserSpeed) : GameObject(pos, GUNSHIP_MASS, GUNSHIP_BOUNCINESS)
{
	if (laserSpeed <= 0)
		throw std::invalid_argument("laser speed should be > 0");

	laserSpeed_ = laserSpeed;

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

	sinceLastShot_ = SHOT_INTERVAL;
}
// Important for cleaning memory using base class pointe
Gunship::~Gunship() = default;
