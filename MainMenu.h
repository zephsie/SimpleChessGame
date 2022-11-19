#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include <string>

class MainMenu : public cocos2d::Layer {
public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    CREATE_FUNC(MainMenu);

private:

    std::string serverUrl;

    cocos2d::ui::Button* button;

    cocos2d::ui::Button* leave;

    cocos2d::ui::Button* chalice;
	
    cocos2d::Label* topPlayers;
	
    void goToGameScene(cocos2d::Ref *sender);

    void showTopEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onShowTopCompleted(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response);

    void parseData(std::string json);
};

#endif