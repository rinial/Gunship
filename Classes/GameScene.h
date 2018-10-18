#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

// #include "TargetEventsListener.h" // TODO uncomment
#include "cocos2d.h"

// Game scene where game happens
class GameScene : public cocos2d::Scene // TODO , public TargetEventsListener
{
public:
	static cocos2d::Scene* createScene();

    virtual bool init() override;

	// static create()
	CREATE_FUNC(GameScene);

	// Stop schedules upon destruction
	~GameScene();

private:
	// True if game is on
	bool playing_ = true;

	// Stop schedules upon leaving scene
	void beforeLeavingScene();

	// Go to game over scene
	void continueToGameOver(float dT);
	// Go to main menu scene
	void continueToMenu(float dT);

	// Handle mouse events
	// Shoot while mouse is down
	void onMouseDown(cocos2d::EventMouse* event);
	void onMouseUp(cocos2d::EventMouse* event);

	// Handle keyboard events
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	// Game time
	unsigned int gameTime_ = 0;
	unsigned int maxGameTime_ = 0;
	// Player's score
	unsigned int score_ = 0;
	unsigned int maxScore_ = 0;

	// Increment game time every second
	void incrementGameTime(float dT);
	// This is where game time is shown
	cocos2d::Label* gameTimeLabel_ = nullptr;

	// Handle target destruction
	// TODO virtual void onTargetDestroyed(class Target* sender) override;
	// This is where score is shown
	cocos2d::Label* scoreLabel_ = nullptr;

	// Physics
	// TODO std::unique_ptr<PhysWorld> sceneWorld_ = nullptr;
	void physicsStep(float dT); // update physics

	// Spaceship
	// Just a pointer, PhysWorld handles its memory
	class Gunship* gunship_ = nullptr;

	// TODO move to gunship
	// Pool of all projectiles
	// Just pointers, PhysWorld handles their memory
	std::queue<class Projectile*> projectilesPool_;
};

#endif // __GAME_SCENE_H__
