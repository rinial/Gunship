#ifndef __PHYS_CONTACT_H__
#define __PHYS_CONTACT_H__

#include "cocos2d.h" // Just for basic things like Vec2

// Forward declarations
class PhysBody;

// Represents special object that holds information about collision
class PhysContact
{
	// We let contact evaluator access a_, b_ and direction_ directly
	friend class PhysContactEvaluator;

public:
	// For hashing
	// We consider contacts equal if bodies are equal even if directions are not
	struct PhysContactHasher
	{
		std::size_t operator()(const PhysContact& contact) const
		{
			return std::hash<PhysBody*>()(contact.a_) ^ std::hash<PhysBody*>()(contact.b_);
		}
	};
	bool operator== (const PhysContact& other) const
	{
		return (a_ == other.a_ && b_ == other.b_) || (a_ == other.b_ && b_ == other.a_);
	}

	PhysContact(PhysBody* a, PhysBody* b, const cocos2d::Vec2& direction) {
		if (!a || !b)
			throw std::invalid_argument("bodies can't be null pointers");
		a_ = a;
		b_ = b;
		setDirection(direction);
	}

	// Gives access to collided objects and contact direction
	PhysBody* getBodyA() const { return a_; }
	PhysBody* getBodyB() const { return b_; }
	const cocos2d::Vec2& getDirection() const { return direction_; }

	// Gives other object based on argument
	PhysBody* getOther(PhysBody* body) const {
		if (!body)
			throw std::invalid_argument("body can't be nullptr");
		if (a_ == body)
			return b_;
		if (b_ == body)
			return a_;
		throw std::invalid_argument("this body is not part of contact");
	}

	// Gives direction from body
	cocos2d::Vec2 getDirectionFrom(PhysBody* body) const {
		if (!body)
			throw std::invalid_argument("body can't be nullptr");
		if (a_ == body)
			return direction_;
		if (b_ == body)
			return -direction_;
		throw std::invalid_argument("this body is not part of contact");
	}

private:

	PhysBody* a_ = nullptr;
	PhysBody* b_ = nullptr;

	// Direction of normal to collision from a to b
	cocos2d::Vec2 direction_;

	// Direction should always be set with this function
	void setDirection(const cocos2d::Vec2& direction) {
		if (direction.isZero())
			return; // sometimes direction can be unknown
			// throw std::invalid_argument("direction can't be (0, 0)");
		direction_ = direction.getNormalized();
	}
};

#endif // __PHYS_CONTACT_H__
