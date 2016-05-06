#include "BilliardBall.h"

BilliardBall::BilliardBall(Color3B color, int num)
{
	target = false;
	ballNum = num;

	pSprite = Sprite::create("ball.png");
	pSprite->setScale(CIRCLE_SCALE);
	pSprite->setColor(color);
}

void BilliardBall::initAngularVelocity(Vec2 targetPosition) {
	float y = -sqrt(1 - targetPosition.lengthSquared());
	Vec2 coordY = Vec2(pBody->GetLinearVelocity().x, pBody->GetLinearVelocity().y);
	coordY.normalize();
	Vec2 coordX = Vec2(coordY.y, -coordY.x);
	Vec3 m_targetPosition = Vec3(coordX.x * targetPosition.x + coordY.x * y, coordX.y * targetPosition.x + coordY.y * y, targetPosition.y);
	//log("%f %f %f", m_targetPosition.x, m_targetPosition.y, m_targetPosition.z);

	angularVelocity = m_targetPosition;
	angularVelocity = CIRCLE_RADIUS * angularVelocity;
	angularVelocity.cross(Vec3(pBody->GetLinearVelocity().x, pBody->GetLinearVelocity().y, 0));
	angularVelocity = (2 * angularVelocity) / (5 * CIRCLE_RADIUS * CIRCLE_RADIUS);
	log("angularVelocity init %f %f %f", angularVelocity.x, angularVelocity.y, angularVelocity.z);
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
	mass = pBody->GetMass();
}

void BilliardBall::setTarget(bool t) {
	target = t;
}

void BilliardBall::setBallNum(int num) {
	ballNum = num;
}

void BilliardBall::setAngularVelocity(Vec3 m_angularVelocity) {
	angularVelocity = m_angularVelocity;
}