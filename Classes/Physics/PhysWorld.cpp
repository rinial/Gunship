#include "PhysWorld.h"
#include "PhysBody.h"
#include "PhysContactEvaluator.h"

// Add/remove a body
void PhysWorld::addBody(std::unique_ptr<PhysBody> body)
{
	if (!body)
		throw std::invalid_argument("body can't be nullptr");

	body->setWorld(this);
	if (body->isActive())
		forEvaluation_.insert(body.get());
	bodies_.push_back(std::move(body));
}
void PhysWorld::removeBody(PhysBody* body)
{
	if (!body)
		return; // it's ok to 'remove' nullptr from the bodies_
		// throw std::invalid_argument("body can't be nullptr");

	forEvaluation_.erase(body);
	removeFromContacts(body);
	//bodies_.erase(std::find_if(bodies_.begin(), bodies_.end(), [&body](auto b) { return body == b.get(); }));
	for (auto it = bodies_.begin(); it != bodies_.end(); ++it)
		if (it->get() == body) {
			bodies_.erase(it);
			break;
		}
}

// Removes all contacts with specific body from currentContacts_
void PhysWorld::removeFromContacts(PhysBody* body)
{
	while (true) {
		const auto foundIt = std::find_if(currentContacts_.begin(), currentContacts_.end(), [&body](auto contact) { return contact.getBodyA() == body || contact.getBodyB() == body; });
		if (foundIt != currentContacts_.end())
			currentContacts_.erase(foundIt);
		else
			break;
	}
}

// Finds collisions, sends events (and can move physics simulation if we were actually simulating something)
void PhysWorld::step(const float dT)
{
	// First call steps in all active bodies
	for (auto& body : bodies_)
		if (body->isActive())
			body->step(dT);

	// Now start testing for collisions
	std::unordered_set<PhysBody*> testedBodies;
	CONTACTS_SET newContacts;

	// Check only those that have been changed
	for (auto& bodyA : forEvaluation_)
	{
		testedBodies.insert(bodyA);

		// We test it with every other body in the world, except for those that are already tested in their evaluation cycle
		// TODO add space partioning to lessen the number of checks
		for (auto& bodyB : bodies_) // bodyA is a simple pointer, bodyB is a unique one. Thus some differences
		{
			if (testedBodies.find(bodyB.get()) != testedBodies.end()) // if testedBodies.contains(bodyB)
				continue;

			PhysContact contact;
			if (!PhysContactEvaluator::intersects(bodyA, bodyB.get(), contact))
				continue;

			// Contact occured, but it may be old. For now, just save it
			newContacts.insert(contact);
		}
	}
	// We have evaluated all that we had for evaluation
	forEvaluation_.clear();

	// We now have all the contacts and need to find, which ones are new
	// First we remove old contacts
	std::vector<PhysContact> forRemovalFromCurrent;
	for (const auto& contact : currentContacts_)
	{
		if (newContacts.find(contact) != newContacts.end()) // if newContacts.contains(contact)
			newContacts.erase(contact); // it's an old contact, remove from new
		else
			forRemovalFromCurrent.push_back(contact); // it's not a contact anymore, mark from removal from current
	}

	// Remove old contacts that are no longer contacts
	for (const auto& contact : forRemovalFromCurrent)
		currentContacts_.erase(contact);

	// Add new contacts to current and notify bodies
	for (const auto& contact : newContacts)
	{
		currentContacts_.insert(contact);

		// Decide if it's hit or overlap
		if (contact.isHit()) {
			contact.getBodyA()->onHit(contact);
			contact.getBodyB()->onHit(contact);
		}
		else {
			contact.getBodyA()->onOverlap(contact);
			contact.getBodyB()->onOverlap(contact);
		}
	}
}

// Called from bodies when they are moved or changed in other ways
// Sets these bodies for evaluation, or removes them from evaluation if they are not active anymore
void PhysWorld::onManipulatedBody(PhysBody* body)
{
	if (!body)
		throw std::invalid_argument("body can't be nullptr");

	if (body->isActive())
		forEvaluation_.insert(body);
	else
	{
		forEvaluation_.erase(body);
		removeFromContacts(body);
	}
}

// Needed to avoid problems with smart pointers
PhysWorld::PhysWorld() {}
PhysWorld::~PhysWorld() {}
