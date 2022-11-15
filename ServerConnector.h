#ifndef SIMPLECHESSGAME_SERVERCONNECTOR_H
#define SIMPLECHESSGAME_SERVERCONNECTOR_H

#include "cocos2d.h"
#include "Spot.h"
#include "network/HttpClient.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include <string>
#include "Player.h"

class ServerConnector {
private:

    ServerConnector();

    static ServerConnector* singleton;

    std::string serverUrl;

public:

	ServerConnector(ServerConnector const&) = delete;

	ServerConnector& operator=(ServerConnector const&) = delete;	

    static ServerConnector* instance();

    void onHttpRequestCompleted(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response, Player* player);

    void savePlayer(Player* player);

    void setPlayerFields(std::string json, Player* player);

    void addWin(long id);
};

#endif
