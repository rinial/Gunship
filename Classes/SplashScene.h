#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"

// Screen at the start of the game
class SplashScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;

	// static create()
	CREATE_FUNC(SplashScene);

private:
	// Go to main menu
	void continueToMenu(float dT);
};

#endif // __SPLASH_SCENE_H__
