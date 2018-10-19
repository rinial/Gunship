#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"

// Player's score
static unsigned int score = 0;
static unsigned int bestScore = 0;
static unsigned int maxScore = 0;

// Game time
static unsigned int gameTime = 0;
static unsigned int bestGameTime = 0;
static unsigned int maxGameTime = 0;

// Game over screen
class GameOverScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene(unsigned int score, unsigned int maxScore, unsigned int gameTime, unsigned int maxGameTime);

    virtual bool init() override;
    
    // static create()
    CREATE_FUNC(GameOverScene);

private:
	// Retry the game
	void menuRetryCallback(cocos2d::Ref* sender);

	// Returns to menu
	void menuMenuCallback(cocos2d::Ref* sender);

	// Exits the game
	void menuExitCallback(cocos2d::Ref* sender);

	// Score/Time that is currently shown on the screen
	unsigned int shownNumber_ = 0;
	// This is where number is shown
	cocos2d::Label* numberLabel_ = nullptr;

	// Best number that is currently shown on the screen
	unsigned int shownBNumber_ = 0;
	// This is where best number is shown
	cocos2d::Label* bNumberLabel_ = nullptr;

	// Changes shown score/time by one and schedules itself
	void updateShownNumber(float dT);

	// True if it's a win
	bool isWin_ = false;
};

#endif // __GAME_OVER_SCENE_H__
