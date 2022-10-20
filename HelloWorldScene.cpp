#include "HelloWorldScene.h"
#include "BishopPiece.h"
#include "PawnPiece.h"
#include "KingPiece.h"
#include "QueenPiece.h"
#include "RookPiece.h"
#include "KnightPiece.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

bool HelloWorld::init()
{
	if (!Scene::init())
	{
		return false;
	}

	turn = Color::WHITE;
	isFinished = false;
	isSelected = false;
	board = new Spot * [8];

	xPrev = -1;
	yPrev = -1;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	AudioEngine::preload(MOVE_SOUND);
	AudioEngine::preload(CAPTURE_SOUND);
	
	for (int i = 0; i < 8; i++) {
		board[i] = new Spot[8];
		for (int j = 0; j < 8; j++) {
			bool isWhite = (i + j) % 2 == 0;
			board[i][j] = Spot(nullptr, isWhite ? Color::WHITE : Color::BLACK);
			drawNodeBoard[i][j] = DrawNode::create();
			drawNodeBoard[i][j]->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width / 8, visibleSize.height / 8), isWhite ? Color4F::WHITE : Color4F::GRAY);
			drawNodeBoard[i][j]->setPosition(Vec2(visibleSize.width / 8 * i, visibleSize.height / 8 * j));
			this->addChild(drawNodeBoard[i][j]);
		}
	}

	for (int i = 0; i < 8; i++) {
		board[1][i].setPiece(new PawnPiece(Color::BLACK));
		board[6][i].setPiece(new PawnPiece(Color::WHITE));

		drawNodeBoard[i][1]->addChild(createPieceSprite(WP, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1));
		drawNodeBoard[i][6]->addChild(createPieceSprite(BP, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1));
	}

	board[0][2].setPiece(new BishopPiece(Color::BLACK));
	board[0][5].setPiece(new BishopPiece(Color::BLACK));
	board[7][2].setPiece(new BishopPiece(Color::WHITE));
	board[7][5].setPiece(new BishopPiece(Color::WHITE));

	drawNodeBoard[2][7]->addChild(createPieceSprite(BB, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.13));
	drawNodeBoard[5][7]->addChild(createPieceSprite(BB, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.13));
	drawNodeBoard[2][0]->addChild(createPieceSprite(WB, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.11));
	drawNodeBoard[5][0]->addChild(createPieceSprite(WB, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.11));

	board[0][4].setPiece(new KingPiece(Color::BLACK));
	board[7][4].setPiece(new KingPiece(Color::WHITE));

	drawNodeBoard[4][7]->addChild(createPieceSprite(BK, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.035));
	drawNodeBoard[4][0]->addChild(createPieceSprite(WK, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.11));

	board[0][3].setPiece(new QueenPiece(Color::BLACK));
	board[7][3].setPiece(new QueenPiece(Color::WHITE));

	drawNodeBoard[3][7]->addChild(createPieceSprite(BQ, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.035));
	drawNodeBoard[3][0]->addChild(createPieceSprite(WQ, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.09));

	board[0][0].setPiece(new RookPiece(Color::BLACK));
	drawNodeBoard[0][7]->addChild(createPieceSprite(BR, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1));

	board[0][7].setPiece(new RookPiece(Color::BLACK));
	drawNodeBoard[7][7]->addChild(createPieceSprite(BR, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1));

	board[7][0].setPiece(new RookPiece(Color::WHITE));
	drawNodeBoard[0][0]->addChild(createPieceSprite(WR, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1));

	board[7][7].setPiece(new RookPiece(Color::WHITE));
	drawNodeBoard[7][0]->addChild(createPieceSprite(WR, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1));

	board[0][1].setPiece(new KnightPiece(Color::BLACK));
	drawNodeBoard[1][7]->addChild(createPieceSprite(BH, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1));

	board[0][6].setPiece(new KnightPiece(Color::BLACK));
	drawNodeBoard[6][7]->addChild(createPieceSprite(BH, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1));

	board[7][1].setPiece(new KnightPiece(Color::WHITE));
	drawNodeBoard[1][0]->addChild(createPieceSprite(WH, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.04));

	board[7][6].setPiece(new KnightPiece(Color::WHITE));
	drawNodeBoard[6][0]->addChild(createPieceSprite(WH, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.04));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		auto touchLocation = touch->getLocation();
		int* index = getIndexOnClick(touchLocation);

		if (index != nullptr) {
			int x = index[0];
			int y = index[1];
			
			if (board[7 - y][x].getPiece() != nullptr && board[7 - y][x].getPiece()->getColor() == turn && !(x == xPrev && y == yPrev)) {
				drawNodeBoard[x][y]->clear();
				drawNodeBoard[x][y]->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width / 8, visibleSize.height / 8), Color4F::RED);
				if (isSelected) {
					drawNodeBoard[xPrev][yPrev]->clear();
					drawNodeBoard[xPrev][yPrev]->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width / 8, visibleSize.height / 8), board[7 - yPrev][xPrev].getColor() != Color::WHITE ? Color4F::WHITE : Color4F::GRAY);
				}
				isSelected = true;
				xPrev = x;
				yPrev = y;
			}
			else if (isSelected && (board[7 - y][x].getPiece() == nullptr || board[7 - y][x].getPiece()->getColor() != turn) && board[7 - yPrev][xPrev].getPiece()->validation(7 - yPrev, xPrev, 7 - y, x, board)) {
				if (dynamic_cast<KingPiece*>(board[7 - y][x].getPiece()) != nullptr) {
					isFinished = true;
				}
		
				if (board[7 - y][x].getPiece() == nullptr) {
					AudioEngine::play2d(MOVE_SOUND);	
				}
				else {
					delete board[7 - y][x].getPiece();
					AudioEngine::play2d(CAPTURE_SOUND);
				}
				
				board[7 - y][x].setPiece(board[7 - yPrev][xPrev].getPiece());
				board[7 - yPrev][xPrev].setPiece(nullptr);
				
				drawNodeBoard[xPrev][yPrev]->clear();
				drawNodeBoard[xPrev][yPrev]->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width / 8, visibleSize.height / 8), board[7 - yPrev][xPrev].getColor() != Color::WHITE ? Color4F::WHITE : Color4F::GRAY);
				
				if (board[7 - y][x].getPiece() != nullptr) {
					drawNodeBoard[x][y]->removeAllChildrenWithCleanup(true);
				}

				Sprite* spriteToCLone = dynamic_cast<Sprite*>(drawNodeBoard[xPrev][yPrev]->getChildren().at(0));
				Sprite* clonedSprite = Sprite::createWithTexture(spriteToCLone->getTexture());
				clonedSprite->setScale(spriteToCLone->getScaleX(), spriteToCLone->getScaleY());
				clonedSprite->setPosition(spriteToCLone->getPosition());
				
				drawNodeBoard[x][y]->addChild(clonedSprite);
				drawNodeBoard[xPrev][yPrev]->removeAllChildrenWithCleanup(true);

				if (isFinished) {
					auto label = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 24);
					label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
					this->addChild(label, 1);
					
					auto listener = EventListenerTouchOneByOne::create();
					listener->onTouchBegan = [=](Touch* touch, Event* event) {
						Director::getInstance()->end();
						return true;
					};

					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							drawNodeBoard[i][j]->removeAllChildrenWithCleanup(true);
						}
					}

					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							if (board[i][j].getPiece() != nullptr) {
								delete board[i][j].getPiece();
							}
						}
					}

					_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, label);
				}


				if (!isFinished && dynamic_cast<PawnPiece*>(board[7 - y][x].getPiece()) != nullptr && (y == 0 || y == 7)) {
					delete board[7 - y][x].getPiece();
					board[7 - y][x].setPiece(new QueenPiece(turn));
					drawNodeBoard[x][y]->removeAllChildrenWithCleanup(true);
					drawNodeBoard[x][y]->addChild(createPieceSprite(turn == Color::WHITE ? WQ : BQ, Vec2(visibleSize.width / 16, visibleSize.height / 16), turn == Color::WHITE ? 0.08 : 0.035));
				}

				turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;				

				isSelected = false;
			}
		}

		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

cocos2d::Sprite* HelloWorld::createPieceSprite(const char* filename, cocos2d::Vec2 position, float scale)
{
	auto sprite = Sprite::create(filename);
	sprite->setPosition(position);
	sprite->setScale(scale);
	return sprite;
}

int* HelloWorld::getIndexOnClick(cocos2d::Vec2 touchLocation)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int x = touchLocation.x / (visibleSize.width / 8);
	int y = touchLocation.y / (visibleSize.height / 8);
	return new int[2] { x, y };
}

