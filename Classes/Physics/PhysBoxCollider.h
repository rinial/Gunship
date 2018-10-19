#ifndef __PHYS_BOX_COLLIDER_H__
#define __PHYS_BOX_COLLIDER_H__

#include "PhysCollider.h"

// Box collider
class PhysBoxCollider : public PhysCollider
{
public:
	// Return size
	const cocos2d::Size& getSize() const { return size_; }

	// Constructors
	// Position should be first, unless it's (0, 0)
	PhysBoxCollider(const cocos2d::Vec2& pos, const cocos2d::Size& size,
		const uint16_t& selfMask = 0, const uint16_t& hitMask = 0, const uint16_t& overlapMask = 0) 
		: PhysCollider(pos, selfMask, hitMask, overlapMask) {
		if (size.width <= 0 || size.height <= 0)
			throw std::invalid_argument("size.width and size.height should be > 0");
		size_ = size;
	}
	explicit PhysBoxCollider(const cocos2d::Size& size,
		const uint16_t& selfMask = 0, const uint16_t& hitMask = 0, const uint16_t& overlapMask = 0)
		: PhysBoxCollider(cocos2d::Vec2::ZERO, size, selfMask, hitMask, overlapMask) {}

private:
	// Size of the 2d-box
	cocos2d::Size size_;
};

#endif // __PHYS_BOX_COLLIDER_H__
