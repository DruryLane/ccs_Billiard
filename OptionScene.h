#ifndef __OPTION_SCENE_H__
#define __OPTION_SCENE_H__

#include "GameManager.h"

class OptionScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(OptionScene);

	void initMenu();

	void doSelectOption(Object* pSender);
};

#endif // __OPTION_SCENE_H__
