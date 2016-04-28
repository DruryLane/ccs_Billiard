#include "MenuScene.h"
#include "GameScene.h"

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{

	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	initBackGround();
	if (createBox2dWorld(true)) {
		this->schedule(schedule_selector(GameScene::tick));
	}

    return true;
}

void GameScene::doMsgReceived(Ref* obj) {
	int option = (int)obj;
	log("GameScene[%d]  메세지 도착", option);
}

void GameScene::tick(float dt) {
	int velocityInterations = 8;
	int positionInterations = 3;

	_world->Step(dt, velocityInterations, positionInterations);

	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != nullptr) {
			Sprite* spriteData = (Sprite *)b->GetUserData();
			spriteData->setPosition(
				Vec2(b->GetPosition().x * PTM_RATIO,
					b->GetPosition().y * PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
}

bool GameScene::createBox2dWorld(bool debug){

	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);

	_world = new b2World(gravity);
	_world->SetAllowSleeping(true);
	_world->SetContinuousPhysics(true);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);

	b2Body *groundBody = _world->CreateBody(&groundBodyDef);

	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

	groundEdge.Set(b2Vec2(GROUND_X1 / PTM_RATIO, GROUND_Y1 / PTM_RATIO), b2Vec2(GROUND_X2 / PTM_RATIO, GROUND_Y1 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	groundEdge.Set(b2Vec2(GROUND_X1 / PTM_RATIO, GROUND_Y1 / PTM_RATIO), b2Vec2(GROUND_X1 / PTM_RATIO, GROUND_Y2 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	groundEdge.Set(b2Vec2(GROUND_X2 / PTM_RATIO, GROUND_Y1 / PTM_RATIO), b2Vec2(GROUND_X2 / PTM_RATIO, GROUND_Y2 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	groundEdge.Set(b2Vec2(GROUND_X1 / PTM_RATIO, GROUND_Y2 / PTM_RATIO), b2Vec2(GROUND_X2 / PTM_RATIO, GROUND_Y2 / PTM_RATIO));
	groundBody->CreateFixture(&boxShapeDef);

	myBall = addNewSprite(Vec2(150, 150), Color3B::RED);
	addNewSprite(Vec2(100, 100));
	return true;
}

GameScene::~GameScene() {
	delete _world;
	_world = nullptr;
}

void GameScene::initBackGround() {
	auto pGround = Sprite::create("ground.png");
	pGround->setAnchorPoint(Vec2(0, 0));
	pGround->setPosition(0, 0);
	this->addChild(pGround, 1);
}

void GameScene::onEnter() {
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void GameScene::onExit() {
	_eventDispatcher->removeAllEventListeners();

	Layer::onExit();
}

bool GameScene::onTouchBegan(Touch *touch, Event *event) {
	auto touchPoint = touch->getLocation();

	b2Body *tBall = this->getBodyAtTab(touchPoint);

	bBallTouch = false;

	if (tBall == myBall) {
		log("touch start..");
		bBallTouch = true;
		startPoint = b2Vec2(touchPoint.x / PTM_RATIO, touchPoint.y / PTM_RATIO);
	}


	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event) {
	Vec2 touchPoint = touch->getLocation();
}

void GameScene::onTouchEnded(Touch *touch, Event *event) {
	Vec2 touchPoint = touch->getLocation();

	if (myBall&& bBallTouch) {
		log("touch end");
		endPoint = b2Vec2(touchPoint.x / PTM_RATIO, touchPoint.y / PTM_RATIO);
		b2Vec2 force = endPoint - startPoint;

		b2Vec2 startVec = b2Vec2(startPoint.x, startPoint.y);

		force = b2Vec2(-5, -5);
		startVec = b2Vec2(myBall->GetPosition().x, myBall->GetPosition().y);
		myBall->ApplyLinearImpulse(force, startVec, true);

		bBallTouch = false;
	}

}

b2Body* GameScene::getBodyAtTab(Vec2 p) {
	b2Fixture *fix;
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != nullptr) {
			if (b->GetType() == b2_staticBody) continue;
			fix = b->GetFixtureList();
			if (fix->TestPoint(b2Vec2(p.x / PTM_RATIO, p.y / PTM_RATIO))) {
				return b;
			}
		}
	}
	return NULL;
}

b2Body* GameScene::addNewSprite(Vec2 point, Color3B color) {
	Sprite* pSprite = Sprite::create("ball.png");
	pSprite->setPosition(point.x, point.y);
	pSprite->setScale(1.5);
	pSprite->setColor(color);
	this->addChild(pSprite, 2);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodyDef.userData = pSprite;

	b2Body* body = _world->CreateBody(&bodyDef);

	//원모양
	b2CircleShape circle;
	circle.m_radius = 12.0f / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 2.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.96f;

	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* GameScene::addNewSprite(Vec2 point) {
	Sprite* pSprite = Sprite::create("ball.png");
	pSprite->setPosition(point.x, point.y);
	pSprite->setScale(1.5);
	this->addChild(pSprite, 2);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodyDef.userData = pSprite;

	b2Body* body = _world->CreateBody(&bodyDef);

	//원모양
	b2CircleShape circle;
	circle.m_radius = 12.0f / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 2.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.96f;

	body->CreateFixture(&fixtureDef);
	return body;
}
