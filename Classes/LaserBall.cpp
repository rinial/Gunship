#include "LaserBall.h"
#include "Physics/Physics.h"
#include "Target.h"
#include "Definitions.h"

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

// Set the color of laser ball
void LaserBall::setColor(const Color3B& color)
{
	laserBall_->setColor(color);
	tail_->setStartColor(Color4F(color));
}

// Disable particles
void LaserBall::setActive(const bool active)
{
	laserBall_->setVisible(active);
	if (active)
		tail_->resumeEmissions();
	else
		tail_->pauseEmissions();

	Projectile::setActive(active);
}

// Update particle position
void LaserBall::setPosition(const cocos2d::Vec2& pos)
{
	Projectile::setPosition(pos);
	
	// Update tail position
	if(tail_) tail_->setSourcePosition(pos);
}

// Create tail
void LaserBall::addToScene(cocos2d::Scene* scene, const int zLevel)
{
	Projectile::addToScene(scene, zLevel);

	if (sceneNode_) {
		// Create particle tail
		tail_ = ParticleSystemQuad::create(LASER_BALL_TRAIL_PARTICLES);
		tail_->setSourcePosition(getPosition());
		sceneNode_->addChild(tail_, zLevel);
	}
}

// Constructors
LaserBall::LaserBall(const Vec2& pos, const Vec2& speed) : LaserBall(pos, std::make_unique<PhysMovement>(speed)) {}
LaserBall::LaserBall(const Vec2& pos, std::unique_ptr<PhysMovement> movement) : Projectile(pos, LASER_BALL_MASS, LASER_BALL_BOUNCINESS)
{
	laserBall_ = Sprite::create();
	laserBall_->initWithFile(LASER_BALL_SPRITE);
	laserBall_->setPosition(Vec2::ZERO);
	rootNode_->addChild(laserBall_);

	addCollider(std::make_unique<PhysCircleCollider>(laserBall_->getContentSize().width / 2, LASER_BALL_BITMASKS));
	setMovement(std::move(movement));
}
// Important for cleaning memory using base class pointer
LaserBall::~LaserBall() = default;

// Called on hits
void LaserBall::onHit(const PhysContact& contact)
{
	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(LASER_BOUNCE_SOUND_EFFECT);

	if (sceneNode_) {
		// Create particle
		auto sparks = ParticleSystemQuad::create(LASER_BALL_BOUNCED_PARTICLES);
		sparks->setPosition(getPosition() + contact.getDirectionFrom(this) * laserBall_->getContentSize().width / 2);
		sparks->setStartColor(tail_->getStartColor());
		sceneNode_->addChild(sparks, rootNode_->getLocalZOrder());
	}

	Projectile::onHit(contact);
}

// Check lifetime and destroy if it's too long
void LaserBall::step(const float dT)
{
	Projectile::step(dT);

	if (getLifeTime() > LASER_BALL_LIFE_TIME)
		// destroy();
		setActive(false);
	else
		laserBall_->setOpacity(std::pow((LASER_BALL_LIFE_TIME - getLifeTime()) / LASER_BALL_LIFE_TIME, 0.3) * LASER_BALL_START_OPACITY);
}

// Called on hitting (overlapping) a Target
void LaserBall::onHitTarget(Target* target, const Vec2& toTarget)
{
	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(LASER_HIT_SOUND_EFFECT);

	if (sceneNode_) {
		// Create particle
		auto sparks = ParticleSystemQuad::create(LASER_BALL_DESTROYED_PARTICLES);
		sparks->setPosition(getPosition());
		sparks->setStartColor(tail_->getStartColor());
		sceneNode_->addChild(sparks, rootNode_->getLocalZOrder());
	}

	target->onBeingHit(this, -toTarget);
	setActive(false); // event will be send and gunship will handle it to pool the laserball
	//destroy();
}
