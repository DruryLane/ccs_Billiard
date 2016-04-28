#include "MenuScene.h"
#include "OptionScene.h"

Scene* OptionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = OptionScene::create();
    scene->addChild(layer);

    return scene;
}

bool OptionScene::init()
{

	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 100)))
	{
		return false;
	}

	initMenu();

    return true;
}

void OptionScene::initMenu() {
	auto winSize = Director::getInstance()->getWinSize();

	auto popLayer = CCLayerColor::create(Color4B(0, 255, 0, 255), 300, 200);
	popLayer->setAnchorPoint(Vec2(0, 0));
	popLayer->setPosition(Vec2(
		(winSize.width - popLayer->getContentSize().width) / 2,
		(winSize.height - popLayer->getContentSize().height) / 2)
		);
	this->addChild(popLayer);

	MenuItemFont::setFontSize(20.0f);

	auto pMenuItem1 = MenuItemFont::create(
		"3 Ball", CC_CALLBACK_1(OptionScene::doSelectOption, this));
	pMenuItem1->setColor(Color3B(0, 0, 0));
	pMenuItem1->setTag(_MODE_3_BALL_);

	auto pMenuItem2 = MenuItemFont::create(
		"4 Ball", CC_CALLBACK_1(OptionScene::doSelectOption, this));
	pMenuItem2->setColor(Color3B(0, 0, 0));
	pMenuItem2->setTag(_MODE_4_BALL_);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, nullptr);
	pMenu->alignItemsVerticallyWithPadding(10.0f);
	pMenu->setPosition(Vec2(150, 100));
	popLayer->addChild(pMenu);
}

void OptionScene::doSelectOption(Ref* pSender) {

	auto pMenuItem = (MenuItem*)pSender;
	int option = pMenuItem->getTag();

	NotificationCenter::getInstance()->postNotification("Menu_Scene", (Ref*)option);
	this->removeFromParentAndCleanup(true);
}