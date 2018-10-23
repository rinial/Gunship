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

// Accelerate in direction
void Gunship::accelerate(const Vec2& direction)
{
	getMovement()->setAcceleration(direction.getNormalized() * GUNSHIP_ACCELERATION);

	if (direction.isZero())
		boosters_->pauseEmissions();
	else
		boosters_->resumeEmissions();
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

	sinceLastShot_ = 0;
	++shotCount_;

	const auto laserLocation = getPosition() + gunDirection_ * gun_->getContentSize().width * LASER_BALL_SPAWN_DISTANCE;

	// "Power" shot is a shot with different curved movement
	const auto isPowerShot = shotCount_ % POWER_SHOT_INDEX == 0;

	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(!isPowerShot ? SHOOT_NORMAL_SOUND_EFFECT : SHOOT_POWERFUL_SOUND_EFFECT);

	// Spawns new or takes from pool
	auto laserBall = spawnLaserBall(laserLocation);
	auto laserSpeed = gunDirection_ * laserSpeed_ * (!isPowerShot ? 1 : POWER_SHOT_SPEED_K);
	laserBall->setMovement(!isPowerShot ?
		std::make_unique<PhysMovement>(laserSpeed) :
		std::make_unique<PhysLeftRightMovement>(laserSpeed, CC_DEGREES_TO_RADIANS(POWER_SHOT_ANGULAR_SPEED), POWER_SHOT_CURVE_DURATION, shotCount_ % (2 * POWER_SHOT_INDEX) == 0 ? 1 : -1));
	laserBall->setColor(!isPowerShot ? LASER_BALL_NORMAL_COLOR : LASER_BALL_POWERFUL_COLOR);
}

// Spawn projectiles
void Gunship::step(const float dT)
{
	GameObject::step(dT);

	sinceLastShot_ += dT;
	if (shooting_ && sinceLastShot_ >= SHOT_INTERVAL)
		shoot();
}

// Handle event from other game object
// Find a deactivated projectile and add it to pool
void Gunship::onGameObjectDeactivated(GameObject* sender)
{
	const auto laserBall = dynamic_cast<LaserBall*>(sender);
	if (laserBall)
		laserBallsPool_.push(laserBall);
}

// Pool a laser ball or create a new one
LaserBall* Gunship::spawnLaserBall(const Vec2& pos)
{
	LaserBall* laserBall = nullptr;

	// Pool
	if(!laserBallsPool_.empty()) {
		laserBall = laserBallsPool_.front();
		laserBallsPool_.pop();
		laserBall->setPosition(pos);
		laserBall->setActive(true);
		laserBall->reset(); // reset life time
	}
	// Create new
	else {
		auto newLaserBall = std::make_unique<LaserBall>(pos);
		newLaserBall->addToScene(sceneNode_, Z_LEVEL_PROJECTILE);
		newLaserBall->addListener(this);
		laserBall = newLaserBall.get();
		getWorld()->addBody(std::move(newLaserBall));
	}

	return laserBall;
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
	rootNode_->addChild(hull_, 0);

	gun_ = Sprite::create();
	gun_->initWithFile(GUN_SPRITE);
	lookInDirection(Vec2(1, 0)); // Initial gun direction
	rootNode_->addChild(gun_, -1); // gun is below the hull

	// Create particle
	boosters_ = ParticleSystemQuad::create(GUNSHIP_BOOSTERS_PARTICLES); 
	boosters_->setPosition(Vec2::ZERO);
	boosters_->pauseEmissions();
	rootNode_->addChild(boosters_, -2);

	addCollider(std::make_unique<PhysCircleCollider>(hull_->getContentSize().width / 2, GUNSHIP_BITMASKS));
	setMovement(std::make_unique<PhysMovement>());

	sinceLastShot_ = SHOT_INTERVAL;
}
// Important for cleaning memory using base class pointe
Gunship::~Gunship() = default;
