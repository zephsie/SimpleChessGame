#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC::ui;

class MainMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	CREATE_FUNC(MainMenu);
	
private:

	void touchEvent(Ref* pSender, Widget::TouchEventType type);
};