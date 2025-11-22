#include "user_interface.h"
#include <iostream>
#include <string>

UserInterface::UserInterface(std::shared_ptr<WebSocketClient> client) 
    : client_(client), is_running_(true) {}

void UserInterface::StartLoop() {
    std::cout << "Commands:\n";
    std::cout << "  /text <msg>   : Send text message\n";
    std::cout << "  /bin <msg>    : Send binary message\n";
    std::cout << "  /quit         : Exit\n";
    std::cout << "> ";

    std::string line;
    while (is_running_ && std::getline(std::cin, line)) {
        if (line == "/quit") {
            client_->Close();
            is_running_ = false;
            break;
        } else if (line.rfind("/text ", 0) == 0) {
            client_->Send(line.substr(6), false);
        } else if (line.rfind("/bin ", 0) == 0) {
            client_->Send(line.substr(5), true);
        } else {
            std::cout << "Unknown command. Try /text or /bin.\n";
        }
        std::cout << "> ";
    }
}