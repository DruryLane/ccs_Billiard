#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#define PTM_RATIO	64
#define POWER		0.6f
#define CIRCLE_SCALE	2.2
#define CIRCLE_RADIUS	CIRCLE_SCALE * 8.0
#define BALL_DENSITY	1.7f;

#define MUSIC_HIT_VOLUME	0.2f

#define PLAYER1		0
#define PLAYER2		1
#define OTHER1		2
#define OTHER2		3

#define GROUND_X1	56.380878f
#define GROUND_X2	432.762817f
#define GROUND_Y1	78.711166f
#define GROUND_Y2	838.464478f

#define _MODE_SINGLE_	0
#define _MODE_DOUBLE_	1
#define _MODE_3_BALL_	2
#define _MODE_4_BALL_	3

#define Z_ORDER_BACKGROND	1
#define Z_ORDER_BALL		2
#define Z_ORDER_CUE			3
#define Z_ORDER_TARGET		4

#define TAG_LABEL_P1SCORE	0
#define TAG_LABEL_P2SCORE	1
#define TAG_SPRITE_TARGET_BALL	2

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define MUSIC_HIT	"hit.mp3"
#define MUSIC_TURN	"drop.mp3"
#else
#define MUSIC_HIT	"hit.wav"
#define MUSIC_TURN	"drop.wav"
#endif

USING_NS_CC;

#endif // __GAME_MANAGER_H__
