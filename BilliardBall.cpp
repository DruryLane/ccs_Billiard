#include "BilliardBall.h"

BilliardBall::BilliardBall(Color3B color, int num)
{
	target = false;
	ballNum = num;
	linearA = b2Vec2_zero;

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

	//각속도에 의한 속도변화
	linearA = angularToLinear(angularVelocity);
	pBody->SetLinearVelocity(pBody->GetLinearVelocity() + linearA);

}

void BilliardBall::updateBilliardBall(float dt) {
	log("%f %f %f %f", angularVelocity.x, angularVelocity.y, angularVelocity.z, angularVelocity.length());
	updateSprite();
	updateLinearVelocity(dt);	//선운동
	updateAngualrVelocity(dt);	//각운동
}

void BilliardBall::updateSprite() {
	pSprite->setPosition(Vec2(pBody->GetPosition().x * PTM_RATIO, pBody->GetPosition().y * PTM_RATIO));
	pSprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(pBody->GetAngle()));
}

void BilliardBall::updateLinearVelocity(float dt) {
	//미끄럼마찰력으로 인한 속도 변화
	b2Vec2 dv = pBody->GetLinearVelocity() - linearA;
	dv.Normalize();
	dv = -1 * FRICTION_SLIDE * dt * dv;
	//log("%f %f", dv.x, dv.y);
	if (dv.Length() < (pBody->GetLinearVelocity() - linearA).Length()) {
		pBody->SetLinearVelocity(pBody->GetLinearVelocity() + dv);
	}
	else {
		pBody->SetLinearVelocity(linearA);
	}
}

void BilliardBall::updateAngualrVelocity(float dt) {
	//구름 마찰력으로 인한 회전력 변화
	Vec3 dw = angularVelocity;
	dw.normalize();
	dw = -FRICTION_ROLLING * dt * dw;

	if (angularVelocity.length() > dw.length()) {
		angularVelocity += dw;
	}
	else {
		dw = angularVelocity;
		angularVelocity = Vec3::ZERO;
	}

	pBody->SetLinearVelocity(pBody->GetLinearVelocity() - linearA);
	linearA = angularToLinear(angularVelocity);
	pBody->SetLinearVelocity(pBody->GetLinearVelocity() + linearA);
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

b2Vec2 BilliardBall::angularToLinear(Vec3 m_angularVelocity) {
	b2Vec2 m_velocity;
	Vec3 r = Vec3(0, 0, -CIRCLE_RADIUS);

	r.cross(m_angularVelocity);
	m_velocity = b2Vec2(r.x, r.y);

	return m_velocity;
}