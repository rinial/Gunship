#include "GameOverScene.h"

#include "MenuScene.h"
#include "GameScene.h"
#include "Definitions.h"

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

Scene* GameOverScene::createScene(const unsigned int score, const unsigned int maxScore, const unsigned int gameTime, const unsigned int maxGameTime)
{
	::score = score;
	::maxScore = maxScore;
	::gameTime = gameTime;
	::maxGameTime = maxGameTime;

	return GameOverScene::create();
}

// Initialize instance
bool GameOverScene::init()
{
	// Super init first
	if (!Scene::init())
		return false;

	isWin_ = gameTime < maxGameTime;

	// Background music
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MENU_BACKGROUND_MUSIC, true);

	// Background image
	auto backSprite = Sprite::create(BACKGROUND_SPRITE);
	backSprite->setPosition(CENTER);
	this->addChild(backSprite, Z_LEVEL_BACKGROUND);

	// Galaxy particles
	auto galaxy = ParticleSystemQuad::create(STARS_PARTICLES);
	galaxy->setPosition(CENTER);
	this->addChild(galaxy, Z_LEVEL_STARS);

	const auto topOffset = V_SIZE.height / 5;

	// Top label
	auto topSprite = Sprite::create(isWin_ ? WIN_SPRITE : LOSS_SPRITE);
	topSprite->setPosition(CENTER_X,
		ORIGIN.y + V_SIZE.height - topOffset - topSprite->getContentSize().height / 2);
	this->addChild(topSprite, Z_LEVEL_UI);

	const auto topToNumbers = V_SIZE.height / 12;

	const auto fromCenterOffstep = V_SIZE.width / 12;

	// Set label for number
	auto numberTextLabel = Label::createWithTTF(isWin_ ? "TIME" : "SCORE", MAIN_FONT, GAME_OVER_NUMBER_TEXT_FONT_SIZE);
	numberTextLabel->setPosition(
		CENTER_X - fromCenterOffstep,
		ORIGIN.y + V_SIZE.height - topOffset - topSprite->getContentSize().height - topToNumbers - numberTextLabel->getContentSize().height / 2);
	numberTextLabel->setColor(GAME_UI_COLOR);
	numberTextLabel->enableShadow(Color4B(GAME_UI_SHADOW_COLOR), Size(1, -1) * GAME_OVER_NUMBER_TEXT_SHADOW_SIZE);
	this->addChild(numberTextLabel, Z_LEVEL_UI);

	// Set label for 'best'
	auto hNumberTextLabel = Label::createWithTTF("BEST", MAIN_FONT, GAME_OVER_NUMBER_TEXT_FONT_SIZE);
	hNumberTextLabel->setPosition(
		CENTER_X + fromCenterOffstep,
		ORIGIN.y + V_SIZE.height - topOffset - topSprite->getContentSize().height - topToNumbers - numberTextLabel->getContentSize().height / 2);
	hNumberTextLabel->setColor(GAME_UI_COLOR);
	hNumberTextLabel->enableShadow(Color4B(GAME_UI_SHADOW_COLOR), Size(1, -1) * GAME_OVER_NUMBER_TEXT_SHADOW_SIZE);
	this->addChild(hNumberTextLabel, Z_LEVEL_UI);

	const auto betweenScores = V_SIZE.height / 40;

	// Set label for actual number 
	shownNumber_ = isWin_ ? maxGameTime : 0;
	numberLabel_ = Label::createWithTTF(__String::createWithFormat("%d", shownNumber_)->getCString(), MAIN_FONT, GAME_OVER_NUMBER_FONT_SIZE);
	numberLabel_->setPosition(
		CENTER_X - fromCenterOffstep,
		ORIGIN.y + V_SIZE.height - topOffset - topSprite->getContentSize().height - topToNumbers - numberTextLabel->getContentSize().height - betweenScores - numberLabel_->getContentSize().height / 2);
	numberLabel_->setColor(GAME_UI_COLOR);
	numberLabel_->enableShadow(Color4B(GAME_UI_SHADOW_COLOR), Size(1, -1) * GAME_OVER_NUMBER_SHADOW_SIZE);
	this->addChild(numberLabel_, Z_LEVEL_UI);

	// Set label for best number 
	UserDefault* def = UserDefault::getInstance();
	bestScore = def->getIntegerForKey(BEST_SCORE_TAG, 0);
	bestGameTime = def->getIntegerForKey(BEST_TIME_TAG, maxGameTime);
	shownBNumber_ = isWin_ ? bestGameTime : bestScore; // We show previous best time on win or best score on loss
	if (score > bestScore) {
		bestScore = score;
		def->setIntegerForKey(BEST_SCORE_TAG, bestScore);
	}
	if (gameTime < bestGameTime) {
		bestGameTime = gameTime;
		def->setIntegerForKey(BEST_TIME_TAG, bestGameTime);
	}
	def->flush();
	bNumberLabel_ = Label::createWithTTF(__String::createWithFormat("%d", shownBNumber_)->getCString(), MAIN_FONT, GAME_OVER_NUMBER_FONT_SIZE);
	bNumberLabel_->setPosition(
		CENTER_X + fromCenterOffstep,
		ORIGIN.y + V_SIZE.height - topOffset - topSprite->getContentSize().height - topToNumbers - numberTextLabel->getContentSize().height - betweenScores - bNumberLabel_->getContentSize().height / 2);
	bNumberLabel_->setColor(GAME_UI_COLOR);
	bNumberLabel_->enableShadow(Color4B(GAME_UI_SHADOW_COLOR), Size(1, -1) * GAME_OVER_NUMBER_SHADOW_SIZE);
	this->addChild(bNumberLabel_, Z_LEVEL_UI);

	// Schedule incrementing shown score
	if ((isWin_ && gameTime < maxGameTime) || (!isWin_ && score > 0))
		this->scheduleOnce(schedule_selector(GameOverScene::updateShownNumber), GAME_OVER_NUMBER_MIN_DELAY + SCENE_TRANSITION_TIME);

	const auto scoreToMenu = V_SIZE.height / 15;

	// Menu
	std::vector<MenuItem*> menuItems; // We will use this vector to set positions for all items later. This way adding new items is easier
	menuItems.push_back(MenuItemImage::create(RETRY_BUTTON_NORMAL_SPRITE, RETRY_BUTTON_PRESSED_SPRITE, CC_CALLBACK_1(GameOverScene::menuRetryCallback, this))); // retry button
	menuItems.push_back(MenuItemImage::create(MENU_BUTTON_NORMAL_SPRITE, MENU_BUTTON_PRESSED_SPRITE, CC_CALLBACK_1(GameOverScene::menuMenuCallback, this))); // menu button
	menuItems.push_back(MenuItemImage::create(EXIT_BUTTON_NORMAL_SPRITE, EXIT_BUTTON_PRESSED_SPRITE, CC_CALLBACK_1(GameOverScene::menuExitCallback, this))); // exit button
	const auto itemsBottomOffset = V_SIZE.height / 5;
	auto spaceForItems = V_SIZE.height - topOffset - topSprite->getContentSize().height - topToNumbers - numberTextLabel->getContentSize().height - betweenScores - numberLabel_->getContentSize().height - scoreToMenu - itemsBottomOffset;
	spaceForItems += (spaceForItems - menuItems.size() * menuItems[0]->getContentSize().height) / (menuItems.size() - 1);
	for (unsigned int i = 0; i < menuItems.size(); ++i) {
		const auto item = menuItems[i];
		item->setPosition(CENTER_X, ORIGIN.y + itemsBottomOffset + spaceForItems - spaceForItems / menuItems.size() * (i + 1) + item->getContentSize().height / 2);
	}
	auto menu = Menu::create(menuItems[0], menuItems[1], menuItems[2], nullptr); // Make sure to add/remove items here if you add/remove them elsewhere
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, Z_LEVEL_UI);

	return true;
}

// Retry the game
void GameOverScene::menuRetryCallback(cocos2d::Ref* sender)
{
	SimpleAudioEngine::getInstance()->playEffect(CLICK_SOUND_EFFECT);
	const auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(SCENE_TRANSITION(scene));
}

// Returns to menu
void GameOverScene::menuMenuCallback(cocos2d::Ref* sender)
{
	SimpleAudioEngine::getInstance()->playEffect(CLICK_SOUND_EFFECT);
	const auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(SCENE_TRANSITION(scene));
}

// Exits the game
void GameOverScene::menuExitCallback(cocos2d::Ref* sender)
{
	SimpleAudioEngine::getInstance()->playEffect(CLICK_SOUND_EFFECT);
	Director::getInstance()->end();
}

// Changes shown score/time by one and schedules itself
void GameOverScene::updateShownNumber(float dT)
{
	// Direction of change
	const int dir = isWin_ ? -1 : 1;
	shownNumber_ += dir;

	numberLabel_->setString(__String::createWithFormat("%d", shownNumber_)->getCString());
	SimpleAudioEngine::getInstance()->playEffect(SCORE_TICK_SOUND_EFFECT);

	if (shownNumber_ * dir > shownBNumber_ * dir) {
		shownBNumber_ += dir;
		// Change shown best number along with current number if it's higher (lower) than best
		bNumberLabel_->setString(__String::createWithFormat("%d", shownBNumber_)->getCString());
	}
	if (shownNumber_ * dir < (isWin_ ? gameTime : score) * dir)
		// Updates shown number slower as it reaches actual number
		this->scheduleOnce(schedule_selector(GameOverScene::updateShownNumber), std::pow(GAME_OVER_NUMBER_MIN_DELAY + (isWin_ ? (maxGameTime - shownNumber_) / (maxGameTime - gameTime) : shownNumber_ / score) * (GAME_OVER_NUMBER_MAX_DELAY - GAME_OVER_NUMBER_MIN_DELAY), GAME_OVER_NUMBER_TICK_POWER));
}
