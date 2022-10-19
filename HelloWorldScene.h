#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#define WP "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\WP.png"
#define BP "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\BP.png"
#define WK "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\WK.png"
#define BK "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\BK.png"
#define WQ "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\WQ.png"
#define BQ "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\BQ.png"
#define WR "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\WR.png"
#define BR "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\BR.png"
#define WB "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\WB.png"
#define BB "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\BB.png"
#define WH "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\WH.png"
#define BH "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\BH.png"

#define MOVE_SOUND "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\MOVE.mp3"
#define CAPTURE_SOUND "C:\\Users\\zephsie\\Documents\\SimpleChessGame\\SimpleChessGame\\Resources\\CAPTURE.mp3"

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

    void choosePieceToReplacePawn(int x, int y);   

    int* getIndexOnClick(cocos2d::Vec2 touchLocation);	
	
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(HelloWorld);
};

#endif
