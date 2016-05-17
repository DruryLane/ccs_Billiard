#include "MenuScene.h"
#include "OptionScene.h"
#include "GameScene.h"

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
	option = 0;

	initMenu();
    
    return true;
}

void MenuScene::initMenu() {
	auto winSize = Director::getInstance()->getWinSize();

	auto pMenuItem1 = MenuItemFont::create(
		"싱글모드", CC_CALLBACK_1(MenuScene::doPopup, this));
	pMenuItem1->setColor(Color3B::BLACK);
	pMenuItem1->setTag(_MODE_SINGLE_);

	auto pMenuItem2 = MenuItemFont::create(
		"2인모드", CC_CALLBACK_1(MenuScene::doPopup, this));
	pMenuItem2->setColor(Color3B::BLACK);
	pMenuItem2->setTag(_MODE_DOUBLE_);

	auto pMenuItem3 = MenuItemFont::create(
		"EXIT", CC_CALLBACK_1(MenuScene::doExit, this));
	pMenuItem3->setColor(Color3B::BLACK);

	pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, nullptr);

	pMenu->alignItemsVertically();
	pMenu->alignItemsVerticallyWithPadding(30.0f);
	pMenu->setPosition(Vec2(winSize.width / 2.0f, winSize.height / 2.0f));

	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(GameScene::doMsgReceived), "Game_Scene", nullptr);
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(MenuScene::doMsgReceived), "Menu_Scene", nullptr);

	this->addChild(pMenu);
}

void MenuScene::doPopup(Ref * obj) {
	auto pMenuItem = (MenuItem*)obj;
	option += pMenuItem->getTag();

	Scene* popWin;
	pMenu->setEnabled(false);
	popWin = OptionScene::createScene();
	this->addChild(popWin, 2000, 2000);
}
void MenuScene::doMsgReceived(Ref* obj) {
	option += 10*(int)obj;
	log("MenuScene[%d]  메세지 도착", option);

	NotificationCenter::getInstance()->postNotification("Game_Scene", (Ref*)option);

	auto pScene = GameScene::createScene();
	Director::getInstance()->replaceScene(pScene);
}

void MenuScene::doExit(Ref* obj) {
	Director::getInstance()->end();
}
