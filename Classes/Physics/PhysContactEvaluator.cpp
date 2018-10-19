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
			// Some pair of colliders intersects
			if (intersects(aCollider.get(), bCollider.get(), direction)) {
				contact.setDirection(direction);
				return true;
			}
		}
	}
	// No pairs of colliders intersect
	return false;
}
// For colliders
// direction is returned by reference if colliders do intersect
bool PhysContactEvaluator::intersects(PhysCollider* a, PhysCollider* b, cocos2d::Vec2& direction)
{
	if (!a || !b)
		throw std::invalid_argument("colliders can't be null pointers");

	// If bit masks are not compatible
	if (((a->hitMask & b->selfMask) == 0 && (a->overlapMask & b->selfMask) == 0) || 
		((a->selfMask & b->hitMask) == 0 && (a->selfMask & b->overlapMask) == 0))
		return false;

	const auto aC = dynamic_cast<PhysCircleCollider*>(a);
	if (aC) // a is circle
	{
		const auto bC = dynamic_cast<PhysCircleCollider*>(b);
		if (bC) // b is circle
			return intersects(aC, bC, direction); // circle, circle

		const auto bR = dynamic_cast<PhysBoxCollider*>(b);
		if (bR) // b is rectangle
			return intersects(aC, bR, direction); // circle, box

		// b is unknown
		return false;
	}

	const auto aR = dynamic_cast<PhysBoxCollider*>(a);
	if (aR)	// a is rectangle
	{
		const auto bC = dynamic_cast<PhysCircleCollider*>(b);
		if (bC) // b is circle
			return intersects(aR, bC, direction); // box, circle

		const auto bR = dynamic_cast<PhysBoxCollider*>(b);
		if (bR) // b is rectangle
			return intersects(aR, bR, direction); // box, box

		// b is unknown
		return false;
	}

	// a is unknown
	return false;
}

// circle, circle
bool PhysContactEvaluator::intersects(PhysCircleCollider* a, PhysCircleCollider* b, cocos2d::Vec2& direction)
{
	// distance(A, B) <= sum(radius A, radius B)
	const auto temp = a->getPosition().getDistance(b->getPosition()) <= a->getRadius() + b->getRadius();
	direction = (b->getPosition() - a->getPosition()).getNormalized();
	return temp;
}

// box, box
bool PhysContactEvaluator::intersects(PhysBoxCollider* a, PhysBoxCollider* b, cocos2d::Vec2& direction)
{
	// For the sake of simplicity we only check AABB case (Axis Aligned Bounding Box)
	// That is enough for this game

	const auto& aX = a->getPosition().x;
	const auto& aY = a->getPosition().y;
	const auto& bX = b->getPosition().x;
	const auto& bY = b->getPosition().y;

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
bool PhysContactEvaluator::intersects(PhysCircleCollider* circle, PhysBoxCollider* rectangle, cocos2d::Vec2& direction)
{
	// For the sake of simplicity we only check AABB case (Axis Aligned Bounding Box), imagining a box around circle
	// We still also check special cases at the corners
	// That is enough for this game

	const auto& cX = circle->getPosition().x;
	const auto& cY = circle->getPosition().y;
	const auto& rX = rectangle->getPosition().x;
	const auto& rY = rectangle->getPosition().y;

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
			circle->getPosition().distance(Vec2(rX + i1 * hWidth, rY + i2 * hHeight)) > radius)
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
bool PhysContactEvaluator::intersects(PhysBoxCollider* rectangle, PhysCircleCollider* circle, cocos2d::Vec2& direction)
{
	// it's symmetric
	const auto temp = intersects(circle, rectangle, direction);
	direction = -direction; // we have to invert it
	return temp; 
}
