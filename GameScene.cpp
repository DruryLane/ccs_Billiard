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
	if (!LayerColor::initWithColor(Color4B::GRAY))
	{
		return false;
	}

	winSize = Director::getInstance()->getWinSize();
	curTurn = PLAYER1;
	isPhysical = false;
	bTurn = true;
	bTarget = false;
	score[0] = 0;
	score[1] = 0;

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
			billiardBall->updateBilliardBall(dt);

			if (b->GetLinearVelocity().Length()) {
				isPhysical = true;
			}
		}
	}
	if (!isPhysical && !bTurn) {
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
	playerBall[PLAYER2]->setBody(createBall(Vec2(130, 200), playerBall[PLAYER2]));

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
	//ballBody->SetLinearDamping(0.5f);
	ballBody->SetAngularDamping(1.5f);
	//원모양
	b2CircleShape circle;
	circle.m_radius = CIRCLE_RADIUS / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = BALL_DENSITY;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 1.00f;

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

	auto pP1Label = LabelTTF::create("Player1 : ", "Arial", 30);
	pP1Label->setAnchorPoint(Vec2(0, 1));
	pP1Label->setPosition(Vec2(50, winSize.height - 30));
	pP1Label->setColor(Color3B::RED);
	this->addChild(pP1Label, Z_ORDER_BACKGROND);

	auto pP2Label = LabelTTF::create("Player2 : ", "Arial", 30);
	pP2Label->setAnchorPoint(Vec2(0, 1));
	pP2Label->setPosition(Vec2(winSize.width / 2 + 20, winSize.height - 30));
	pP2Label->setColor(Color3B::YELLOW);
	this->addChild(pP2Label, Z_ORDER_BACKGROND);

	auto pP1ScoreLabel = LabelTTF::create("0", "Arial", 30);
	pP1ScoreLabel->setAnchorPoint(Vec2(0, 1));
	pP1ScoreLabel->setPosition(Vec2(50 + pP1Label->getContentSize().width, winSize.height - 30));
	pP1ScoreLabel->setColor(Color3B::RED);
	pP1ScoreLabel->setTag(TAG_LABEL_P1SCORE);
	this->addChild(pP1ScoreLabel, Z_ORDER_BACKGROND);

	auto pP2ScoreLabel = LabelTTF::create("0", "Arial", 30);
	pP2ScoreLabel->setAnchorPoint(Vec2(0, 1));
	pP2ScoreLabel->setPosition(Vec2(winSize.width / 2 + 20 + pP2Label->getContentSize().width, winSize.height - 30));
	pP2ScoreLabel->setColor(Color3B::YELLOW);
	pP2ScoreLabel->setTag(TAG_LABEL_P2SCORE);
	this->addChild(pP2ScoreLabel, Z_ORDER_BACKGROND);

	pTarget = Sprite::create("ball_target.png");
	pTarget->setPosition((winSize.width / 2.0f) - 46, winSize.height - 300);
	pTarget->setAnchorPoint(Vec2(0.5, 0.5));
	pTarget->setOpacity(70);
	pTarget->setScale(0.5f);
	pTarget->setVisible(false);
	this->addChild(pTarget, Z_ORDER_TARGET);

	auto pTargetBall = Sprite::create("ball.png");
	pTargetBall->setColor(Color3B::RED);
	pTargetBall->setOpacity(90);
	pTargetBall->setScale(2.0);
	pTargetBall->setPosition(pTarget->getContentSize().width / 2.0, pTarget->getContentSize().height / 2.0);
	pTargetBall->setTag(TAG_SPRITE_TARGET_BALL);
	pTarget->addChild(pTargetBall);

	auto pBtnTarget = MenuItemImage::create("btn_target1.png", "btn_target2.png", CC_CALLBACK_1(GameScene::setTarget, this));
	auto pMenu = Menu::create(pBtnTarget, nullptr);
	pMenu->setPosition(Vec2(winSize.width - 50, 100));
	this->addChild(pMenu, Z_ORDER_BACKGROND);

}

void GameScene::initCue() {
	pCue = Sprite::create("cue.png");
	pCue->setAnchorPoint(Vec2(0.5, 1.2));
	pCue->setPosition(playerBall[curTurn]->getPosition().x, playerBall[curTurn]->getPosition().y);
	pCue->setScale(0.4);
	this->addChild(pCue, Z_ORDER_CUE);

	pCuePower = Sprite::create("cue.png");
	pCuePower->setPosition(pCueBox->getContentSize().width / 2.0f, pCueBox->getContentSize().height / 2.0f);
	pCueBox->addChild(pCuePower, Z_ORDER_CUE);

	pArrow = Sprite::create("arrow.png");
	pArrow->setAnchorPoint(Vec2(0.5, 0));
	pArrow->setPosition(playerBall[curTurn]->getPosition().x, playerBall[curTurn]->getPosition().y);
	pArrow->setScale(0.5);
	pArrow->setOpacity(70.0f);
	this->addChild(pArrow, Z_ORDER_CUE);

	force = Vec2(0, 1);
}

void GameScene::printScore(int player) {
	std::stringstream sScore;

	sScore << score[player];
	dynamic_cast<LabelTTF*>(this->getChildByTag(player))->setString(sScore.str());
}

void GameScene::setTarget(Ref* sender) {
	if (bTurn) {
		bTarget = true;
		pTarget->setVisible(true);
	}
}
void GameScene::moveTagetBall(Vec2 oldPoint, Vec2 newPoint) {
	
	Vec2 moveVec = newPoint - oldPoint;
	Vec2 centerVec = Vec2(pTarget->getContentSize().width / 2.0f, pTarget->getContentSize().height / 2.0f);
	Sprite* pTargetBall = (Sprite*)pTarget->getChildByTag(TAG_SPRITE_TARGET_BALL);
	Vec2 curVec = pTargetBall->getPosition();

	if (((curVec + moveVec) - centerVec).length() < (pTarget->getContentSize().width / 2.0f) - 15) {
		pTargetBall->setPosition(curVec + moveVec);
	}
}

void GameScene::turnStart() {
	bTurn = true;

	playerBall[curTurn]->setTarget(false);

	if (myContactLstener->isScore()) {
		//점수 증가
		score[curTurn] += 10;

		//점수 출력
		printScore(curTurn);
	}
	else {
		curTurn++;
		curTurn %= 2;
	}

	playerBall[curTurn]->setTarget(true);

	pCue->setPosition(playerBall[curTurn]->getPosition().x, playerBall[curTurn]->getPosition().y);
	pCue->setVisible(true);
	pArrow->setPosition(playerBall[curTurn]->getPosition().x, playerBall[curTurn]->getPosition().y);
	pArrow->setVisible(true);
}

void GameScene::turnEnd() {
	power = 0;
	pCuePower->setPosition(pCueBox->getContentSize().width / 2.0f, pCueBox->getContentSize().height / 2.0f);
	pCue->setVisible(false);
	pArrow->setVisible(false);
	force.normalize();
	bTurn = false;
	auto pTargetBall = (Sprite*)pTarget->getChildByTag(TAG_SPRITE_TARGET_BALL);
	pTargetBall->setPosition(pTarget->getContentSize().width / 2.0, pTarget->getContentSize().height / 2.0);
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
	if (bTurn) {
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

	if (bTurn) {
		if (bTarget) {
			moveTagetBall(oldPoint, newPoint);
		}
		else {
			if (bSelect) {
				Vec2 firstVector = oldPoint - pCue->getPosition();
				float firstRotateAngle = -firstVector.getAngle();
				float previousTouch = CC_RADIANS_TO_DEGREES(firstRotateAngle);

				Vec2 secondVector = newPoint - pCue->getPosition();
				float secondRotateAngle = -secondVector.getAngle();
				float currentTouch = CC_RADIANS_TO_DEGREES(secondRotateAngle);

				gRotation += currentTouch - previousTouch;
				gRotation = fmod(gRotation, 360.0f);

				force = force.rotateByAngle(Vec2::ZERO, firstRotateAngle - secondRotateAngle);
				pCue->setRotation(gRotation);
				pArrow->setRotation(gRotation);
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
}

void GameScene::onTouchEnded(Touch *touch, Event *event) {
	if (bTurn) {
		if (bTarget) {
			pTarget->setVisible(false);
			bTarget = false;
		}
		else {
			if (bSelect) {
				bSelect = false;
			}
			else {
				//파워가 0일때 턴종료 ㄴㄴ
				if (power) {

					//당구공에 힘전달
					b2Body* pBody = playerBall[curTurn]->getBody();
					force *= (power * POWER);
					b2Vec2 bForce = b2Vec2(force.x / PTM_RATIO, force.y / PTM_RATIO);
					pBody->ApplyLinearImpulse(bForce, pBody->GetPosition(), true);

					//targetPosition 전달 후 각속도값 계산
					auto pTargetBall = (Sprite*)pTarget->getChildByTag(TAG_SPRITE_TARGET_BALL);
					Vec2 targetPosition;
					targetPosition = pTargetBall->getPosition() - Vec2(pTarget->getContentSize().width / 2.0, pTarget->getContentSize().height / 2.0f);
					targetPosition = targetPosition / (pTarget->getContentSize().width / 2.0);
					playerBall[curTurn]->initAngularVelocity(targetPosition);

					//턴종료
					turnEnd();
				}
			}
		}
	}
}