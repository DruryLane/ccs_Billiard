#ifndef __BILLIARD_BALL_H__
#define __BILLIARD_BALL_H__

#include "GameManager.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class BilliardBall {
public:
	BilliardBall(Vec2 _position, Color3B _color, Layer *_layer, b2World *_world);
	void CreateBody();
	Vec2 getPosition();
	b2Body* getBody();

private:
	Vec2 position;
	Color3B color;
	b2Body* ballBody;
	Layer* layer;
	b2World* world;
};

#endif // __BILLIARD_BALL_H__