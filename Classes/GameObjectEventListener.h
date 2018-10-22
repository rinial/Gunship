#ifndef __CP_GAME_OBJECT_EVENT_LISTENER_H__
#define __CP_GAME_OBJECT_EVENT_LISTENER_H__

// Forward declarations
class GameObject;

// Abstract class that allows implementing onBeginContact function to handle collisions
class GameObjectEventListener
{
public:
	// Called from GameObject when it is destroyed
	virtual void onGameObjectBeginDestroy(GameObject* sender) {}

	// Can add other events here
};

#endif // __CP_GAME_OBJECT_EVENT_LISTENER_H__
