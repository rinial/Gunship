#include "PhysBody.h"
#include "PhysContact.h"
#include "PhysMovement.h"

USING_NS_CC;

// Set the body that will be changed by this movement_
// Should only be called from PhysBody directly when adding movement_
void PhysMovement::setBody(PhysBody* body)
{
	if (!body)
		throw std::invalid_argument("body can't be nullptr");
	body_ = body;
}

// Called from PhysBody on hits as it can affect movement
void PhysMovement::onHit(const PhysContact& contact)
{
	// Don'd do anything if direction is unknown
	if (contact.getDirection().isZero())
		return;

	// bodies
	const auto a = body_;
	const auto b = contact.getOther(body_);

	// bounciness
	const auto bounciness = std::max(a->getBounciness(), b->getBounciness());

	// direction of impact
	const auto n = contact.getDirectionFrom(a);
	// perpendicular to it
	const auto p = n.getPerp();

	// a on p
	const auto pA = speed_.project(p);

	if (b->isKinematic())
	{
		// a on n
		const auto nA = speed_.project(n);

		// calculate speed
		speed_ = pA - bounciness * nA;
	}
	else
	{
		// masses
		const auto mA = a->getMass();
		const auto mB = b->getMass();

		// speeds
		const auto sA = speed_;
		const auto sB = b->getMovement()->getSpeed();

		// speeds on line of impact
		const auto nA = sA.project(n);
		const auto nB = sB.project(n);

		// calculate speed
		speed_ = pA + (nA * mA + nB * mB + bounciness * mB * (nB - nA)) / (mA + mB);
	}
}

// Evaluates body movement over a period of time
void PhysMovement::move(const float dT)
{
	body_->setPosition(body_->getPosition() + speed_ * dT);
}
