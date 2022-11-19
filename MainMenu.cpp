#include "MainMenu.h"
#include "ui/CocosGUI.h"
#include "HelloWorldScene.h"
#include "DEFINITIONS.h"
#include "AudioEngine.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;

cocos2d::Scene *MainMenu::createScene() {
    auto scene = cocos2d::Scene::create();
    auto layer = MainMenu::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenu::init() {
    if (!Layer::init()) {
        return false;
    }

    serverUrl = SERVER;

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

    button = ui::Button::create(START, STARTP);
    button->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 8));
    button->setScaleX(visibleSize.width / button->getContentSize().width / 2);
    button->setScaleY(visibleSize.width / button->getContentSize().width / 2);
    this->addChild(button);

    button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            AudioEngine::stopAll();
            AudioEngine::play2d(CAPTURE_SOUND);
            goToGameScene(sender);
        }
    });

    leave = ui::Button::create(LEAVE, LEAVE);
    leave->setPosition(cocos2d::Vec2(visibleSize.width / 8, visibleSize.height / 8));
    leave->setScaleX(visibleSize.width / button->getContentSize().width / 25);
    leave->setScaleY(visibleSize.width / button->getContentSize().width / 25);
    this->addChild(leave);
	
	leave->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) {
			Director::getInstance()->end();
		}
	});

	chalice = ui::Button::create(CHALICE, CHALICE);
	chalice->setPosition(cocos2d::Vec2(7 * visibleSize.width / 8, visibleSize.height / 8));
	chalice->setScaleX(visibleSize.width / button->getContentSize().width / 7);
	chalice->setScaleY(visibleSize.width / button->getContentSize().width / 7);
	chalice->addTouchEventListener(CC_CALLBACK_2(MainMenu::showTopEvent, this));
	this->addChild(chalice);

	topPlayers = cocos2d::Label::createWithTTF("", FONT, 12);
	topPlayers->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));
    topPlayers->setVisible(false);
    topPlayers->setTextColor(Color4B::RED);
    topPlayers->setAlignment(TextHAlignment::CENTER);
	this->addChild(topPlayers, 1000);

    return true;
}

void MainMenu::goToGameScene(cocos2d::Ref *sender) {
    auto scene = HelloWorld::createScene();

    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1, scene));
}

void MainMenu::showTopEvent(Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
    button->setEnabled(false);
	leave->setEnabled(false);
	chalice->setEnabled(false);

    std::string url = serverUrl;
	url += "/top";

    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback(CC_CALLBACK_2(MainMenu::onShowTopCompleted, this));

    network::HttpClient::getInstance()->send(request);
    request->release();
	
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	auto blackFilter = cocos2d::LayerColor::create(Color4B(0, 0, 0, 255), visibleSize.width, visibleSize.height);
	blackFilter->setPosition(cocos2d::Vec2(0, 0));
	this->addChild(blackFilter);

	auto listener = EventListenerTouchOneByOne::create();
	
	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		blackFilter->removeFromParent();
        topPlayers->setVisible(false);

        button->setEnabled(true);
        leave->setEnabled(true);
        chalice->setEnabled(true);
		
        return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, blackFilter);

    topPlayers->setVisible(true);
    topPlayers->setString("Getting data...");
}

void MainMenu::onShowTopCompleted(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response) {
    if (!response) {
        return;
    }

    long statusCode = response->getResponseCode();

    if (response->isSucceed()) {
        std::vector<char>* buffer = response->getResponseData();

        std::string data(buffer->begin(), buffer->end());

        parseData(data);
    } else {
        topPlayers->setString("Something went wrong!");
    }
}

void MainMenu::parseData(std::string json) {
	rapidjson::Document document;
	document.Parse<0>(json.c_str());

	std::string top = "TOP PLAYERS\n\n\n";
	
    for (rapidjson::SizeType i = 0; i < document.Size(); i++) {
		std::string name = document[i]["name"].GetString();
		int wins = document[i]["wins"].GetInt();

        top += name + " - " + std::to_string(wins) + "wins\n\n";
    }

    topPlayers->setString(top);
}