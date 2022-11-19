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
#include "extensions/cocos-ext.h"

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

    serverUrl = SERVER;

    player1 = new Player();
    player2 = new Player();

    xPrev = -1;
    yPrev = -1;

    visibleSize = Director::getInstance()->getVisibleSize();

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

    hidden = Label::createWithTTF("", FONT, 12);
    hidden->setPosition(Vec2(visibleSize.width / 2, 6 * visibleSize.height / 7));
    hidden->setVisible(false);
    hidden->setTextColor(Color4B::RED);
    hidden->setAlignment(TextHAlignment::CENTER);
    this->addChild(hidden, 1000);

    menuButton = ui::Button::create(MENU, MENU);
    menuButton->setPosition(cocos2d::Vec2(visibleSize.width / 40, visibleSize.height / 2));
    menuButton->setScaleX(visibleSize.width / menuButton->getContentSize().width / 25);
    menuButton->setScaleY(visibleSize.width / menuButton->getContentSize().width / 25);
    menuButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::drawEvent, this));
    this->addChild(menuButton, 125);
	menuButton->setEnabled(false);

    blackFilter = Sprite::create(BLACKBG);
    blackFilter->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    blackFilter->setScaleX(visibleSize.width / blackFilter->getContentSize().width);
    blackFilter->setScaleY(visibleSize.height / blackFilter->getContentSize().height);
    this->addChild(blackFilter, 300);

    player1Box = ui::EditBox::create(Size(visibleSize.width / 1.25, visibleSize.height / 10), ui::Scale9Sprite::create(BLACKBG));
    player1Box->setPosition(Vec2(visibleSize.width / 2, 3 * visibleSize.height / 4));

    player1Box->setPlaceholderFont(FONT, 15);
    player1Box->setPlaceHolder("Player One Username");
    player1Box->setPlaceholderFontColor(Color3B::RED);

    player1Box->setFont(FONT, 16);
    player1Box->setMaxLength(16);
    player1Box->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    player1Box->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    player1Box->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    player1Box->setTextHorizontalAlignment(TextHAlignment::CENTER);

    player1Box->setFontColor(Color3B::RED);
    player1Box->setDelegate(this);
    player1Box->setTag(1);
    this->addChild(player1Box, 500);

    player2Box = ui::EditBox::create(Size(visibleSize.width / 1.25, visibleSize.height / 10), ui::Scale9Sprite::create(BLACKBG));
    player2Box->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    player2Box->setPlaceholderFont(FONT, 15);
    player2Box->setPlaceHolder("Player Two Username");
    player2Box->setPlaceholderFontColor(Color3B::RED);

    player2Box->setFont(FONT, 16);
    player2Box->setMaxLength(16);
    player2Box->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    player2Box->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    player2Box->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
    player2Box->setTextHorizontalAlignment(TextHAlignment::CENTER);

    player2Box->setFontColor(Color3B::RED);
    player2Box->setDelegate(this);
    player2Box->setTag(2);
    this->addChild(player2Box, 500);

    skipButton = ui::Button::create(SKIP, SKIPP);
    skipButton->setPosition(cocos2d::Vec2(visibleSize.width / 4, visibleSize.height / 2 - visibleSize.height / 3));
    skipButton->setScaleX(visibleSize.width / skipButton->getContentSize().width / 4);
    skipButton->setScaleY(visibleSize.width / skipButton->getContentSize().width / 4);
    skipButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::skipEvent, this));
    this->addChild(skipButton, 500);

    sendButton = ui::Button::create(SEND, SENDP);
    sendButton->setPosition(cocos2d::Vec2(visibleSize.width / 4 * 3, visibleSize.height / 2 - visibleSize.height / 3));
    sendButton->setScaleX(visibleSize.width / sendButton->getContentSize().width / 3.5);
    sendButton->setScaleY(visibleSize.width / sendButton->getContentSize().width / 3.5);
    sendButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::sendEvent, this));
    this->addChild(sendButton, 500);

    gameListener = EventListenerTouchOneByOne::create();

    gameListener->onTouchBegan = [=](Touch *touch, Event *event) {
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

                auto explosion = ParticleExplosion::create();
				explosion->setPosition(Vec2(visibleSize.width / 8 * x + visibleSize.width / 16, visibleSize.height / 8 * y + visibleSize.height / 16));
                explosion->setAutoRemoveOnFinish(true);
                explosion->setStartColor(Color4F(255.0f, 0.0f, 0.0f, 1.0f));
				explosion->setSpeed(250);	
				explosion->setScale(0.5f);	
				this->addChild(explosion, 1000);
            }

            board[7 - y][x].setPiece(board[7 - yPrev][xPrev].getPiece());
            board[7 - yPrev][xPrev].setPiece(nullptr);

            drawNodeBoard[xPrev][yPrev]->clear();
            drawNodeBoard[xPrev][yPrev]->drawSolidRect(Vec2(0, 0), Vec2(visibleSize.width / 8, visibleSize.height / 8), board[7 - yPrev][xPrev].getColor() != Color::WHITE ? Color4F::WHITE : Color4F::GRAY);

			drawNodeBoard[x][y]->removeAllChildren();

            Sprite *spriteToCLone = dynamic_cast<Sprite *>(drawNodeBoard[xPrev][yPrev]->getChildren().at(0));
            Sprite *clonedSprite = Sprite::createWithTexture(spriteToCLone->getTexture());
            clonedSprite->setScale(spriteToCLone->getScaleX(), spriteToCLone->getScaleY());
            clonedSprite->setPosition(spriteToCLone->getPosition());

            drawNodeBoard[x][y]->addChild(clonedSprite, 100);
            drawNodeBoard[xPrev][yPrev]->removeAllChildren();

            if (isFinished) {
                gameListener->setEnabled(false);
                menuButton->setEnabled(false);
                showWinScene();
            }

            if (!isFinished && dynamic_cast<PawnPiece *>(board[7 - y][x].getPiece()) != nullptr && (y == 0 || y == 7)) {
                delete board[7 - y][x].getPiece();
                board[7 - y][x].setPiece(new QueenPiece(turn));

                drawNodeBoard[x][y]->removeAllChildren();
                drawNodeBoard[x][y]->addChild(createPieceSprite(turn == Color::WHITE ? WQ : BQ, Vec2(visibleSize.width / 16, visibleSize.height / 16),turn == Color::WHITE ? 0.08 : 0.035), 100);
            }

            turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;

            isSelected = false;
        }

        delete point;

        return true;
    };

	gameListener->setEnabled(false);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(gameListener, this);

    return true;
}

void HelloWorld::goToMainMenuScene(cocos2d::Ref *sender) {
    auto scene = MainMenu::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
}

void HelloWorld::skipEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        AudioEngine::play2d(CAPTURE_SOUND);

        isOnline = false;

        this->removeChild(player1Box);
		this->removeChild(player2Box);
		this->removeChild(skipButton);
        this->removeChild(blackFilter);
        this->removeChild(sendButton);

        gameListener->setEnabled(true);
        menuButton->setEnabled(true);
    }
}

void HelloWorld::sendEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        AudioEngine::play2d(CAPTURE_SOUND);
		
        if (player1->name.size() >= 2 && !player1->name.empty() && player2->name.size() >= 2 && !player2->name.empty()) {
            savePlayer(player1);
            savePlayer(player2);

            this->removeChild(player1Box);
            this->removeChild(player2Box);
            this->removeChild(skipButton);
            this->removeChild(blackFilter);
            this->removeChild(sendButton);

            gameListener->setEnabled(true);
            menuButton->setEnabled(true);
        }
    }
}

void HelloWorld::drawEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        gameListener->setEnabled(false);
        menuButton->setEnabled(false);
        AudioEngine::play2d(CAPTURE_SOUND);
        showDrawScene();
    }
}

void HelloWorld::stopEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    AudioEngine::stopAll();
	
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete board[i][j].getPiece();
        }
    }

    delete player1;
    delete player2;

    goToMainMenuScene(this);
}

void HelloWorld::showWinScene() {
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
        hidden->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 7));
		
        if (turn == Color::WHITE) {
            addWin(player1);
        } else {
            addWin(player2);
        }
    }

    auto jumpscare = Sprite::create(JUMPSCARE);
    jumpscare->setScaleX(visibleSize.width / jumpscare->getContentSize().width / 3);
    jumpscare->setScaleY(visibleSize.width / jumpscare->getContentSize().width / 3);
    jumpscare->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(jumpscare, 200);

    auto scale = ScaleTo::create(2.0f, 1.1f);
    jumpscare->runAction(scale);

    auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		stopEvent(this, ui::Widget::TouchEventType::ENDED);
		return true;
	};

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::showDrawScene() {
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

    auto jumpscare = Sprite::create(JUMPSCARE_DRAW);
    jumpscare->setScaleX(visibleSize.width / jumpscare->getContentSize().width / 3);
    jumpscare->setScaleY(visibleSize.width / jumpscare->getContentSize().width / 3);
    jumpscare->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(jumpscare, 200);

    auto scale = ScaleTo::create(2.0f, 0.75f);
    jumpscare->runAction(scale);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* touch, Event* event) {
        stopEvent(this, ui::Widget::TouchEventType::ENDED);
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

Vec2 *HelloWorld::getIndexOnClick(cocos2d::Vec2 touchLocation) {
    int x = touchLocation.x / (visibleSize.width / 8);
    int y = touchLocation.y / (visibleSize.height / 8);
    return new Vec2(x, y);
}

void HelloWorld::onSavePlayerCompleted(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response, Player* player) {
    if (!response) {
        isOnline = false;
        return;
    }

    long statusCode = response->getResponseCode();
	
	if (response->isSucceed()) {
        std::vector<char>* buffer = response->getResponseData();

        std::string data(buffer->begin(), buffer->end());

        setPlayerFields(data, player);

        hidden->setString("Sent!");
    } else {
        isOnline = false;
		hidden->setString("Something went wrong!");
    }

    hidden->setVisible(true);
	
	auto delay = DelayTime::create(1.5f);
	auto hide = CallFunc::create([=]() {
		hidden->setVisible(false);
	});

	auto sequence = Sequence::create(delay, hide, nullptr);
	hidden->runAction(sequence);
}

void HelloWorld::onAddWinCompleted(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response, Player* player) {
    if (!response) {
        return;
    }

    long statusCode = response->getResponseCode();

    if (response->isSucceed()) {
        std::vector<char>* buffer = response->getResponseData();

        std::string data(buffer->begin(), buffer->end());

        setPlayerFields(data, player);

        hidden->setString("Player named " + player->name + "\nwith UID " + std::to_string(player->id) + " and " + std::to_string(player->score) + " wins won!");
    }
    else {
		hidden->setString("Something went wrong!");
	}
}

void HelloWorld::savePlayer(Player* player) {
    std::string json = "{\"name\":\"" + player->name + "\"}";

    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();

    std::string url = serverUrl + "/new";

    request->setUrl(url);
    request->setRequestType(network::HttpRequest::Type::POST);

    std::vector <std::string> headers;
    headers.push_back("Content-Type:application/json; charset=utf-8");

    request->setHeaders(headers);
    request->setRequestData(json.c_str(), json.size());
    request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onSavePlayerCompleted, this, player));

    network::HttpClient::getInstance()->send(request);

    request->release();
}

void HelloWorld::addWin(Player* player) {
    std::string url = serverUrl;
    url += "/" + std::to_string(player->id) + "/win";

    network::HttpRequest* request = new network::HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(network::HttpRequest::Type::PUT);
    request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onAddWinCompleted, this, player));

    network::HttpClient::getInstance()->send(request);
    request->release();

    hidden->setString("Getting data...");
    hidden->setVisible(true);
}

void HelloWorld::setPlayerFields(std::string json, Player* player) {
    rapidjson::Document document;

    document.Parse(json.c_str());

    rapidjson::Value& id = document["id"];
    rapidjson::Value& name = document["name"];
    rapidjson::Value& score = document["wins"];

    player->id = id.GetInt();
    player->name = name.GetString();
    player->score = score.GetInt();
}