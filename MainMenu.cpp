#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "HelloWorldScene.h"
#include "DEFINITIONS.h"
#include "AudioEngine.h"

USING_NS_CC;

cocos2d::Scene* MainMenu::createScene() {
    auto scene = cocos2d::Scene::create();
    auto layer = MainMenu::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenu::init() {
    if (!Layer::init()) {
        return false;
    }

    AudioEngine::play2d(MUSIC, true);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    auto background = cocos2d::Sprite::create(BG);
    background->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.width / background->getContentSize().width);
    this->addChild(background);

    auto title = cocos2d::Sprite::create(LOGO);
    title->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 1.2));
    title->setScaleX(visibleSize.width / title->getContentSize().width / 1.1);
    title->setScaleY(visibleSize.width / title->getContentSize().width / 1.1);
    this->addChild(title);

    auto button = ui::Button::create(START, STARTP);
    button->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 6));
    button->setScaleX(visibleSize.width / button->getContentSize().width / 2);
    button->setScaleY(visibleSize.width / button->getContentSize().width / 2);

    button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            AudioEngine::stopAll();
            AudioEngine::play2d(CAPTURE_SOUND);
            goToGameScene(sender);
        }
        });

    this->addChild(button);

    return true;
}

void MainMenu::goToGameScene(cocos2d::Ref* sender) {
    auto scene = HelloWorld::createScene();

    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1, scene));
}