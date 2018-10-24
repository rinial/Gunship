#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Physics/PhysBody.h"
#include "cocos2d.h"
#include <unordered_set>

// Forward declarations
class GameObjectEventListener;

// Basic game object that has (is) a physics body and cointains a scene node to attach sprites to
class GameObject : public PhysBody
{
public:
	// Increment life time
	virtual void step(float dT) override;

	// Add/remove listeners
	void addListener(GameObjectEventListener* listener);
	void removeListener(GameObjectEventListener* listener);

	// Update position and inform the world about it
	// Also set new node position to move sprites
	virtual void setPosition(const cocos2d::Vec2& pos) override;

	// Set activeness and inform the world
	// Also changes visibility
	virtual void setActive(bool active) override;

	// Adds game object to scene
	virtual void addToScene(cocos2d::Scene* scene, int zLevel = 0);

	// Return life time
	float getLifeTime() const { return lifeTime_; }

	// Reset game object to initial state
	virtual void reset() { lifeTime_ = 0; }

	// False if destroy() was called
	bool isAlive() const { return isAlive_; }

	// Destroy this object, calls onDestroy first for children to add functionality
	void destroy();
protected:
	virtual void onDestroy() {}

public:
	// Constructor
	explicit GameObject(const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO, const float& mass = 1, const float& bounciness = 1);
	// Destructor
	virtual ~GameObject();

protected:
	// Root of the object
	cocos2d::Node* rootNode_;
	// Scene
	cocos2d::Scene* sceneNode_ = nullptr;

private:
	// Event listeners
	std::unordered_set<GameObjectEventListener*> listeners_;

	// Life time of this object
	float lifeTime_ = 0;

	// False if destroy() was called
	// Usually needed only when two GameObjects are interacting
	// No need to check if object is alive in every method
	bool isAlive_ = true;
};

#endif // __GAME_OBJECT_H__
