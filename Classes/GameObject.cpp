#include "GameObject.h"
#include "Physics/Physics.h"

USING_NS_CC;

// Update position and inform the world about it
// Also set new node position to move sprites
void GameObject::setPosition(const cocos2d::Vec2& pos)
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
}

// Constructor
GameObject::GameObject(const cocos2d::Vec2& pos, const float& mass, const float& bounciness) : PhysBody(pos, mass, bounciness)
{
	rootNode_ = Node::create();
	rootNode_->setPosition(pos);
}
// Important for cleaning memory using base class pointe
GameObject::~GameObject() = default;
