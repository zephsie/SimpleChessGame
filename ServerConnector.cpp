#include "ServerConnector.h"
#include "DEFINITIONS.h"
#include "AudioEngine.h"

USING_NS_CC;

ServerConnector *ServerConnector::singleton = nullptr;

ServerConnector::ServerConnector() {
    serverUrl = SERVER;
}

ServerConnector *ServerConnector::instance() {
    if (nullptr == singleton) {
        singleton = new ServerConnector();
    }
    return singleton;
}

void ServerConnector::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response, Player *player) {
    std::vector<char> *buffer = response->getResponseData();

    std::string data(buffer->begin(), buffer->end());

    setPlayerFields(data, player);
}

void ServerConnector::savePlayer(Player *player) {
    std::string json = "{\"name\":\"" + player->name + "\"}";

    network::HttpRequest *request = new network::HttpRequest();

    std::string url = serverUrl + "/add";

    request->setUrl(url);
    request->setRequestType(network::HttpRequest::Type::POST);

    std::vector <std::string> headers;
    headers.push_back("Content-Type:application/json; charset=utf-8");

    request->setHeaders(headers);
    request->setRequestData(json.c_str(), json.size());
    request->setResponseCallback(CC_CALLBACK_2(ServerConnector::onHttpRequestCompleted, this, player));

    network::HttpClient::getInstance()->send(request);

    request->release();
}

void ServerConnector::setPlayerFields(std::string json, Player *player) {
    rapidjson::Document document;

    document.Parse(json.c_str());

    rapidjson::Value &id = document["id"];
    rapidjson::Value &name = document["name"];
    rapidjson::Value &score = document["wins"];

    player->id = id.GetInt();
    player->name = name.GetString();
    player->score = score.GetInt();
}

void ServerConnector::addWin(long id) {
    std::string url = serverUrl;
    url += "/" + std::to_string(id) + "/win";

    network::HttpRequest *request = new network::HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);

    network::HttpClient::getInstance()->send(request);
    request->release();
}