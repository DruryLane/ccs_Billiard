#include "BilliardBall.h"

BilliardBall::BilliardBall(Color3B color)
{
	pSprite = Sprite::create("ball.png");
	pSprite->setScale(2.2);
	pSprite->setColor(color);
}

Sprite* BilliardBall::getSprite() {
	return pSprite;
}

b2Body* BilliardBall::getBody() {
	return pBody;
}

Vec2 BilliardBall::getPosition() {
	return pSprite->getPosition();
}

void BilliardBall::setSprite(Sprite* sprite) {
	pSprite = sprite;
}

void BilliardBall::setBody(b2Body* body) {
	pBody = body;
}
