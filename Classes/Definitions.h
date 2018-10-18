#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

// Visible origin, size (of screen) and center
#define ORIGIN Director::getInstance()->getVisibleOrigin()
#define V_SIZE Director::getInstance()->getVisibleSize()
#define CENTER_X (ORIGIN.x + V_SIZE.width / 2.0)   // sometimes we only 
#define CENTER_Y (ORIGIN.y + V_SIZE.height / 2.0) // need one axis
#define CENTER Vec2(CENTER_X, CENTER_Y) 

// For UI
#define MAIN_FONT "fonts/y2k.ttf"
#define GREEN_COLOR Color3B(111, 185, 109) // 40, 210, 35
#define GAME_SCORE_COLOR Color3B::WHITE
#define GAME_SCORE_SHADOW_COLOR GREEN_COLOR
#define GAME_SCORE_SHADOW_SIZE 0.04 // depends on size
#define GAME_SCORE_FONT_SIZE (0.04 * V_SIZE.height)
// TODO move all 4 offsets to code?
#define GAME_SCORE_X_OFFSET (0.07 * V_SIZE.width) // from left 
#define GAME_SCORE_Y_OFFSET GAME_SCORE_X_OFFSET // from top
#define GAME_TIME_X_OFFSET (0.07 * V_SIZE.width) // from right
#define GAME_TIME_Y_OFFSET GAME_SCORE_X_OFFSET // from top
#define GAME_OVER_TIME_TEXT_FONT_SIZE (0.035 * V_SIZE.height)
#define GAME_OVER_TIME_FONT_SIZE (0.08 * V_SIZE.height)
#define GAME_OVER_TIME_MIN_DELAY 0.17
#define GAME_OVER_TIME_MAX_DELAY 0.55

// For memory
#define BEST_TIME_TAG "BestTime"

// For sprites
#define BACKGROUND_SPRITE "sprites/Background.png"
#define SPLASH_SCREEN_SPRITE "sprites/SplashScreen.png"
#define TITLE_SPRITE "sprites/Title.png"
#define EXIT_BUTTON_NORMAL_SPRITE "sprites/ExitButtonNormal.png"
#define EXIT_BUTTON_PRESSED_SPRITE "sprites/ExitButtonPressed.png"
#define PLAY_BUTTON_NORMAL_SPRITE "sprites/PlayButtonNormal.png"
#define PLAY_BUTTON_PRESSED_SPRITE "sprites/PlayButtonPressed.png"
#define MENU_BUTTON_NORMAL_SPRITE "sprites/MenuButtonNormal.png"
#define MENU_BUTTON_PRESSED_SPRITE "sprites/MenuButtonPressed.png"
#define RETRY_BUTTON_NORMAL_SPRITE "sprites/RetryButtonNormal.png"
#define RETRY_BUTTON_PRESSED_SPRITE "sprites/RetryButtonPressed.png"
#define GUNSHIP_SPRITE "sprites/Gunship.png"
#define GUN_SPRITE "sprites/Gun.png"
#define PROJECTILE_SPRITE "sprites/Projectile.png"
#define ASTEROID_SPRITE "sprites/Asteroid.png"

// For sounds
#define GAME_BACKGROUND_MUSIC "audio/bgGame.mp3"
#define MENU_BACKGROUND_MUSIC "audio/bgMenu.wav"
#define CLICK_SOUND_EFFECT "audio/click.wav"
#define LASER_BOUNCE_SOUND_EFFECT "audio/laserBounce.wav"
#define LASER_HIT_SOUND_EFFECT "audio/laserHit.wav"
#define SCORE_TICK_SOUND_EFFECT "audio/scoreTick.wav"
#define SHOOT_NORMAL_SOUND_EFFECT "audio/shootNormal.wav"
#define SHOOT_POWERFUL_SOUND_EFFECT "audio/shootPowerful.wav"
#define TIME_OUT_SOUND_EFFECT "audio/timeOut.wav"
#define TIME_TICK_SOUND_EFFECT "audio/timeTick.wav"

// For scene transitions and durations
#define SCENE_TRANSITION_TIME 0.5
#define SCENE_TRANSITION_TYPE TransitionCrossFade
#define SCENE_TRANSITION(scene) SCENE_TRANSITION_TYPE::create(SCENE_TRANSITION_TIME, scene)
#define GAME_OVER_SCENE_TRANSITION_DELAY 1.5 // delay after game is over and before going to game over screen
#define SPLASH_SCREEN_DURATION 1.0


// General
// TODO read from file
#define GAME_TIME 30
// TODO read from file
#define NUM_OF_ASTEROIDS 10

// For gunship
// TODO read from file
#define PROJECTILE_SPEED 200

// For physics
#define PHYSICS_UPDATE_INTERVAL (1.0 / 60)
#define COLLISION_BITMASK_ALL        0b11111111
#define COLLISION_BITMASK_GUNSHIP    0b00000001
#define COLLISION_BITMASK_ASTEROID   0b00000010
#define COLLISION_BITMASK_PROJECTILE 0b00000100
#define COLLISION_BITMASK_EDGE	     0b00001000

#endif // __DEFINITIONS_H__
