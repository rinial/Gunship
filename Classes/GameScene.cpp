#include "GameScene.h"

#include "MenuScene.h"
#include "GameOverScene.h"
#include "Gunship.h"
#include "Target.h"
#include "Asteroid.h"
#include "Physics/Physics.h"
#include "Definitions.h"
#include <ctime>
#include <numeric>

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
	maxScore_ = 20; // number of targets
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

	//// Set labels for controls
	//// Menu
	//auto menuLabel = Label::createWithTTF("[ ESC ] Menu", MAIN_FONT, GAME_UI_FONT_SIZE);
	//menuLabel->setPosition(ORIGIN + menuLabel->getContentSize() / 2 + Vec2(scoreLeftOffset, scoreTopOffset));
	//menuLabel->setColor(GAME_UI_COLOR);
	//menuLabel->enableShadow(Color4B(GAME_UI_SHADOW_COLOR), Size(1, -1) * GAME_UI_SHADOW_SIZE);
	//this->addChild(menuLabel, Z_LEVEL_UI);
	//// Retry
	//auto retryLabel = Label::createWithTTF("[ R ] Retry", MAIN_FONT, GAME_UI_FONT_SIZE);
	//retryLabel->setPosition(
	//	ORIGIN.x + V_SIZE.width - retryLabel->getContentSize().width / 2 - timeRightOffset,
	//	ORIGIN.y + retryLabel->getContentSize().height / 2 + timeTopOffset);
	//retryLabel->setColor(GAME_UI_COLOR);
	//retryLabel->enableShadow(Color4B(GAME_UI_SHADOW_COLOR), Size(1, -1) * GAME_UI_SHADOW_SIZE);
	//this->addChild(retryLabel, Z_LEVEL_UI);

	// Create physics world
	sceneWorld_ = std::make_unique<PhysWorld>(ORIGIN - PARTITIONS_OUTSIDE_OFFSET * V_SIZE, V_SIZE * (1 + 2 * PARTITIONS_OUTSIDE_OFFSET));

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

	// We create it just to find normal size
	auto tempGunship = Sprite::create();
	tempGunship->initWithFile(GUNSHIP_SPRITE);
	const auto gunshipSize = tempGunship->getContentSize();

	// We create it just to find normal size
	auto tempAsteroid = Sprite::create();
	tempAsteroid->initWithFile(ASTEROID_SPRITE);
	const auto asteroidSize = tempAsteroid->getContentSize();
	const auto maxAsteroidSize = asteroidSize * ASTEROID_MAX_SCALE;

	// Rescale asteroids if too many of them have to be on the screen
	const auto square = V_SIZE.width * V_SIZE.height;
	auto cellSide = std::sqrt(square * ASTEROIDS_SPARCITY / maxScore_);
	if (cellSide > maxAsteroidSize.width) cellSide = maxAsteroidSize.width; // we don't want cells to be too big with fewer asteroids
	const unsigned int nX = std::floor(V_SIZE.width / cellSide) + 1;
	const unsigned int nY = std::floor(V_SIZE.height / cellSide) + 1;
	cellSide = std::min(V_SIZE.width / nX, V_SIZE.height / nY);
	const auto extraScale = cellSide / maxAsteroidSize.width;

	// Create cells where asteroids can be placed
	const auto nCells = nX * nY;
	std::vector<unsigned int> cellIndices(nCells);
	std::iota(cellIndices.begin(), cellIndices.end(), 0);
	std::shuffle(cellIndices.begin(), cellIndices.end(), std::mt19937(std::random_device()())); // shuffle indices
	// Create all asteroids in random positions with random speeds and scales
	unsigned int placed = 0;
	for(unsigned int i = 0; i < nCells && placed < maxScore_; ++i) {
		const auto column = cellIndices[i] % nX;
		const auto row = cellIndices[i] / nX;
		const auto center = ORIGIN + (Vec2(column, row) + Vec2(0.5, 0.5)) * cellSide;

		// Check if cell is ok (not near center)
		if (std::abs(CENTER_X - center.x) < cellSide / 2 + gunshipSize.width / 2 &&
			std::abs(CENTER_Y - center.y) < cellSide / 2 + gunshipSize.height / 2)
			continue;

		auto const relativeScale = (ASTEROID_MIN_SCALE + rand_0_1() * (ASTEROID_MAX_SCALE - ASTEROID_MIN_SCALE));
		auto scale = relativeScale * extraScale;
		const auto size = asteroidSize * scale;
		auto position = center + Vec2((cellSide - size.width) * rand_minus1_1(), (cellSide - size.height) * rand_minus1_1()) / 2;
		auto speed = Vec2::ONE.rotateByAngle(Vec2::ZERO, rand_0_1() * CC_DEGREES_TO_RADIANS(360)) // random direction
			* rand_0_1() * ASTEROID_MAX_SPEED * V_SIZE.width / (relativeScale * relativeScale);  // random magnitude
		std::unique_ptr<PhysMovement> movement;
		if (rand_0_1() > 0.5)
			movement = std::make_unique<PhysMovement>(speed);
		else {
			auto angularSpeed = rand_minus1_1() * CC_DEGREES_TO_RADIANS(ASTEROID_MAX_ANGULAR_SPEED);
			auto curveTime = ASTEROID_MIN_CURVE_TIME + rand_0_1() * (ASTEROID_MAX_CURVE_TIME - ASTEROID_MIN_CURVE_TIME);
			movement = std::make_unique<PhysLeftRightMovement>(speed, angularSpeed, curveTime);
		}
		auto asteroid = std::make_unique<Asteroid>(position, std::move(movement), scale);
		asteroid->addListener(this); // start listening to target events
		asteroid->addToScene(this, Z_LEVEL_TARGET); // add cocos2d node to scene
		sceneWorld_->addBody(std::move(asteroid));
		++placed;
	}
	// Create all asteroids in random positions with random speeds and scales
	//for (unsigned int i = placed; i < maxScore_; ++i) {
	//	// TODO !! make sure they dont interfere with each other and leave space for gunship in center
	//	auto position = ORIGIN + maxAsteroidSize / 2 + Vec2((V_SIZE.width - maxAsteroidSize.width) * rand_0_1(), (V_SIZE.height - maxAsteroidSize.height) * rand_0_1()); 
	//	auto scale = (ASTEROID_MIN_SCALE + rand_0_1() * (ASTEROID_MAX_SCALE - ASTEROID_MIN_SCALE)) * extraScale;
	//	auto speed = Vec2::ONE.rotateByAngle(Vec2::ZERO, rand_0_1() * CC_DEGREES_TO_RADIANS(360)) // random direction
	//	             * rand_0_1() * ASTEROID_MAX_SPEED * V_SIZE.width / (scale * scale);         // random magnitude
	//	std::unique_ptr<PhysMovement> movement;
	//	if (rand_0_1() > 0.5)
	//		movement = std::make_unique<PhysMovement>(speed);
	//	else {
	//		auto angularSpeed = rand_minus1_1() * CC_DEGREES_TO_RADIANS(ASTEROID_MAX_ANGULAR_SPEED);
	//		auto curveTime = ASTEROID_MIN_CURVE_TIME + rand_0_1() * (ASTEROID_MAX_CURVE_TIME - ASTEROID_MIN_CURVE_TIME);
	//		movement = std::make_unique<PhysLeftRightMovement>(speed, angularSpeed, curveTime);
	//	}
	//	auto asteroid = std::make_unique<Asteroid>(position, std::move(movement), scale);
	//	asteroid->addListener(this); // start listening to target events
	//	asteroid->addToScene(this, Z_LEVEL_TARGET); // add cocos2d node to scene
	//	sceneWorld_->addBody(std::move(asteroid));
	//}

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

	// We start all schedule only after scene transition is finished
	this->scheduleOnce(schedule_selector(GameScene::startSchedules), SCENE_TRANSITION_TIME);

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
void GameScene::goToMenu()
{
	beforeLeavingScene(); // Stops schedules

	SimpleAudioEngine::getInstance()->playBackgroundMusic(MENU_BACKGROUND_MUSIC, true);

	const auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(SCENE_TRANSITION(scene));
}

// Go to game scene (retry)
void GameScene::retry()
{
	beforeLeavingScene(); // Stops schedules

	const auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(SCENE_TRANSITION(scene));
}

// Handle mouse events
// Shoot while mouse is down
void GameScene::onMouseDown(EventMouse* event)
{
	gunship_->startShooting();
}
void GameScene::onMouseUp(EventMouse* event)
{
	gunship_->stopShooting();
}
void GameScene::onMouseMove(EventMouse* event)
{
	mouseLocation_ = event->getLocationInView();
}

// Handle keyboard events
void GameScene::onKeyPressed(const EventKeyboard::KeyCode keyCode, Event* event)
{
	switch(keyCode)
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		goToMenu();
		break;
	case EventKeyboard::KeyCode::KEY_R:
		retry();
		break;
	default:
		break;
	}
}
void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	// TODO
}

// Start all schedules
void GameScene::startSchedules(float dT)
{
	// Start incrementing time
	this->schedule(schedule_selector(GameScene::incrementGameTime), 1, maxGameTime_ - 1, 0);
	// Start updating physics
	this->schedule(schedule_selector(GameScene::physicsStep), PHYSICS_UPDATE_INTERVAL);
	// Start general updates
	this->scheduleUpdate();
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
void GameScene::onGameObjectBeginDestroy(GameObject* sender)
{
	if (!playing_)
		return;

	const auto target = dynamic_cast<Target*>(sender);
	if (!target)
		return;

	++score_;

	// Update label
	scoreLabel_->setString(__String::createWithFormat("Score: %d / %d", score_, maxScore_)->getCString());

	// Check for win
	if (score_ >= maxScore_) {
		// Play sound
		SimpleAudioEngine::getInstance()->playEffect(WIN_SOUND_EFFECT);

		playing_ = false;
		this->scheduleOnce(schedule_selector(GameScene::continueToGameOver), GAME_OVER_SCENE_TRANSITION_DELAY);
	}
}

// Update physics
void GameScene::physicsStep(const float dT)
{
	sceneWorld_->step(dT);
}

// General update
void GameScene::update(const float dT)
{
	gunship_->lookAt(mouseLocation_);
}
