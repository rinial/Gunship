#ifndef __PHYS_CONTACT_EVALUATOR_H__
#define __PHYS_CONTACT_EVALUATOR_H__

#include "cocos2d.h" // Just for basic things like Vec2

// Forward declarations
class PhysBody;
class PhysContact;
class PhysCollider;
class PhysCircleCollider;
class PhysBoxCollider;

// Class containing static functions for contact evaluation
class PhysContactEvaluator
{
public:
	// Contact tests
	// For bodies
	// contact is returned by reference if bodies do intersect
	static bool intersects(PhysBody* a, PhysBody* b, PhysContact& contact);
private:
	// For colliders
	// direction is returned by reference if colliders do intersect
	static bool intersects(PhysCollider* a, PhysCollider* b, cocos2d::Vec2& direction, bool& isHit);
	static bool intersects(PhysCircleCollider* a, PhysCircleCollider* b, cocos2d::Vec2& direction);
	static bool intersects(PhysBoxCollider* a, PhysBoxCollider* b, cocos2d::Vec2& direction);
	static bool intersects(PhysCircleCollider* circle, PhysBoxCollider* rectangle, cocos2d::Vec2& direction);
	static bool intersects(PhysBoxCollider* rectangle, PhysCircleCollider* circle, cocos2d::Vec2& direction);

public:
	PhysContactEvaluator() = delete; // We don't want instances of this class
};

#endif // __PHYS_CONTACT_EVALUATOR_H__
