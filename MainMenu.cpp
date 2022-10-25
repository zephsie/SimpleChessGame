#include "MainMenu.h"

cocos2d::Scene* MainMenu::createScene()
{
    auto scene = cocos2d::Scene::create();
    auto layer = MainMenu::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // create lable with title
    auto label = cocos2d::Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 24);
    label->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    
   
    return true;
}