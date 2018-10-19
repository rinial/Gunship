#ifndef __PHYS_CIRCLE_COLLIDER_H__
#define __PHYS_CIRCLE_COLLIDER_H__

#include "PhysCollider.h"

// Circle collider
class PhysCircleCollider : public PhysCollider
{
public:
	// Returns radius
	const float& getRadius() const { return radius_; }

	// Constructors
	// Position should be first, unless it's (0, 0)
	PhysCircleCollider(const cocos2d::Vec2& pos, const float& radius,
		const uint16_t& selfMask = 0, const uint16_t& hitMask = 0, const uint16_t& overlapMask = 0)
		: PhysCollider(pos, selfMask, hitMask, overlapMask) {
		if (radius <= 0)
			throw std::invalid_argument("radius should be > 0");
		radius_ = radius;
	}
	explicit PhysCircleCollider(const float& radius,
		const uint16_t& selfMask = 0, const uint16_t& hitMask = 0, const uint16_t& overlapMask = 0)
		: PhysCircleCollider(cocos2d::Vec2::ZERO, radius, selfMask, hitMask, overlapMask) {}

private:
	// Radius of the circle
	float radius_;
};

#endif // __PHYS_CIRCLE_COLLIDER_H__
