#include "BilliardBall.h"

BilliardBall::BilliardBall(Color3B color, int num)
{
	target = false;
	ballNum = num;

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

int BilliardBall::getBallNum() {
	return ballNum;
}

bool BilliardBall::isTarget() {
	return target;
}

void BilliardBall::setSprite(Sprite* sprite) {
	pSprite = sprite;
}

void BilliardBall::setBody(b2Body* body) {
	pBody = body;
}

void BilliardBall::setTarget(bool t) {
	target = t;
}

void BilliardBall::setBallNum(int num) {
	ballNum = num;
}
