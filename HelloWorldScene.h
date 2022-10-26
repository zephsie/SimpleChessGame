#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Spot.h"

class HelloWorld : public cocos2d::Scene {
private:	
    Spot** board;

    cocos2d::DrawNode* drawNodeBoard[8][8];

	int xPrev, yPrev;
	
    Color turn;

    bool isFinished;

    bool isSelected;

	cocos2d::Sprite* createPieceSprite(const char* filename, cocos2d::Vec2 position, float scale);

    void deconstruct();

    int* getIndexOnClick(cocos2d::Vec2 touchLocation);	

    void goToMainMenuScene(cocos2d::Ref* sender);
	
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(HelloWorld);
};

#endif
