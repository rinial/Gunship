#include "SplashScene.h"

#include "MenuScene.h"
#include "Definitions.h"

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

Scene* SplashScene::createScene()
{
	return SplashScene::create();
}

// Initialize instance
bool SplashScene::init()
{
	// Super init first
	if (!Scene::init())
		return false;

	// Preload sounds
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GAME_BACKGROUND_MUSIC);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MENU_BACKGROUND_MUSIC);
	SimpleAudioEngine::getInstance()->preloadEffect(CLICK_SOUND_EFFECT);
	SimpleAudioEngine::getInstance()->preloadEffect(LASER_BOUNCE_SOUND_EFFECT);
	SimpleAudioEngine::getInstance()->preloadEffect(LASER_HIT_SOUND_EFFECT);
	SimpleAudioEngine::getInstance()->preloadEffect(SCORE_TICK_SOUND_EFFECT);
	SimpleAudioEngine::getInstance()->preloadEffect(SHOOT_NORMAL_SOUND_EFFECT);
	SimpleAudioEngine::getInstance()->preloadEffect(SHOOT_POWERFUL_SOUND_EFFECT);
	SimpleAudioEngine::getInstance()->preloadEffect(TIME_OUT_SOUND_EFFECT);
	SimpleAudioEngine::getInstance()->preloadEffect(TIME_TICK_SOUND_EFFECT);
	SimpleAudioEngine::getInstance()->preloadEffect(WIN_SOUND_EFFECT);

	// Background music
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MENU_BACKGROUND_MUSIC, true);

	// Background image
	auto splashSprite = Sprite::create(SPLASH_SCREEN_SPRITE);
	splashSprite->setPosition(CENTER);
	this->addChild(splashSprite);
		
	// Go to main menu in a bit
	this->scheduleOnce(schedule_selector(SplashScene::continueToMenu), SPLASH_SCREEN_DURATION);

	return true;
}

// Go to main menu
void SplashScene::continueToMenu(float dT)
{
	const auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(SCENE_TRANSITION(scene));
}
