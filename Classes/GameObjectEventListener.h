#ifndef __CP_GAME_OBJECT_EVENT_LISTENER_H__
#define __CP_GAME_OBJECT_EVENT_LISTENER_H__

// Forward declarations
class GameObject;

// Abstract class that allows handling events of GameObject
class GameObjectEventListener
{
public:
	// Called when GameObject is destroyed
	virtual void onGameObjectBeginDestroy(GameObject* sender) {}

	// Called when GameObject is activated
	virtual void onGameObjectActivated(GameObject* sender) {}

	// Called when GameObject is deactivated
	virtual void onGameObjectDeactivated(GameObject* sender) {}
};

#endif // __CP_GAME_OBJECT_EVENT_LISTENER_H__
