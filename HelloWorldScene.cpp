#include "HelloWorldScene.h"
#include "BishopPiece.h"
#include "PawnPiece.h"
#include "KingPiece.h"
#include "QueenPiece.h"
#include "RookPiece.h"
#include "KnightPiece.h"
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "MainMenu.h"
#include "DEFINITIONS.h"

USING_NS_CC;

Scene *HelloWorld::createScene() {
    return HelloWorld::create();
}

bool HelloWorld::init() {
    if (!Scene::init()) {
        return false;
    }

    AudioEngine::play2d(MUSIC, true);

    turn = Color::WHITE;
    isFinished = false;
    isSelected = false;
    board = new Spot *[8];
    isOnline = true;

    player1 = new Player();
    player2 = new Player();

    xPrev = -1;
    yPrev = -1;

    Size visibleSize = Director::getInstance()->getVisibleSize();
	

    auto menuButton = ui::Button::create(TREE, TREE);
    menuButton->setPosition(cocos2d::Vec2(visibleSize.width / 50, visibleSize.height / 2));
    menuButton->setScaleX(visibleSize.width / menuButton->getContentSize().width / 15);
    menuButton->setScaleY(visibleSize.width / menuButton->getContentSize().width / 15);
    menuButton->setRotation(90);

    menuButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            AudioEngine::play2d(CAPTURE_SOUND);
            showDrawScene();
        }
    });

    this->addChild(menuButton, 125);


    auto blackFilter = Sprite::create(BLACKBG);
    blackFilter->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    blackFilter->setScaleX(visibleSize.width / blackFilter->getContentSize().width);
    blackFilter->setScaleY(visibleSize.height / blackFilter->getContentSize().height);
    blackFilter->setName("blackFilter");
    this->addChild(blackFilter, 300);


    ui::EditBox *player1Box = player1Box = ui::EditBox::create(Size(visibleSize.width / 1.25, visibleSize.height / 10), ui::Scale9Sprite::create(BLACKBG));
    player1Box->setPosition(Vec2(visibleSize.width / 2, 3 * visibleSize.height / 4));

    player1Box->setPlaceholderFontSize(16);
    player1Box->setPlaceHolder("Player One Username");
    player1Box->setPlaceholderFontColor(Color3B::RED);

    player1Box->setMaxLength(16);
    player1Box->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    player1Box->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    player1Box->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    player1Box->setTextHorizontalAlignment(TextHAlignment::CENTER);

    player1Box->setFontSize(16);
    player1Box->setFontColor(Color3B::RED);
    player1Box->setDelegate(this);
    player1Box->setName("player1Box");
    player1Box->setTag(1);
    this->addChild(player1Box, 500);


    ui::EditBox *player2Box = player2Box = ui::EditBox::create(Size(visibleSize.width / 1.25, visibleSize.height / 10), ui::Scale9Sprite::create(BLACKBG));
    player2Box->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    player2Box->setPlaceholderFontSize(16);
    player2Box->setPlaceHolder("Player Two Username");
    player2Box->setPlaceholderFontColor(Color3B::RED);

    player2Box->setMaxLength(16);
    player2Box->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    player2Box->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    player2Box->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    player2Box->setTextHorizontalAlignment(TextHAlignment::CENTER);

    player2Box->setFontSize(16);
    player2Box->setFontColor(Color3B::RED);
    player2Box->setDelegate(this);
    player2Box->setName("player2Box");
    player2Box->setTag(2);
    this->addChild(player2Box, 500);


    auto skipButton = ui::Button::create(SKIP, SKIPP);
    skipButton->setName("skipButton");
    skipButton->setPosition(cocos2d::Vec2(visibleSize.width / 4, visibleSize.height / 2 - visibleSize.height / 3));
    skipButton->setScaleX(visibleSize.width / skipButton->getContentSize().width / 4);
    skipButton->setScaleY(visibleSize.width / skipButton->getContentSize().width / 4);

    skipButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            AudioEngine::play2d(CAPTURE_SOUND);

            isOnline = false;

            this->removeChildByName("player1Box");
            this->removeChildByName("player2Box");
            this->removeChildByName("skipButton");
            this->removeChildByName("blackFilter");
            this->removeChildByName("sendButton");
        }
    });

    this->addChild(skipButton, 500);


    auto sendButton = ui::Button::create(SEND, SENDP);
    sendButton->setName("sendButton");
    sendButton->setPosition(cocos2d::Vec2(visibleSize.width / 4 * 3, visibleSize.height / 2 - visibleSize.height / 3));
    sendButton->setScaleX(visibleSize.width / sendButton->getContentSize().width / 4);
    sendButton->setScaleY(visibleSize.width / sendButton->getContentSize().width / 4);

    sendButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            if (player1->name.size() >= 2 && !player1->name.empty() && player2->name.size() >= 2 &&
                !player2->name.empty()) {
                AudioEngine::play2d(CAPTURE_SOUND);

                serverConnector = ServerConnector::instance();
					
                serverConnector->savePlayer(player1);
                serverConnector->savePlayer(player2);

                this->removeChildByName("player1Box");
                this->removeChildByName("player2Box");
                this->removeChildByName("skipButton");
                this->removeChildByName("blackFilter");
                this->removeChildByName("sendButton");
            }
        }
    });

    this->addChild(sendButton, 500);


    for (int i = 0; i < 8; i++) {
        board[i] = new Spot[8];

        for (int j = 0; j < 8; j++) {
            bool isWhite = (i + j) % 2 == 0;
            board[i][j] = Spot(nullptr, isWhite ? Color::WHITE : Color::BLACK);

            drawNodeBoard[i][j] = DrawNode::create();
            drawNodeBoard[i][j]->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width / 8, visibleSize.height / 8), isWhite ? Color4F::WHITE : Color4F::GRAY);
            drawNodeBoard[i][j]->setPosition(Vec2(visibleSize.width / 8 * i, visibleSize.height / 8 * j));

            this->addChild(drawNodeBoard[i][j], 100);
        }
    }


    for (int i = 0; i < 8; i++) {
        board[1][i].setPiece(new PawnPiece(Color::BLACK));
        board[6][i].setPiece(new PawnPiece(Color::WHITE));

        drawNodeBoard[i][1]->addChild(createPieceSprite(WP, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1), 100);
        drawNodeBoard[i][6]->addChild(createPieceSprite(BP, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1), 100);
    }


    board[0][2].setPiece(new BishopPiece(Color::BLACK));
    board[0][5].setPiece(new BishopPiece(Color::BLACK));
    board[7][2].setPiece(new BishopPiece(Color::WHITE));
    board[7][5].setPiece(new BishopPiece(Color::WHITE));

    drawNodeBoard[2][7]->addChild(createPieceSprite(BB, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.13), 100);
    drawNodeBoard[5][7]->addChild(createPieceSprite(BB, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.13), 100);
    drawNodeBoard[2][0]->addChild(createPieceSprite(WB, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.11), 100);
    drawNodeBoard[5][0]->addChild(createPieceSprite(WB, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.11), 100);


    board[0][4].setPiece(new KingPiece(Color::BLACK));
    board[7][4].setPiece(new KingPiece(Color::WHITE));

    drawNodeBoard[4][7]->addChild(createPieceSprite(BK, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.035), 100);
    drawNodeBoard[4][0]->addChild(createPieceSprite(WK, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.11), 100);


    board[0][3].setPiece(new QueenPiece(Color::BLACK));
    board[7][3].setPiece(new QueenPiece(Color::WHITE));

    drawNodeBoard[3][7]->addChild(createPieceSprite(BQ, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.035), 100);
    drawNodeBoard[3][0]->addChild(createPieceSprite(WQ, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.09), 100);


    board[0][0].setPiece(new RookPiece(Color::BLACK));
    drawNodeBoard[0][7]->addChild(createPieceSprite(BR, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1), 100);

    board[0][7].setPiece(new RookPiece(Color::BLACK));
    drawNodeBoard[7][7]->addChild(createPieceSprite(BR, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1), 100);


    board[7][0].setPiece(new RookPiece(Color::WHITE));
    drawNodeBoard[0][0]->addChild(createPieceSprite(WR, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1), 100);

    board[7][7].setPiece(new RookPiece(Color::WHITE));
    drawNodeBoard[7][0]->addChild(createPieceSprite(WR, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1), 100);


    board[0][1].setPiece(new KnightPiece(Color::BLACK));
    drawNodeBoard[1][7]->addChild(createPieceSprite(BH, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1), 100);

    board[0][6].setPiece(new KnightPiece(Color::BLACK));
    drawNodeBoard[6][7]->addChild(createPieceSprite(BH, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.1), 100);


    board[7][1].setPiece(new KnightPiece(Color::WHITE));
    drawNodeBoard[1][0]->addChild(createPieceSprite(WH, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.04), 100);

    board[7][6].setPiece(new KnightPiece(Color::WHITE));
    drawNodeBoard[6][0]->addChild(createPieceSprite(WH, Vec2(visibleSize.width / 16, visibleSize.height / 16), 0.04), 100);


    auto listener = EventListenerTouchOneByOne::create();

    listener->onTouchBegan = [=](Touch *touch, Event *event) {
        Vec2 *point = getIndexOnClick(touch->getLocation());

        int x = point->x;
        int y = point->y;

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

        } else if (isSelected && (board[7 - y][x].getPiece() == nullptr || board[7 - y][x].getPiece()->getColor() != turn) && board[7 - yPrev][xPrev].getPiece()->validation(7 - yPrev, xPrev, 7 - y, x, board)) {
            if (dynamic_cast<KingPiece *>(board[7 - y][x].getPiece()) != nullptr) {
                isFinished = true;
            }

            if (board[7 - y][x].getPiece() == nullptr) {
                AudioEngine::play2d(MOVE_SOUND);
            } else {
                delete board[7 - y][x].getPiece();
                AudioEngine::play2d(CAPTURE_SOUND);
            }

            board[7 - y][x].setPiece(board[7 - yPrev][xPrev].getPiece());
            board[7 - yPrev][xPrev].setPiece(nullptr);

            drawNodeBoard[xPrev][yPrev]->clear();
            drawNodeBoard[xPrev][yPrev]->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width / 8, visibleSize.height / 8), board[7 - yPrev][xPrev].getColor() != Color::WHITE ? Color4F::WHITE : Color4F::GRAY);

            drawNodeBoard[x][y]->removeAllChildrenWithCleanup(true);

            Sprite *spriteToCLone = dynamic_cast<Sprite *>(drawNodeBoard[xPrev][yPrev]->getChildren().at(0));
            Sprite *clonedSprite = Sprite::createWithTexture(spriteToCLone->getTexture());
            clonedSprite->setScale(spriteToCLone->getScaleX(), spriteToCLone->getScaleY());
            clonedSprite->setPosition(spriteToCLone->getPosition());

            drawNodeBoard[x][y]->addChild(clonedSprite, 100);
            drawNodeBoard[xPrev][yPrev]->removeAllChildrenWithCleanup(true);

            if (isFinished) {
                showWinScene();
            }

            if (!isFinished && dynamic_cast<PawnPiece *>(board[7 - y][x].getPiece()) != nullptr && (y == 0 || y == 7)) {
                delete board[7 - y][x].getPiece();
                board[7 - y][x].setPiece(new QueenPiece(turn));

                drawNodeBoard[x][y]->removeAllChildrenWithCleanup(true);
                drawNodeBoard[x][y]->addChild(createPieceSprite(turn == Color::WHITE ? WQ : BQ, Vec2(visibleSize.width / 16, visibleSize.height / 16),turn == Color::WHITE ? 0.08 : 0.035), 100);
            }

            turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;

            isSelected = false;
        }

        delete point;

        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void HelloWorld::goToMainMenuScene(cocos2d::Ref *sender) {
    auto scene = MainMenu::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void HelloWorld::showWinScene() {
    Size visibleSize = Director::getInstance()->getVisibleSize();


    auto filter = LayerColor::create(Color4B(0, 0, 0, 0));
    this->addChild(filter, 150);
    auto fade = FadeTo::create(2.0f, 255);
    filter->runAction(fade);


    auto title = Sprite::create(GAMEOVER);
    title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.2));
    title->setScaleX(visibleSize.width / title->getContentSize().width / 1.1);
    title->setScaleY(visibleSize.width / title->getContentSize().width / 1.1);
    this->addChild(title, 200);

    std::string text;

    if (!isOnline) {
        Sprite *winner = turn == Color::WHITE ? Sprite::create(WPLAYER) : Sprite::create(BPLAYER);

        winner->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 7));
        winner->setScaleX(visibleSize.width / winner->getContentSize().width / 1.5);
        winner->setScaleY(visibleSize.width / winner->getContentSize().width / 1.5);
        this->addChild(winner, 200);
    } else {
        if (turn == Color::WHITE) {
            serverConnector->addWin(player1->id);
			text = "Player named " + player1->name + "\nwith UID " + std::to_string(player1->id) + " and " + std::to_string(player1->score) + " wins won!";
        } else {
            serverConnector->addWin(player2->id);
			text = "Player named " + player2->name + "\nwith UID " + std::to_string(player2->id) + " and " + std::to_string(player2->score) + " wins won!";
        }

        auto label = Label::createWithTTF(text, "fonts/arial.ttf", 12);
        label->setTextColor(Color4B::RED);
        label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);

        label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 7));
        this->addChild(label, 200);
    }


    auto jumpscare = Sprite::create(JUMPSCARE);
    jumpscare->setScaleX(visibleSize.width / jumpscare->getContentSize().width / 3);
    jumpscare->setScaleY(visibleSize.width / jumpscare->getContentSize().width / 3);
    jumpscare->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(jumpscare, 200);

    auto scale = ScaleTo::create(2.0f, 1.1f);
    jumpscare->runAction(scale);


    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch *touch, Event *event) {
        AudioEngine::stopAll();
        deconstruct();
        goToMainMenuScene(this);

        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::showDrawScene() {
    Size visibleSize = Director::getInstance()->getVisibleSize();


    auto filter = LayerColor::create(Color4B(0, 0, 0, 0));
    this->addChild(filter, 150);
    auto fade = FadeTo::create(2.0f, 255);
    filter->runAction(fade);


    auto title = Sprite::create(GAMEOVER);
    title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.2));
    title->setScaleX(visibleSize.width / title->getContentSize().width / 1.1);
    title->setScaleY(visibleSize.width / title->getContentSize().width / 1.1);
    this->addChild(title, 200);


    auto draw = Sprite::create(DRAW);
    draw->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 7));
    draw->setScaleX(visibleSize.width / draw->getContentSize().width / 2.5);
    draw->setScaleY(visibleSize.width / draw->getContentSize().width / 2.5);
    this->addChild(draw, 200);


    auto jumpscare = Sprite::create(JUMPSCARE2);
    jumpscare->setScaleX(visibleSize.width / jumpscare->getContentSize().width / 3);
    jumpscare->setScaleY(visibleSize.width / jumpscare->getContentSize().width / 3);
    jumpscare->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(jumpscare, 200);


    auto scale = ScaleTo::create(2.0f, 0.75f);
    jumpscare->runAction(scale);


    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch *touch, Event *event) {
        AudioEngine::stopAll();
        deconstruct();
        goToMainMenuScene(this);

        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::editBoxReturn(cocos2d::ui::EditBox *editBox) {
    if (editBox->getTag() == 1) {
        player1->name = editBox->getText();
    }

    if (editBox->getTag() == 2) {
        player2->name = editBox->getText();
    }
}

cocos2d::Sprite *HelloWorld::createPieceSprite(const char *filename, cocos2d::Vec2 position, float scale) {
    auto sprite = Sprite::create(filename);
    sprite->setPosition(position);
    sprite->setScale(scale);
    return sprite;
}

void HelloWorld::deconstruct() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete board[i][j].getPiece();
        }
    }

    delete player1;
    delete player2;
}

Vec2 *HelloWorld::getIndexOnClick(cocos2d::Vec2 touchLocation) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    int x = touchLocation.x / (visibleSize.width / 8);
    int y = touchLocation.y / (visibleSize.height / 8);
    return new Vec2(x, y);
}