#ifndef __Box2d_ContactListener__
#define __Box2d_ContactListener__

#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "BilliardBall.h"
#include "GameManager.h"

USING_NS_CC;

class ContactListener : public b2ContactListener
{
public:
	ContactListener();
	~ContactListener();

	bool isScore();

	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

private:
	bool contactBall[4];
};

#endif // __Box2d_ContactListener__
