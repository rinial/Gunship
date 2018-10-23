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
	// AABB test (Axis Aligned Bounding Box)
	// Returns true if body's rectangle intersects specified rectangle
	// Useful for partitions and other calculations
	static bool inRect(PhysBody* body, const cocos2d::Vec2& origin, const cocos2d::Size& size);
private:
	// Same for colliders
	static bool inRect(const cocos2d::Vec2& posBody, PhysCollider* collider, const cocos2d::Vec2& origin, const cocos2d::Size& size);
	static bool inRect(const cocos2d::Vec2& posBody, PhysBoxCollider* box, const cocos2d::Vec2& origin, const cocos2d::Size& size);
	static bool inRect(const cocos2d::Vec2& posBody, PhysCircleCollider* circle, const cocos2d::Vec2& origin, const cocos2d::Size& size);

public:
	// Contact tests
	// For bodies
	// contact is returned by reference if bodies do intersect
	static bool intersects(PhysBody* a, PhysBody* b, PhysContact& contact);
private:
	// For colliders
	// direction is returned by reference if colliders do intersect
	static bool intersects(const cocos2d::Vec2& posA, PhysCollider* a, const cocos2d::Vec2& posB, PhysCollider* b, cocos2d::Vec2& direction, bool& isHit);
	static bool intersects(const cocos2d::Vec2& posA, PhysCircleCollider* a, const cocos2d::Vec2& posB, PhysCircleCollider* b, cocos2d::Vec2& direction);
	static bool intersects(const cocos2d::Vec2& posA, PhysBoxCollider* a, const cocos2d::Vec2& posB, PhysBoxCollider* b, cocos2d::Vec2& direction);
	static bool intersects(const cocos2d::Vec2& posCircle, PhysCircleCollider* circle, const cocos2d::Vec2& posRectangle, PhysBoxCollider* rectangle, cocos2d::Vec2& direction);
	static bool intersects(const cocos2d::Vec2& posRectangle, PhysBoxCollider* rectangle, const cocos2d::Vec2& posCircle, PhysCircleCollider* circle, cocos2d::Vec2& direction);

public:
	PhysContactEvaluator() = delete; // We don't want instances of this class
};

#endif // __PHYS_CONTACT_EVALUATOR_H__
