#pragma once
#include "../client/ws_client.h"
#include <memory>
#include <thread>

class UserInterface {
public:
    UserInterface(std::shared_ptr<WebSocketClient> client);
    void StartLoop();

private:
    std::shared_ptr<WebSocketClient> client_;
    bool is_running_;
};