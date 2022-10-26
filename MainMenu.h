#pragma once

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	
	CREATE_FUNC(MainMenu);
	
private:
	void goToGameScene(cocos2d::Ref* sender);
};