#include "Asteroid.h"
#include "Physics/Physics.h"
#include "Projectile.h"
#include "Definitions.h"

USING_NS_CC;

// Constructors
Asteroid::Asteroid(const Vec2& pos, const float& scale, const Color3B& color) : Asteroid(pos, std::make_unique<PhysMovement>(), scale, color) {}
Asteroid::Asteroid(const Vec2& pos, std::unique_ptr<PhysMovement> movement, const float& scale, const Color3B& color) : Target(pos, ASTEROID_MASS * scale * scale, ASTEROID_BOUNCINESS), color_(color)
{
	asteroid_ = Sprite::create();
	asteroid_->initWithFile(ASTEROID_SPRITE);
	asteroid_->setPosition(0, 0);
	asteroid_->setScale(scale);
	asteroid_->setColor(color_);

	rootNode_->addChild(asteroid_);

	addCollider(std::make_unique<PhysCircleCollider>(asteroid_->getContentSize().width / 2 * scale, ASTEROID_BITMASKS));
	setMovement(std::move(movement));

	healthPoints_ = ASTEROID_HP;
}
// Important for cleaning memory using base class pointer
Asteroid::~Asteroid() = default;

// Called on being hit (overlaped) by a Projectile
void Asteroid::onBeingHit(Projectile* projectile, const Vec2& toProjectile)
{
	if (healthPoints_ <= 1) {
		if (sceneNode_) {
			// Create particle
			auto wreck = ParticleSystemQuad::create(ASTEROID_BREAK_PARTICLES);
			wreck->setPosition(getPosition());
			wreck->setColor(color_);
			sceneNode_->addChild(wreck, rootNode_->getLocalZOrder());
		}

		destroy();
	}
	else {
		--healthPoints_;
		const auto temp = static_cast<float>(healthPoints_) / ASTEROID_HP;
		asteroid_->setColor(Color3B(color_.r * temp, color_.g * temp, color_.b * temp));
	}
}
