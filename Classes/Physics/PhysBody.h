#ifndef __PHYS_BODY_H__
#define __PHYS_BODY_H__

#include "cocos2d.h" // Just for basic things like Vec2

// Forward declarations
class PhysWorld;
class PhysCollider;
class PhysMovement;
class PhysContact;

// Normal body creation:
// 1) create base    |  auto body std::make_unique<PhysBody>(pos, mass, ...);
// 2) add colliders  |  body->addCollider(std::move(std::make_unique<PhysCollider>(pos, ...)));
// 3) add movement   |  body->addMovement(std::move(std::make_unique<PhysMovement>(...));
// 4) add to world   |  world->addBody(std::move(body));

// Basic physics body with colliders, movement_ and other required information
class PhysBody
{
public:
	// Set the world to inform it of body changes later
	// Should only be called from PhysWorld directly when adding body
	void setWorld(PhysWorld* world);
	// Return world
	PhysWorld* getWorld() const { return world_; }

	// Update position and inform the world about it
	virtual void setPosition(const cocos2d::Vec2& pos);
	// Return position
	const cocos2d::Vec2& getPosition() const { return position_; }

	// Add/remove the collider and inform the world about it
	void addCollider(std::unique_ptr<PhysCollider> collider);
	void removeCollider(PhysCollider* collider);
	// Return all colliders
	const std::vector<std::unique_ptr<PhysCollider>>& getColliders() const { return colliders_; }

	// Set/get mass
	void setMass(const float& mass);
	const float& getMass() const { return mass_; }

	// Set/get bounciness
	void setBounciness(const float& bounciness);
	const float& getBounciness() const { return bounciness_; }

	// Set/get movement
	void setMovement(std::unique_ptr<PhysMovement> movement);
	PhysMovement* getMovement() const { return movement_.get(); }

	// True if there is no movement_
	bool isKinematic() const { return movement_ == nullptr; }

	// Set activeness and inform the world
	virtual void setActive(bool active);
	// True if object is active (should step and can be hit/overlapped)
	bool isActive() const { return isActive_; }

	// One step in time for the body
	// If it is active and it is not kinematic, it moves
	virtual void step(float dT);

	// Called on hits
	virtual void onHit(const PhysContact& contact);
	// Called on overlaps
	virtual void onOverlap(const PhysContact& contact) {}

private:
	// Inform the world about some changes
	void informWorld();

public:
	// Constructor
	explicit PhysBody(const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO, const float& mass = 1, const float& bounciness = 1);
	// Important for cleaning memory using base class pointer
	virtual ~PhysBody();

private:
	// Used to inform world when this body has changed (and thus should be evaluated for contacts)
	// Only set directly from PhysWorld upon adding new PhysBody
	PhysWorld* world_ = nullptr;

	// All colliders of this body
	std::vector<std::unique_ptr<PhysCollider>> colliders_;

	// Position of the body in the PhysWorld space
	cocos2d::Vec2 position_;

	// We don't have rotation here for the sake of simplicity
	// It isn't important for the game
	// float rotation_;

	// Mass of the body in (0, +inf)
	float mass_ = 1;
	// Bounciness of the body in [0, 1]
	float bounciness_ = 1;

	// Movement of the body
	// If there is no movement_ it means that body is kinematic = can only be moved directly via setPosition()
	std::unique_ptr<PhysMovement> movement_;

	// If true, body acts normally
	// If false, it doesn't act at all
	bool isActive_ = true;
};

#endif // __PHYS_BODY_H__
