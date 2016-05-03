#include "BilliardBall.h"

BilliardBall::BilliardBall(Vec2 _position, Color3B _color, Layer *_layer, b2World *_world)
{
	position = _position;
	color = _color;
	layer = _layer;
	world = _world;
}

void BilliardBall::CreateBody() {
	Sprite* pSprite = Sprite::create("ball.png");
	pSprite->setPosition(position.x, position.y);
	pSprite->setScale(2.2);
	pSprite->setColor(color);
	layer->addChild(pSprite, Z_ORDER_BALL);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x / PTM_RATIO, position.y / PTM_RATIO);
	bodyDef.userData = pSprite;

	ballBody = world->CreateBody(&bodyDef);
	ballBody->SetLinearDamping(0.5f);
	//원모양
	b2CircleShape circle;
	circle.m_radius = (pSprite->getScale()* 8.0f) / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 2.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.96f;

	ballBody->CreateFixture(&fixtureDef);
}

Vec2 BilliardBall::getPosition() {
	return Vec2(ballBody->GetPosition().x * PTM_RATIO, ballBody->GetPosition().y * PTM_RATIO);
}

b2Body* BilliardBall::getBody() {
	return ballBody;
}
