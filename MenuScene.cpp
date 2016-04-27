#include "MenuScene.h"
#include "OptionScene.h"

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

	initMenu();
    
    return true;
}

void MenuScene::initMenu() {
	auto winSize = Director::getInstance()->getWinSize();

	auto pMenuItem1 = MenuItemFont::create(
		"싱글모드", CC_CALLBACK_1(MenuScene::doPopup, this));
	pMenuItem1->setColor(Color3B::BLACK);

	auto pMenuItem2 = MenuItemFont::create(
		"2인모드", CC_CALLBACK_1(MenuScene::doPopup, this));
	pMenuItem2->setColor(Color3B::BLACK);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, nullptr);

	pMenu->alignItemsVertically();
	pMenu->setPosition(Vec2(winSize.width / 2.0f, winSize.height / 2.0f));
	
	this->addChild(pMenu);

	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(MenuScene::doMsgReceived),
		"TouchStatus",
		nullptr);
}

void MenuScene::doPopup(Ref * obj) {
	Scene* popWin;
	popWin = OptionScene::createScene();
	this->addChild(popWin, 2000, 2000);
}
void MenuScene::doMsgReceived(Ref* obj) {
	int option = (int)obj;

	log("[%d]  메세지 도착", option);
}
