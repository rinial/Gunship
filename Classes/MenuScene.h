#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

// Main menu of the game
class MenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    
    // static create()
    CREATE_FUNC(MenuScene);

private:
	// Start the game
	void menuPlayCallback(cocos2d::Ref* sender);

	// Exit the game
	void menuExitCallback(cocos2d::Ref* sender);
};

#endif // __MENU_SCENE_H__
