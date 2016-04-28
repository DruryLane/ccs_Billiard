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

	auto pSprite1 = Sprite::create("ground.png");
	pSprite1->setAnchorPoint(Vec2(0, 0));
	pSprite1->setPosition(0, 0);
	this->addChild(pSprite1);

	auto pSprite3 = Sprite::create("ball.png");
	pSprite3->setAnchorPoint(Vec2(0, 0));
	pSprite3->setPosition(150, 100);
	pSprite3->setColor(Color3B::RED);
	pSprite3->setScale(1.5);
	pSprite1->addChild(pSprite3);
    
    return true;
}

void GameScene::doMsgReceived(Ref* obj) {
	int option = (int)obj;
	log("GameScene[%d]  메세지 도착", option);
}
