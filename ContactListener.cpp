#include "ContactListener.h"

ContactListener::ContactListener(){
	for (int i = 0; i < 4; i++) {
		contactBall[i] = false;
	}
	option = UserDefault::getInstance()->getIntegerForKey("mode");
	threeBallCushion = 0;
}
ContactListener::~ContactListener(){

}

bool ContactListener::isScore() {
	bool b;
	if (option / 10 == _MODE_4_BALL_) {
		if (!contactBall[PLAYER1] && !contactBall[PLAYER2] && contactBall[OTHER1] && contactBall[OTHER2]) {
			b = true;
		}
		else {
			b = false;
		}
	}
	else {
		if (((contactBall[PLAYER1] && contactBall[OTHER1]) || (contactBall[PLAYER2] && contactBall[OTHER1])) && (threeBallCushion >= 3)) {
			b = true;
		}
		else {
			b = false;
		}
	}

	for (int i = 0; i < 4; i++) {
		contactBall[i] = false;
	}

	return b;
}

void ContactListener::setThreeBallCushion(int n) {
	threeBallCushion = n;
}

void ContactListener::BeginContact(b2Contact* contact){
	
}

void ContactListener::EndContact(b2Contact* contact){
	
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){

}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
	b2Fixture* fixA = contact->GetFixtureA();
	b2Fixture* fixB = contact->GetFixtureB();

	b2Body* bodyA = fixA->GetBody();
	b2Body* bodyB = fixB->GetBody();
	
	if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_dynamicBody){
		BilliardBall* ballA = (BilliardBall*)bodyA->GetUserData();
		BilliardBall* ballB = (BilliardBall*)bodyB->GetUserData();
		if (ballA->isTarget()) {
			contactBall[ballB->getBallNum()] = true;
		}
		else if (ballB->isTarget()) {
			contactBall[ballA->getBallNum()] = true;
		}
		
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(impulse->normalImpulses[0] * MUSIC_HIT_VOLUME);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(MUSIC_HIT);
		log("%f", impulse->normalImpulses[0]);
	}
	else {
		if ((contactBall[PLAYER1]&&contactBall[OTHER1]) || (contactBall[PLAYER2] && contactBall[OTHER1])) {
		}
		else {
			threeBallCushion++;
		}
	}
}