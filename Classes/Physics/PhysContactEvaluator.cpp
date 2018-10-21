#include "PhysContactEvaluator.h"

#include "PhysBody.h"
#include "PhysContact.h"
#include "PhysCollider.h"
#include "PhysCircleCollider.h"
#include "PhysBoxCollider.h"
#include "Definitions.h"

#include "cocos2d.h" // We use Vec2 for physics
#include <array>

USING_NS_CC;

// Contact tests
// For bodies
// contact is returned by reference if bodies do intersect
bool PhysContactEvaluator::intersects(PhysBody* a, PhysBody* b, PhysContact& contact)
{
	if (!a || !b)
		throw std::invalid_argument("bodies can't be null pointers");
	contact.a_ = a;
	contact.b_ = b;

	auto& aColliders = a->getColliders();
	auto& bColliders = b->getColliders();
	for(auto& aCollider : aColliders)
	{
		for (auto& bCollider : bColliders)
		{
			auto direction = contact.direction_;
			bool isHit;
			// Some pair of colliders intersects
			if (intersects(a->getPosition(), aCollider.get(), b->getPosition(), bCollider.get(), direction, isHit)) {
				contact.setDirection(direction);
				contact.isHit_ = isHit;
				return true;
			}
		}
	}
	// No pairs of colliders intersect
	return false;
}
// For colliders
// direction is returned by reference if colliders do intersect
bool PhysContactEvaluator::intersects(const Vec2& posA, PhysCollider* a, const Vec2& posB, PhysCollider* b, Vec2& direction, bool& isHit)
{
	if (!a || !b)
		throw std::invalid_argument("colliders can't be null pointers");

	// If bit masks are hit-compatible
	if ((a->hitMask & b->selfMask) != 0 && (a->selfMask & b->hitMask) != 0)
		isHit = true;
	// If bit masks are overlap-compatible
	else if ((a->overlapMask & b->selfMask) != 0 && (a->selfMask & b->overlapMask) != 0)
		isHit = false;
	// If bit masks are incompatible
	else
		return false;

	const auto aC = dynamic_cast<PhysCircleCollider*>(a);
	if (aC) // a is circle
	{
		const auto bC = dynamic_cast<PhysCircleCollider*>(b);
		if (bC) // b is circle
			return intersects(posA, aC, posB, bC, direction); // circle, circle

		const auto bR = dynamic_cast<PhysBoxCollider*>(b);
		if (bR) // b is rectangle
			return intersects(posA, aC, posB, bR, direction); // circle, box

		// b is unknown
		return false;
	}

	const auto aR = dynamic_cast<PhysBoxCollider*>(a);
	if (aR)	// a is rectangle
	{
		const auto bC = dynamic_cast<PhysCircleCollider*>(b);
		if (bC) // b is circle
			return intersects(posA, aR, posB, bC, direction); // box, circle

		const auto bR = dynamic_cast<PhysBoxCollider*>(b);
		if (bR) // b is rectangle
			return intersects(posA, aR, posB, bR, direction); // box, box

		// b is unknown
		return false;
	}

	// a is unknown
	return false;
}

// circle, circle
bool PhysContactEvaluator::intersects(const Vec2& posA, PhysCircleCollider* a, const Vec2& posB, PhysCircleCollider* b, cocos2d::Vec2& direction)
{
	const auto positionA = posA + a->getPosition();
	const auto positionB = posB + b->getPosition();

	// distance(A, B) <= sum(radius A, radius B)
	const auto temp = positionA.getDistance(positionB) <= a->getRadius() + b->getRadius();
	direction = (positionB - positionA).getNormalized();
	return temp;
}

// box, box
bool PhysContactEvaluator::intersects(const Vec2& posA, PhysBoxCollider* a, const Vec2& posB, PhysBoxCollider* b, cocos2d::Vec2& direction)
{
	const auto positionA = posA + a->getPosition();
	const auto positionB = posB + b->getPosition();

	// For the sake of simplicity we only check AABB case (Axis Aligned Bounding Box)
	// That is enough for this game

	const auto& aX = positionA.x;
	const auto& aY = positionA.y;
	const auto& bX = positionB.x;
	const auto& bY = positionB.y;

	const auto xDist = std::abs(aX - bX);
	const auto yDist = std::abs(aY - bY);

	const auto sumWidth = a->getSize().width + b->getSize().width;
	const auto sumHeight = a->getSize().height + b->getSize().height;

	if (xDist > sumWidth / 2)
		return false; // Too far on X axis
	if (yDist > sumHeight / 2)
		return false; // Too far on Y axis
	
	// To get the direction, we check boxes that are a bit smaller and see where they DON'T intersect
	if (xDist > sumWidth * DIR_HELPER / 2)
		direction = Vec2(bX - aX, 0).getNormalized();
	else if (yDist > sumHeight * DIR_HELPER / 2)
		direction = Vec2(0, bY - aY).getNormalized();
	// Otherwise direction is unknown
	else direction = Vec2::ZERO;

	return true; // Intersecting on both axis
}

// circle, box
bool PhysContactEvaluator::intersects(const Vec2& posCircle, PhysCircleCollider* circle, const Vec2& posRectangle, PhysBoxCollider* rectangle, cocos2d::Vec2& direction)
{
	const auto positionC = posCircle + circle->getPosition();
	const auto positionR = posRectangle + rectangle->getPosition();

	// For the sake of simplicity we only check AABB case (Axis Aligned Bounding Box), imagining a box around circle
	// We still also check special cases at the corners
	// That is enough for this game

	const auto& cX = positionC.x;
	const auto& cY = positionC.y;
	const auto& rX = positionR.x;
	const auto& rY = positionR.y;

	const auto& radius = circle->getRadius();
	const auto hWidth = rectangle->getSize().width / 2;
	const auto hHeight = rectangle->getSize().height / 2;

	const auto xDist = std::abs(cX - rX);
	const auto yDist = std::abs(cY - rY);

	const auto sumWidth = radius + hWidth;
	const auto sumHeight = radius + hHeight;

	if (xDist > sumWidth)
		return false; // Too far on X axis
	if (yDist > sumHeight)
		return false; // Too far on Y axis

	// return true; // They are intersecting on both axis, but they may not actually be in contact

	// Check corners for special non-intersect cases
	for (auto i1 : { -1, 1 }) for (auto i2 : { -1, 1 })
		if (hWidth < i1 * (cX - rX) && hHeight < i2 * (cY - rY) &&
			positionC.distance(Vec2(rX + i1 * hWidth, rY + i2 * hHeight)) > radius)
			return false;

	// Now there is definitely intersection
	if (cX > rX - hWidth && cX < rX + hWidth && cY > rY - hHeight && cY < rY + hHeight) // center is inside
		// we check it first to avoid checking it later in other cases
		direction = Vec2::ZERO; // direction is unknown
	else if (cX >= rX - hWidth && cX <= rX + hWidth) // top or bottom
		direction = Vec2(0, cY >= rY + hHeight ? -1 : 1);
	else if (cY >= rY - hHeight && cY <= rY + hHeight) // right or left
		direction = Vec2(cX >= rX + hWidth ? -1 : 1, 0);
	else // corner intersection case
	{
		// direction = Vec2::ZERO;
		// We use the same method as in box-box intersection, trying to find direction
		// To get the direction, we check boxes that are a bit smaller and see where they DON'T intersect
		if (xDist > sumWidth * DIR_HELPER / 2)
			direction = Vec2(rX - cX, 0).getNormalized();
		else if (yDist > sumHeight * DIR_HELPER / 2)
			direction = Vec2(0, rY - cY).getNormalized();
		// Otherwise direction is unknown
		else direction = Vec2::ZERO;
	}

	// Intersection found
	return true; 
}

// box, circle
bool PhysContactEvaluator::intersects(const Vec2& posRectangle, PhysBoxCollider* rectangle, const Vec2& posCircle, PhysCircleCollider* circle, cocos2d::Vec2& direction)
{
	// it's symmetric
	const auto temp = intersects(posCircle, circle, posRectangle, rectangle, direction);
	direction = -direction; // we have to invert it
	return temp; 
}
