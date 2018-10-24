#include "PhysWorld.h"
#include "PhysBody.h"
#include "PhysContactEvaluator.h"
#include "Definitions.h"

USING_NS_CC;

// Add/remove a body
void PhysWorld::addBody(std::unique_ptr<PhysBody> body)
{
	if (!body || !body.get())
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

	forRemoval_.insert(body);
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

// Removes body from all partitions
void PhysWorld::removeFromPartitions(PhysBody* body)
{
	for (auto& partition : partitions_)
		partition.erase(body);
}

// Finds collisions, sends events (and can move physics simulation if we were actually simulating something)
void PhysWorld::step(const float dT)
{
	// First remove all for removal
	for (auto body : forRemoval_) {
		forEvaluation_.erase(body);
		removeFromContacts(body);
		removeFromPartitions(body);
		//bodies_.erase(std::find_if(bodies_.begin(), bodies_.end(), [&body](auto b) { return body == b.get(); }));
		for (auto it = bodies_.begin(); it != bodies_.end(); ++it)
			if (it->get() == body) {
				bodies_.erase(it);
				break;
			}
	}
	forRemoval_.clear();

	// Then call steps in all active bodies
	const auto currentBodiesSize = bodies_.size();
	// Can't do that since bodies can sometimes create new bodies in their step (but can't delete)
	// for(auto body : bodies_)
	for (unsigned int i = 0; i < currentBodiesSize; ++i) {
		auto body = bodies_[i].get();
		if (body->isActive())
			body->step(dT);
	}

	// We store forEvaluation for each partition now
	std::vector<std::list<PhysBody*>> forEvaluationInPartitions(partitions_.size());

	// Update partitions
	// Partitions are needed for faster computations
	for (auto& body : forEvaluation_) {
		for (unsigned int i = 0; i < partitions_.size(); ++i) {
			if (PhysContactEvaluator::inRect(body, getPartitionsOrigin(i), partitionSize_)) {
				partitions_[i].insert(body);
				forEvaluationInPartitions[i].push_back(body);
			}
			else
				partitions_[i].erase(body);
		}
	}
	forEvaluation_.clear();

	// Now start testing for collisions
	CONTACTS_SET newContacts;
	for (unsigned int i = 0; i < partitions_.size(); ++i)
	{
		std::unordered_set<PhysBody*> testedBodies;
		for (auto& bodyA : forEvaluationInPartitions[i])
		{
			testedBodies.insert(bodyA);
			for (auto& bodyB : partitions_[i]) {
				if (testedBodies.find(bodyB) != testedBodies.end()) // if testedBodies.contains(bodyB)
					continue;

				PhysContact contact;
				if (!PhysContactEvaluator::intersects(bodyA, bodyB, contact))
					continue;

				// Contact occured, but it may be old. For now, just save it
				newContacts.insert(contact);
			}
		}
	}

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
		removeFromPartitions(body);
	}
}

// Return partition's origin
Vec2 PhysWorld::getPartitionsOrigin(const unsigned int index) const
{
	if (index > partitions_.size())
		throw std::out_of_range("index of partitions out of range");

	const auto column = index % N_PARTITIONS_X;
	const auto row = index / N_PARTITIONS_X;
	return origin_ + Vec2(column * partitionSize_.width, row * partitionSize_.height);
}

// Constructor
PhysWorld::PhysWorld(const Vec2& origin, const Size& size) : size_(size), origin_(origin)
{
	partitions_ = std::vector<std::unordered_set<PhysBody*>>(N_PARTITIONS_X * N_PARTITIONS_Y);
	partitionSize_ = Size(size_.width / N_PARTITIONS_X, size.height / N_PARTITIONS_Y);
}
// Needed to avoid problems with smart pointers
PhysWorld::~PhysWorld() = default;
