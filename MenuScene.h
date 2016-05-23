#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "GameManager.h"
#include "cocos-ext.h"

class MenuScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(MenuScene);

	void initMenu();

	void doPopup(Ref* obj);
	void doSelect(Ref* obj);
	void doExit(Ref* obj);

	Menu* pMenu;
	Size winSize;
};

#endif // __MENU_SCENE_H__
