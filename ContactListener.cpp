#include "ContactListener.h"

ContactListener::ContactListener(){
	for (int i = 0; i < 4; i++) {
		contactBall[i] = false;
	}
}
ContactListener::~ContactListener(){

}

bool ContactListener::isScore() {
	bool b;
	if (!contactBall[PLAYER1] && !contactBall[PLAYER2] && contactBall[OTHER1] && contactBall[OTHER2]) {
		b = true;
	}
	else {
		b = false;
	}

	for (int i = 0; i < 4; i++) {
		contactBall[i] = false;
	}

	return b;
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
	}
}