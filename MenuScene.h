#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "GameManager.h"

class MenuScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(MenuScene);

	void initMenu();

	void doPopup(Ref* obj);
	void doMsgReceived(Ref* obj);

	int option;
	Menu* pMenu;
};

#endif // __MENU_SCENE_H__
