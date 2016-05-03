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
	curTurn = PLAYER1;
	isPhysical = false;
	turn = true;

	if (createBox2dWorld()) {
		this->schedule(schedule_selector(GameScene::tick));
	}
	initBackGround();
	initCue();

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

	if (isPhysical) {
		isPhysical = false;
	}

	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext()) {
		if (b->GetUserData() != nullptr) {
			BilliardBall* billiardBall = (BilliardBall*)b->GetUserData();
			Sprite* spriteData = billiardBall->getSprite();
			spriteData->setPosition(
				Vec2(b->GetPosition().x * PTM_RATIO,
					b->GetPosition().y * PTM_RATIO));
			spriteData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));

			if (b->GetLinearVelocity().Length()) {
				isPhysical = true;
			}
		}
	}
	if (!isPhysical && !turn) {
		turnStart();
	}
}

bool GameScene::createBox2dWorld(){

	initBox2dWorld(b2Vec2(0.0f, 0.0f));
	initBall();

	return true;
}

void GameScene::initBox2dWorld(b2Vec2 g) {

	b2Vec2 gravity = g;

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

	myContactLstener = new ContactListener();
	_world->SetContactListener((b2ContactListener*)myContactLstener);

}

void GameScene::initBall() {
	playerBall[PLAYER1] = new BilliardBall(Color3B::RED, PLAYER1);
	playerBall[PLAYER1]->setBody(createBall(Vec2(230, 450), playerBall[PLAYER1]));
	playerBall[PLAYER1]->setTarget(true);

	playerBall[PLAYER2] = new BilliardBall(Color3B::YELLOW, PLAYER2);
	playerBall[PLAYER2]->setBody(createBall(Vec2(230, 400), playerBall[PLAYER2]));

	otherBall1 = new BilliardBall(Color3B::WHITE, OTHER1);
	otherBall1->setBody(createBall(Vec2(210, 550), otherBall1));

	otherBall2 = new BilliardBall(Color3B::WHITE, OTHER2);
	otherBall2->setBody(createBall(Vec2(250, 550), otherBall2));
}

b2Body* GameScene::createBall(Vec2 position, BilliardBall* pBilliardBall) {
	Sprite* pSprite = pBilliardBall->getSprite();
	pSprite->setPosition(position.x, position.y);
	this->addChild(pSprite, Z_ORDER_BALL);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x / PTM_RATIO, position.y / PTM_RATIO);
	bodyDef.userData = pBilliardBall;

	b2Body* ballBody = _world->CreateBody(&bodyDef);
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
	return ballBody;
}

GameScene::~GameScene() {
	delete _world;
	delete playerBall[PLAYER1];
	delete playerBall[PLAYER2];
	delete otherBall1;
	delete otherBall2;
	delete myContactLstener;

	_world = nullptr;
	playerBall[PLAYER1] = nullptr;
	playerBall[PLAYER2] = nullptr;
	otherBall1 = nullptr;
	otherBall2 = nullptr;
	myContactLstener = nullptr;
}

void GameScene::initBackGround() {
	auto pGround = Sprite::create("ground.png");
	pGround->setAnchorPoint(Vec2(0, 0));
	pGround->setPosition(10, 30);
	pGround->setScale(0.4);
	this->addChild(pGround, Z_ORDER_BACKGROND);

	pCueBox = Sprite::create("stickGround.png");
	pCueBox->setAnchorPoint(Vec2(1, 0.5));
	pCueBox->setPosition(winSize.width - 10, winSize.height / 2.0f);
	pCueBox->setScale(0.4);
	this->addChild(pCueBox, Z_ORDER_BACKGROND);
}

void GameScene::initCue() {
	pCue = Sprite::create("Cue.png");
	pCue->setAnchorPoint(Vec2(0.5, 1.2));
	pCue->setPosition(playerBall[curTurn]->getPosition().x, playerBall[curTurn]->getPosition().y);
	pCue->setScale(0.4);
	this->addChild(pCue, Z_ORDER_CUE);

	pCuePower = Sprite::create("Cue.png");
	pCuePower->setPosition(pCueBox->getContentSize().width / 2.0f, pCueBox->getContentSize().height / 2.0f);
	pCueBox->addChild(pCuePower, Z_ORDER_CUE);

	force = Vec2(0, 1);
}

void GameScene::turnStart() {
	turn = true;

	playerBall[curTurn]->setTarget(false);

	if (myContactLstener->isScore()) {
		log("들어옴");
		//점수 증가//
	}
	else {
		curTurn++;
		curTurn %= 2;
	}

	playerBall[curTurn]->setTarget(true);

	pCue->setPosition(playerBall[curTurn]->getPosition().x, playerBall[curTurn]->getPosition().y);
	pCue->setVisible(true);
}

void GameScene::turnEnd() {
	power = 0;
	pCuePower->setPosition(pCueBox->getContentSize().width / 2.0f, pCueBox->getContentSize().height / 2.0f);
	pCue->setVisible(false);
	force.normalize();
	turn = false;
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
	if (turn) {
		if (pCueBox->getBoundingBox().containsPoint(touchPoint)) {
			bSelect = false;
		}
		else {
			bSelect = true;
		}
	}

	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event) {
	Vec2 oldPoint = touch->getPreviousLocation();
	Vec2 newPoint = touch->getLocation();

	if (turn) {
		if (bSelect) {
			Vec2 firstVector = oldPoint - pCue->getPosition();
			float firstRotateAngle = -firstVector.getAngle();
			float previousTouch = CC_RADIANS_TO_DEGREES(firstRotateAngle);

			Vec2 secondVector = newPoint - pCue->getPosition();
			float secondRotateAngle = -secondVector.getAngle();
			float currentTouch = CC_RADIANS_TO_DEGREES(secondRotateAngle);

			force = force.rotateByAngle(Vec2::ZERO, firstRotateAngle - secondRotateAngle);
			gRotation += currentTouch - previousTouch;
			gRotation = fmod(gRotation, 360.0f);
			pCue->setRotation(gRotation);
		}
		else {
			power += (oldPoint.y - newPoint.y);
			if (power > pCueBox->getContentSize().height)
				power = pCueBox->getContentSize().height;
			if (power < 0)
				power = 0;
			pCuePower->setPositionY(pCueBox->getContentSize().height / 2.0f - power);
		}
	}
}

void GameScene::onTouchEnded(Touch *touch, Event *event) {
	if (turn) {
		if (bSelect) {
			bSelect = false;
			log("%f %f", force.x, force.y);
		}
		else {
			force *= (power * POWER);
			b2Vec2 bForce = b2Vec2(force.x / PTM_RATIO, force.y / PTM_RATIO);
			playerBall[curTurn]->getBody()->ApplyForceToCenter(bForce, true);
			turnEnd();
		}
	}
}