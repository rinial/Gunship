#ifndef __PHYS_WORLD_H__
#define __PHYS_WORLD_H__

#include "cocos2d.h"
#include "PhysContact.h"
#include <unordered_set>

#define CONTACTS_SET std::unordered_set<PhysContact, PhysContact::PhysContactHasher>

// Forward declarations
class PhysBody;

// Physics world that stores all the bodies and evaluates their contacts in step function
// When world is destroyed, all memory for all of its bodies is cleared
class PhysWorld
{
public:
	// Add/remove a body
	void addBody(std::unique_ptr<PhysBody> body);
	void removeBody(PhysBody* body);
	// Return all bodies
	const std::vector<std::unique_ptr<PhysBody>>& getBodies() const { return bodies_; }

private:
	// Removes all contacts with specific body from currentContacts_
	void removeFromContacts(PhysBody* body);
	// Removes body from all partitions
	void removeFromPartitions(PhysBody* body);

public:
	// Return all current contacts
	const CONTACTS_SET& getCurrentContacts() const { return currentContacts_; }

	// Finds collisions, sends events (and can move physics simulation if we were actually simulating something)
	void step(float dT);

	// Called from bodies when they are moved or changed in other ways
	// Sets these bodies for evaluation, or removes them from evaluation if they are not active anymore
	void onManipulatedBody(PhysBody* body);

private:
	// Returns partition's origin
	cocos2d::Vec2 getPartitionsOrigin(unsigned int index) const;

public:
	// Constructor
	PhysWorld(const cocos2d::Vec2& origin, const cocos2d::Size& size);
	// Needed to avoid problems with smart pointers
	~PhysWorld();

private:
	// Parameters of the world
	// These is where partitions are created
	// Everything outside is ignored during contact evaluation
	// Usually should be a bit wider than screen size
	cocos2d::Size size_;
	cocos2d::Vec2 origin_; // bottom left

	// All bodies handled by this world
	std::vector<std::unique_ptr<PhysBody>> bodies_;

	// All contacts detected in this world
	CONTACTS_SET currentContacts_;

	// All bodies that should be checked for collisions next
	// We need set so that one body isn't added for evaluation several times
	std::unordered_set<PhysBody*> forEvaluation_; 

	// Bodies are removed only at the start of new step to avoid problems
	std::unordered_set<PhysBody*> forRemoval_;

	// Bodies in partitions of the world
	// Needed to make computations faster
	std::vector<std::unordered_set<PhysBody*>> partitions_;
	cocos2d::Size partitionSize_;
};

#endif // __PHYS_WORLD_H__
