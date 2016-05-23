#include "MenuScene.h"
#include "IntroScene.h"

Scene* IntroScene::createScene()
{
    auto scene = Scene::create();
    auto layer = IntroScene::create();
    scene->addChild(layer);

    return scene;
}

bool IntroScene::init()
{

	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}
	winSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("intro.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	this->addChild(bg);

	auto pLabel = LabelTTF::create("화면을 터치하세요", "Arial", 35);
	pLabel->setAnchorPoint(Vec2(0.5, 0.5));
	pLabel->setPosition(Vec2(winSize.width / 2.0f, 100));
	pLabel->setColor(Color3B::BLACK);
	this->addChild(pLabel, Z_ORDER_BACKGROND);

	auto action1 = ScaleTo::create(1.0f, 0.7f);
	auto action2 = ScaleTo::create(0.5f, 1.0f);
	auto action3 = Sequence::create(action1, action2, nullptr);

	pLabel->runAction(RepeatForever::create(action3));

    return true;
}

void IntroScene::onEnter() {
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(IntroScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(IntroScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool IntroScene::onTouchBegan(Touch *touch, Event *event) {

	return true;
}

void IntroScene::onTouchEnded(Touch *touch, Event *event) {
	auto pScene = MenuScene::createScene();
	Director::getInstance()->replaceScene(pScene);
}
