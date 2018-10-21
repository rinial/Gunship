#include "GameScene.h"

#include "MenuScene.h"
#include "GameOverScene.h"
#include "Gunship.h"
// TODO uncomment
// #include "Asteroid.h"
#include "Physics/Physics.h"
#include "Definitions.h"
#include <ctime>

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

// TODO move to spaceship?
constexpr unsigned int maxNumProjectiles = PROJECTILE_LIFE_TIME / SHOT_INTERVAL + 1;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

// Initialize instance
bool GameScene::init()
{
	// Super init first
	if (!Scene::init())
		return false;

	// Seed randomness
	std::srand(std::time(nullptr));

	// TODO read from file
	maxGameTime_ = 20;
	maxScore_ = 10; // number of targets
	unsigned int projectileSpeed = 100; // TODO use it

	// Background music
	SimpleAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC, true);

	// Background image
	auto backSprite = Sprite::create(BACKGROUND_SPRITE);
	backSprite->setPosition(CENTER);
	this->addChild(backSprite, Z_LEVEL_BACKGROUND);

	// Set label for targets (score) 
	scoreLabel_ = Label::createWithTTF(__String::createWithFormat("Score: %d / %d", 0, maxScore_)->getCString(), MAIN_FONT, GAME_UI_FONT_SIZE);
	const auto scoreLeftOffset = 0.04 * V_SIZE.width;
	const auto scoreTopOffset = scoreLeftOffset;
	scoreLabel_->setPosition(
		CENTER_X,
		ORIGIN.y + V_SIZE.height - scoreLabel_->getContentSize().height / 2 - scoreTopOffset);
	scoreLabel_->setWidth(V_SIZE.width - 2 * scoreLeftOffset);
	scoreLabel_->setAlignment(TextHAlignment::LEFT);
	scoreLabel_->setColor(GAME_UI_COLOR);
	scoreLabel_->enableShadow(Color4B(GAME_UI_SHADOW_COLOR), Size(1, -1) * GAME_UI_SHADOW_SIZE);
	this->addChild(scoreLabel_, Z_LEVEL_UI);

	// Set label for game time 
	gameTimeLabel_ = Label::createWithTTF(__String::createWithFormat("Time left: %d", maxGameTime_)->getCString(), MAIN_FONT, GAME_UI_FONT_SIZE);
	const auto timeRightOffset = scoreLeftOffset;
	const auto timeTopOffset = timeRightOffset;
	gameTimeLabel_->setPosition(
		CENTER_X,
		ORIGIN.y + V_SIZE.height - gameTimeLabel_->getContentSize().height / 2 - timeTopOffset);
	gameTimeLabel_->setWidth(V_SIZE.width - 2 * timeRightOffset);
	gameTimeLabel_->setAlignment(TextHAlignment::RIGHT);
	gameTimeLabel_->setColor(GAME_UI_COLOR);
	gameTimeLabel_->enableShadow(Color4B(GAME_UI_SHADOW_COLOR), Size(1, -1) * GAME_UI_SHADOW_SIZE);
	this->addChild(gameTimeLabel_, Z_LEVEL_UI);

	// Create physics world
	sceneWorld_ = std::make_unique<PhysWorld>();

	// Create edge around screen
	auto edgeBody = std::make_unique<PhysBody>(CENTER);
	edgeBody->addCollider(std::make_unique<PhysBoxCollider>(Vec2(0, (V_SIZE.height + EDGE_WIDTH) / 2), Size(V_SIZE.width, EDGE_WIDTH), EDGE_BITMASKS));  // top
	edgeBody->addCollider(std::make_unique<PhysBoxCollider>(Vec2(0, -(V_SIZE.height + EDGE_WIDTH) / 2), Size(V_SIZE.width, EDGE_WIDTH), EDGE_BITMASKS)); // bottom
	edgeBody->addCollider(std::make_unique<PhysBoxCollider>(Vec2((V_SIZE.width + EDGE_WIDTH) / 2, 0), Size(EDGE_WIDTH, V_SIZE.height), EDGE_BITMASKS));  // right
	edgeBody->addCollider(std::make_unique<PhysBoxCollider>(Vec2(-(V_SIZE.width + EDGE_WIDTH) / 2, 0), Size(EDGE_WIDTH, V_SIZE.height), EDGE_BITMASKS)); // left
	sceneWorld_->addBody(std::move(edgeBody));

	// Create a gunship in the center of the screen
	auto gunship = std::make_unique<Gunship>(CENTER);
	gunship_ = gunship.get(); // save pointer for easy access
	gunship->addToScene(this, Z_LEVEL_GUNSHIP); // add cocos2d node to scene
	sceneWorld_->addBody(std::move(gunship)); // PhysWorld controls memory

	// TODO move to gunship
	// Add all possible projectiles to pool to avoid FPS drops on creating new ones
	for (unsigned int i = 0; i < maxNumProjectiles; ++i) {
		// TODO auto projectile = std::make_unique<Projectile>(...);
		// TODO projectilesPool_.push(projectile.get()); // save pointer for easy access
		// TODO projectile->addToScene(this, Z_LEVEL_PROJECTILE); // add cocos2d node to scene
		// TODO sceneWorld_->addBody(std::move(projectile)); // PhysWorld controls memory
	}

	// Create all asteroids in random positions
	// TODO make sure they dont interfere with each other and leave space for gunship in center
	for (unsigned int i = 0; i < maxScore_; ++i) {
		// TODO auto asteroid = std::make_unique<Asteroid>(...);
		// TODO asteroid->addEventListener(this); // start listening to target events
		// TODO asteroid->addToScene(this, Z_LEVEL_TARGET); // add cocos2d node to scene
		// TODO sceneWorld_->addBody(std::move(asteroid));
	}

	// Start listening to mouse
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(GameScene::onMouseUp, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	// Start listening to keyboard
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	// Start incrementing time
	this->schedule(schedule_selector(GameScene::incrementGameTime), 1, maxGameTime_ - 1, SCENE_TRANSITION_TIME);

	// Start updating physics
	this->schedule(schedule_selector(GameScene::physicsStep), PHYSICS_UPDATE_INTERVAL, CC_REPEAT_FOREVER, SCENE_TRANSITION_TIME);

	// Start updates
	this->scheduleUpdate();

	return true;
}

// Needed to avoid problems with smart pointers
GameScene::GameScene() = default;
// Stop schedules upon destruction
GameScene::~GameScene()
{
	beforeLeavingScene();
}

// Stop schedules upon leaving scene
void GameScene::beforeLeavingScene()
{
	unscheduleAllCallbacks();
	// memory for sceneWorld_ will be freed automatically
}

// Go to game over screen
void GameScene::continueToGameOver(float dT)
{
	beforeLeavingScene(); // Stops schedules

	const auto scene = GameOverScene::createScene(score_, maxScore_, gameTime_, maxGameTime_);
	Director::getInstance()->replaceScene(SCENE_TRANSITION(scene));
}

// Go to main menu scene
void GameScene::continueToMenu(float dT)
{
	beforeLeavingScene(); // Stops schedules

	SimpleAudioEngine::getInstance()->playBackgroundMusic(MENU_BACKGROUND_MUSIC, true);

	const auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(SCENE_TRANSITION(scene));
}

// Handle mouse events
// Shoot while mouse is down
void GameScene::onMouseDown(EventMouse* event)
{
	if (!playing_)
		return;

	// TODO turn gunship's shooting mode on
}
void GameScene::onMouseUp(EventMouse* event)
{
	if (!playing_)
		return;

	// TODO turn gunship's shooting mode off
}
void GameScene::onMouseMove(cocos2d::EventMouse* event)
{
	mouseLocation_ = event->getLocationInView();
}

// Handle keyboard events
void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	// Go back to menu
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
		continueToMenu(0);
}
void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	// TODO
}

// Increment game time every second
void GameScene::incrementGameTime(float dT)
{
	if (!playing_)
		return;

	++gameTime_;

	// Update label
	gameTimeLabel_->setString(__String::createWithFormat("Time left: %d", maxGameTime_ - gameTime_)->getCString());

	// Check for loss
	if (gameTime_ >= maxGameTime_) {
		// Play sound
		SimpleAudioEngine::getInstance()->playEffect(TIME_OUT_SOUND_EFFECT);

		playing_ = false;
		this->scheduleOnce(schedule_selector(GameScene::continueToGameOver), GAME_OVER_SCENE_TRANSITION_DELAY);
	}
	else if (gameTime_ >= maxGameTime_ - 10)
		// Play sound
		SimpleAudioEngine::getInstance()->playEffect(TIME_TICK_SOUND_EFFECT);
}

// Handle target destruction
// TODO uncomment
//void GameScene::onTargetDestroyed(Target* sender)
//{
//	if (!playing_)
//		return;
//
//	++score_;
//
//	// Update label
//	scoreLabel_->setString(__String::createWithFormat("Score: %d / %d", score_, maxScore_)->getCString());
//
//	// Check for win
//	if (score_ >= maxScore_) {
//		// Play sound
//		SimpleAudioEngine::getInstance()->playEffect(WIN_SOUND_EFFECT);
//
//		playing_ = false;
//		this->scheduleOnce(schedule_selector(GameScene::continueToGameOver), GAME_OVER_SCENE_TRANSITION_DELAY);
//	}
//}

// Update physics
void GameScene::physicsStep(const float dT)
{
	sceneWorld_->step(dT);
}

// General update
void GameScene::update(const float dT)
{
	if (!playing_)
		return;

	gunship_->lookAt(mouseLocation_);
}
