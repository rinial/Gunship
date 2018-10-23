#include "MenuScene.h"
#include "GameScene.h"
#include "Definitions.h"

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

Scene* MenuScene::createScene()
{
	return MenuScene::create();
}

// Initialize instance
bool MenuScene::init()
{
	// Super init first
	if (!Scene::init())
		return false;

	// Background image
	auto backSprite = Sprite::create(BACKGROUND_SPRITE);
	backSprite->setPosition(CENTER);
	this->addChild(backSprite, Z_LEVEL_BACKGROUND);

	// Galaxy particles
	auto galaxy = ParticleSystemQuad::create(STARS_PARTICLES);
	galaxy->setPosition(CENTER);
	this->addChild(galaxy, Z_LEVEL_STARS);

	// Title image on the top right
	auto titleSprite = Sprite::create(TITLE_SPRITE);
	const auto titleTopOffset = V_SIZE.height / 8;
	const auto titleRightOffset = titleTopOffset;
	titleSprite->setPosition(
		ORIGIN.x + V_SIZE.width - titleRightOffset - titleSprite->getContentSize().width / 2, 
		ORIGIN.y + V_SIZE.height - titleTopOffset - titleSprite->getContentSize().height / 2);
	this->addChild(titleSprite, Z_LEVEL_UI);

	// Menu
	std::vector<MenuItem*> menuItems; // We will use this vector to set positions for all items later. This way adding new items is easier
	menuItems.push_back(MenuItemImage::create(PLAY_BUTTON_NORMAL_SPRITE, PLAY_BUTTON_PRESSED_SPRITE, CC_CALLBACK_1(MenuScene::menuPlayCallback, this))); // play button
	menuItems.push_back(MenuItemImage::create(EXIT_BUTTON_NORMAL_SPRITE, EXIT_BUTTON_PRESSED_SPRITE, CC_CALLBACK_1(MenuScene::menuExitCallback, this))); // exit button
	const auto itemsBottomOffset = V_SIZE.height / 8;
	const auto itemsLeftOffset = itemsBottomOffset;
	const auto spaceForItems = V_SIZE.height / 6;
	for (unsigned int i = 0; i < menuItems.size(); ++i) {
		const auto item = menuItems[i];
		item->setPosition(itemsLeftOffset + item->getContentSize().width / 2, ORIGIN.y + itemsBottomOffset + spaceForItems - spaceForItems / menuItems.size() * (i + 1) + item->getContentSize().height / 2);
	}
	// Make sure to add/remove items here if you add/remove them elsewhere
	auto menu = Menu::create(menuItems[0], menuItems[1], nullptr); 
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, Z_LEVEL_UI);

	return true;
}

// Start the game
void MenuScene::menuPlayCallback(cocos2d::Ref* sender)
{
	SimpleAudioEngine::getInstance()->playEffect(CLICK_SOUND_EFFECT);
	const auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(SCENE_TRANSITION(scene));
}

// Exits the game
void MenuScene::menuExitCallback(cocos2d::Ref* sender)
{
	SimpleAudioEngine::getInstance()->playEffect(CLICK_SOUND_EFFECT);
	Director::getInstance()->end();
}
