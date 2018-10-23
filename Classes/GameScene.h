#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "GameObjectEventListener.h"
#include "cocos2d.h"

// Game scene where game happens
class GameScene : public cocos2d::Scene, public GameObjectEventListener
{
public:
	static cocos2d::Scene* createScene();

    virtual bool init() override;

	// static create()
	CREATE_FUNC(GameScene);

	// Needed to avoid problems with smart pointers
	GameScene();
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
	void goToMenu();
	// Go to game scene (retry)
	void retry();

	// Handle mouse events
	// Shoot while mouse is down
	void onMouseDown(cocos2d::EventMouse* event);
	void onMouseUp(cocos2d::EventMouse* event);
	void onMouseMove(cocos2d::EventMouse* event);

	// Position of the mouse
	cocos2d::Vec2 mouseLocation_;

	// Handle keyboard events
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	// Game time
	unsigned int gameTime_ = 0;
	unsigned int maxGameTime_ = 0;
	// Player's score
	unsigned int score_ = 0;
	unsigned int maxScore_ = 0;

	// Start all schedules
	void startSchedules(float dT);

	// Increment game time every second
	void incrementGameTime(float dT);
	// This is where game time is shown
	cocos2d::Label* gameTimeLabel_ = nullptr;

	// Handle target destruction
	virtual void onGameObjectBeginDestroy(class GameObject* sender) override;
	// This is where score is shown
	cocos2d::Label* scoreLabel_ = nullptr;

	// Physics
	std::unique_ptr<class PhysWorld> sceneWorld_;
	void physicsStep(float dT); // update physics

	// General update
	virtual void update(float dT) override;

	// Spaceship
	// Just a pointer, PhysWorld handles its memory
	class Gunship* gunship_ = nullptr;

	// For spaceship controls
	bool rightPressed_ = false;
	bool leftPressed_ = false;
	bool upPressed_ = false;
	bool downPressed_ = false;
	float xAxis_ = 0.0f;
	float yAxis_ = 0.0f;

	// Particles for cursor
	cocos2d::ParticleSystemQuad* cursor_;
};

#endif // __GAME_SCENE_H__
