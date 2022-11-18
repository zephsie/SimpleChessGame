#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Spot.h"
#include "network/HttpClient.h"
#include "ui/CocosGUI.h"
#include <string>
#include "Player.h"

class HelloWorld : public cocos2d::Scene, public cocos2d::ui::EditBoxDelegate {
private:

    Spot **board;

    std::string serverUrl;

    cocos2d::DrawNode *drawNodeBoard[8][8];

    int xPrev, yPrev;

    Player *player1;
    Player *player2;

    Color turn;

    bool isFinished, isSelected, isOnline;

    cocos2d::Label* hidden;

    cocos2d::Size visibleSize;

	cocos2d::ui::Button* menuButton;
    cocos2d::ui::Button* skipButton;
    cocos2d::ui::Button* sendButton;

    cocos2d::ui::EditBox* player1Box = player1Box;
    cocos2d::ui::EditBox* player2Box = player2Box;

    cocos2d::EventListenerTouchOneByOne* gameListener;

    cocos2d::Sprite* blackFilter;

    cocos2d::Sprite* createPieceSprite(const char *filename, cocos2d::Vec2 position, float scale);

    cocos2d::Vec2* getIndexOnClick(cocos2d::Vec2 touchLocation);

    virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);

    void showWinScene();

    void showDrawScene();

    void goToMainMenuScene(cocos2d::Ref *sender);

	void skipEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void sendEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void drawEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	
	void stopEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type);

    void onSavePlayerCompleted(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response, Player* player);

	void onAddWinCompleted(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response, Player* player);

    void savePlayer(Player* player);

    void addWin(Player* player);

    void setPlayerFields(std::string json, Player* player);

public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    CREATE_FUNC(HelloWorld);
};

#endif