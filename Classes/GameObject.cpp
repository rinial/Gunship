#include "GameObject.h"
#include "GameObjectEventListener.h"
#include "Physics/Physics.h"

USING_NS_CC;

// Increment life time
void GameObject::step(const float dT)
{
	PhysBody::step(dT);

	lifeTime_ += dT;
}

// Add/remove listeners
void GameObject::addListener(GameObjectEventListener* listener)
{
	if (!listener)
		return;
	listeners_.insert(listener);
}
void GameObject::removeListener(GameObjectEventListener* listener)
{
	if (!listener)
		return;
	listeners_.erase(listener);
}

// Update position and inform the world about it
// Also set new node position to move sprites
void GameObject::setPosition(const Vec2& pos)
{
	PhysBody::setPosition(pos);
	rootNode_->setPosition(pos);
}

// Set activeness and inform the world
// Also changes visibility
void GameObject::setActive(const bool active)
{
	PhysBody::setActive(active);
	rootNode_->setVisible(active);

	// Send event
	if (active)
		for (auto listener : listeners_)
			listener->onGameObjectActivated(this);
	else
		for (auto listener : listeners_)
			listener->onGameObjectDeactivated(this);
}

// Adds game object to scene
void GameObject::addToScene(Scene* scene, const int zLevel)
{
	if (!scene)
		throw std::invalid_argument("scene can't be nullptr");
	scene->addChild(rootNode_, zLevel);
	sceneNode_ = scene;
}

// Destroy this object
void GameObject::destroy()
{
	// Send event
	for (auto listener : listeners_)
		listener->onGameObjectBeginDestroy(this);

	onDestroy();
	rootNode_->removeFromParentAndCleanup(true);
	getWorld()->removeBody(this);
}

// Constructor
GameObject::GameObject(const Vec2& pos, const float& mass, const float& bounciness) : PhysBody(pos, mass, bounciness)
{
	rootNode_ = Node::create();
	rootNode_->setPosition(pos);
}
// Important for cleaning memory using base class pointe
GameObject::~GameObject() = default;
