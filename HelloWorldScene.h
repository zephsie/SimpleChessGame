#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Spot.h"
#include "network/HttpClient.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include <string>
#include "Player.h"
#include "ServerConnector.h"

class HelloWorld : public cocos2d::Scene, public cocos2d::ui::EditBoxDelegate {
private:

    Spot **board;

    cocos2d::DrawNode *drawNodeBoard[8][8];

    int xPrev, yPrev;

    Player *player1;
    Player *player2;

    Color turn;

    bool isFinished, isSelected, isOnline;

	ServerConnector* serverConnector;

    cocos2d::Sprite *createPieceSprite(const char *filename, cocos2d::Vec2 position, float scale);

    void deconstruct();

    cocos2d::Vec2 *getIndexOnClick(cocos2d::Vec2 touchLocation);

    virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);

    void showWinScene();

    void showDrawScene();

    void goToMainMenuScene(cocos2d::Ref *sender);

public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    CREATE_FUNC(HelloWorld);
};

#endif