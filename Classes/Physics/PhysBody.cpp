#include "PhysBody.h"
#include "PhysCollider.h"
#include "PhysWorld.h"
#include "PhysContact.h"
#include "PhysMovement.h"

USING_NS_CC;

// Sets the world to inform it of body changes later
// Should only be called from PhysWorld directly when adding body
void PhysBody::setWorld(PhysWorld* world)
{
	if (!world)
		throw std::invalid_argument("world can't be nullptr");
	world_ = world;
}

// Updates position and informs world about it
void PhysBody::setPosition(const Vec2& pos)
{
	position_ = pos;
	informWorld();
}

// Add/remove the collider and inform the world about it
void PhysBody::addCollider(std::unique_ptr<PhysCollider> collider)
{
	if (!collider)
		throw std::invalid_argument("collider can't be nullptr");
	colliders_.push_back(std::move(collider));
	informWorld();
}
void PhysBody::removeCollider(PhysCollider* collider)
{
	if (!collider)
		return; // it's ok to 'remove' nullptr from the colliders_
		// throw std::invalid_argument("collider can't be nullptr");

	// colliders_.erase(std::find_if(colliders_.begin(), colliders_.end(), [&collider](auto col) { return collider == col.get(); }));
	for(auto it = colliders_.begin(); it != colliders_.end(); ++it)
		if (it->get() == collider) {
			colliders_.erase(it);
			break;
		}
	informWorld();
}

// Set mass
void PhysBody::setMass(const float& mass)
{
	if (mass <= 0)
		throw std::invalid_argument("mass should be > 0");
	mass_ = mass;
}

// Set bounciness
void PhysBody::setBounciness(const float& bounciness)
{
	if (bounciness < 0 || bounciness > 1)
		throw std::invalid_argument("bounciness should be in [0, 1]");
	bounciness_ = bounciness;
}

// Set movement
void PhysBody::setMovement(std::unique_ptr<PhysMovement> movement)
{
	movement_ = std::move(movement);
	movement_->setBody(this);
}

// Set activeness and inform the world
void PhysBody::setActive(const bool active)
{
	if (isActive_ == active)
		return;
	isActive_ = active;
	informWorld();
}

// One step in time for the body
// If it is active and it is not kinematic, it moves
void PhysBody::step(const float dT)
{
	if (dT <= 0)
		throw std::invalid_argument("deltaTime should be > 0");

	if (!isActive_ || isKinematic())
		return;

	movement_->move(dT);
}

// Called on hits
void PhysBody::onHit(const PhysContact& contact)
{
	if (isKinematic())
		return;

	movement_->onHit(contact);
}

// Inform the world about some changes
void PhysBody::informWorld()
{
	if (world_)
		world_->onManipulatedBody(this);
}

// Constructor
PhysBody::PhysBody(const cocos2d::Vec2& pos, const float& mass, const float& bounciness) : position_(pos), mass_(mass), bounciness_(bounciness) {}
// Important for cleaning memory using base class pointer
PhysBody::~PhysBody() = default;
