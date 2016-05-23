#include "MenuScene.h"
#include "GameScene.h"

using namespace cocos2d::extension;

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    scene->addChild(layer);

    return scene;
}

bool MenuScene::init()
{

	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}

	winSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("backGround.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	this->addChild(bg);

	initMenu();
    
    return true;
}

void MenuScene::initMenu() {
	MenuItemFont::setFontSize(60);

	auto winSize = Director::getInstance()->getWinSize();

	auto pMenuItem1 = MenuItemFont::create(
		"싱글모드", CC_CALLBACK_1(MenuScene::doPopup, this));
	pMenuItem1->setColor(Color3B::WHITE);
	pMenuItem1->setTag(_MODE_SINGLE_);

	auto pMenuItem2 = MenuItemFont::create(
		"2인모드", CC_CALLBACK_1(MenuScene::doPopup, this));
	pMenuItem2->setColor(Color3B::WHITE);
	pMenuItem2->setTag(_MODE_DOUBLE_);

	auto pMenuItem3 = MenuItemFont::create(
		"EXIT", CC_CALLBACK_1(MenuScene::doExit, this));
	pMenuItem3->setColor(Color3B::WHITE);

	pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, nullptr);

	pMenu->alignItemsVerticallyWithPadding(50.0f);
	pMenu->setPosition(Vec2(winSize.width / 2.0f, winSize.height / 2.0f - 300));

	this->addChild(pMenu);
}

void MenuScene::doPopup(Ref * obj) {
	auto pMenuItem = (MenuItem*)obj;
	int option = pMenuItem->getTag();

	UserDefault::getInstance()->setIntegerForKey("mode", option);
	UserDefault::getInstance()->flush();

	auto backLayer = LayerColor::create(Color4B(0, 0, 0, 100), winSize.width, winSize.height);
	backLayer->setAnchorPoint(Vec2(0, 0));
	backLayer->setPosition(Vec2(0, 0));
	backLayer->setTag(TAG_LAYER_SELECTMODE);
	this->addChild(backLayer, Z_ORDER_NEW_LAYER);

	auto popLayer = Scale9Sprite::create("green_edit.png", Rect(0, 0, 28, 28), Rect(9, 9, 9, 9));
	popLayer->setContentSize(Size(300, 200));
	popLayer->setAnchorPoint(Vec2(0, 0));
	popLayer->setPosition(Vec2((winSize.width - popLayer->getContentSize().width) / 2,
		(winSize.height - popLayer->getContentSize().height) / 2));
	backLayer->addChild(popLayer);

	auto pMenuItem1 = MenuItemFont::create(
		"3 Ball", CC_CALLBACK_1(MenuScene::doSelect, this));
	pMenuItem1->setColor(Color3B(0, 0, 0));
	pMenuItem1->setTag(_MODE_3_BALL_);

	auto pMenuItem2 = MenuItemFont::create(
		"4 Ball", CC_CALLBACK_1(MenuScene::doSelect, this));
	pMenuItem2->setColor(Color3B(0, 0, 0));
	pMenuItem2->setTag(_MODE_4_BALL_);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, nullptr);
	pMenu->alignItemsVerticallyWithPadding(10.0f);
	pMenu->setPosition(Vec2(150, 100));
	popLayer->addChild(pMenu);

}

void MenuScene::doSelect(Ref* obj) {
	auto pMenuItem = (MenuItem*)obj;
	int option = (pMenuItem->getTag()) * 10;
	option += UserDefault::getInstance()->getIntegerForKey("mode");

	UserDefault::getInstance()->setIntegerForKey("mode", option);
	UserDefault::getInstance()->flush();

	this->removeChildByTag(TAG_LAYER_SELECTMODE);

	auto pScene = GameScene::createScene();
	Director::getInstance()->replaceScene(pScene);
}

void MenuScene::doExit(Ref* obj) {
	Director::getInstance()->end();
}