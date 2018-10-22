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
#define GAME_UI_COLOR Color3B::WHITE
#define GAME_UI_SHADOW_COLOR GREEN_COLOR
#define GAME_UI_FONT_SIZE (0.04 * V_SIZE.height)
#define GAME_UI_SHADOW_SIZE (GAME_UI_FONT_SIZE * 0.04)
#define GAME_OVER_NUMBER_TEXT_FONT_SIZE (0.04 * V_SIZE.height)
#define GAME_OVER_NUMBER_TEXT_SHADOW_SIZE (GAME_OVER_NUMBER_TEXT_FONT_SIZE * 0.04)
#define GAME_OVER_NUMBER_FONT_SIZE (0.08 * V_SIZE.height)
#define GAME_OVER_NUMBER_SHADOW_SIZE (GAME_OVER_NUMBER_FONT_SIZE * 0.04)
#define GAME_OVER_NUMBER_MIN_DELAY 0.17
#define GAME_OVER_NUMBER_MAX_DELAY 0.55
#define GAME_OVER_NUMBER_TICK_POWER 1.3

// For memory
#define BEST_SCORE_TAG "BestScore"
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
#define WIN_SPRITE "sprites/WinLabel.png"
#define LOSS_SPRITE "sprites/LossLabel.png"

// For sounds
#define GAME_BACKGROUND_MUSIC "audio/bgGame.mp3"
#define MENU_BACKGROUND_MUSIC "audio/bgMenu.wav"
#define CLICK_SOUND_EFFECT "audio/click.wav"
#define LASER_BOUNCE_SOUND_EFFECT "audio/laserBounce.wav"
#define LASER_HIT_SOUND_EFFECT "audio/laserHit.wav"
#define SCORE_TICK_SOUND_EFFECT "audio/scoreTick.wav"
#define SHOOT_NORMAL_SOUND_EFFECT "audio/shootNormal.wav"
// TODO use
#define SHOOT_POWERFUL_SOUND_EFFECT "audio/shootPowerful.wav"
#define TIME_OUT_SOUND_EFFECT "audio/timeOut.wav"
#define TIME_TICK_SOUND_EFFECT "audio/timeTick.wav"
#define WIN_SOUND_EFFECT "audio/win.wav"

// For scene transitions and durations
#define SCENE_TRANSITION_TIME 0.5
#define SCENE_TRANSITION_TYPE TransitionCrossFade
#define SCENE_TRANSITION(scene) SCENE_TRANSITION_TYPE::create(SCENE_TRANSITION_TIME, scene)
#define GAME_OVER_SCENE_TRANSITION_DELAY 0.8 // delay after game is over and before going to game over screen
#define SPLASH_SCREEN_DURATION 1.0

// Z levels
#define Z_LEVEL_BACKGROUND	0
#define Z_LEVEL_TARGET		1
#define Z_LEVEL_PROJECTILE	2
#define Z_LEVEL_GUNSHIP		3
#define Z_LEVEL_UI			4

// For gunship
#define SHOT_INTERVAL 0.8
#define PROJECTILE_LIFE_TIME 10
#define GUNSHIP_MASS 3.5
#define GUNSHIP_BOUNCINESS 1

// For asteroids
#define ASTEROID_MASS 5
#define ASTEROID_BOUNCINESS 1
#define ASTEROID_MAX_SPEED 0.08 // based on screen width
#define ASTEROID_MAX_ANGULAR_SPEED 120
#define ASTEROID_MIN_CURVE_TIME 1
#define ASTEROID_MAX_CURVE_TIME 10
#define ASTEROID_MIN_SCALE 0.8
#define ASTEROID_MAX_SCALE 1.6

// For physics
#define PHYSICS_UPDATE_INTERVAL (1.0 / 60)
#define DIR_HELPER 0.92
#define EDGE_WIDTH 10
#define COLLISION_BITMASK_ALL        0b11111111
#define COLLISION_BITMASK_NOTHING	 0b00000000
#define COLLISION_BITMASK_GUNSHIP    0b00000001
#define COLLISION_BITMASK_ASTEROID   0b00000010
#define COLLISION_BITMASK_PROJECTILE 0b00000100
#define COLLISION_BITMASK_EDGE	     0b00001000
// self, hit, overlap
#define EDGE_BITMASKS      COLLISION_BITMASK_EDGE, \
                           COLLISION_BITMASK_ALL, \
                           COLLISION_BITMASK_NOTHING
#define GUNSHIP_BITMASKS   COLLISION_BITMASK_GUNSHIP, \
	                       COLLISION_BITMASK_ASTEROID | COLLISION_BITMASK_EDGE | COLLISION_BITMASK_GUNSHIP, \
	                       COLLISION_BITMASK_NOTHING
#define ASTEROID_BITMASKS  COLLISION_BITMASK_ASTEROID, \
	                       COLLISION_BITMASK_ASTEROID | COLLISION_BITMASK_EDGE | COLLISION_BITMASK_GUNSHIP, \
						   COLLISION_BITMASK_PROJECTILE
#define LASERBALL_BITMASKS COLLISION_BITMASK_PROJECTILE, \
	                       COLLISION_BITMASK_EDGE | COLLISION_BITMASK_PROJECTILE, \
                           COLLISION_BITMASK_ASTEROID

#endif // __DEFINITIONS_H__
