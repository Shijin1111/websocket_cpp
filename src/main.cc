#include "client/ws_client.h"
#include "cli/user_interface.h"
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <thread>

int main() {
    net::io_context ioc;
    
    // Use TLS v1.2 or v1.3
    ssl::context ctx(ssl::context::tlsv12_client);
    try {
        ctx.set_default_verify_paths();
        ctx.set_verify_mode(ssl::verify_none); // Keep verify_none for simple testing
    } catch (std::exception& e) {
        std::cerr << "SSL Context Error: " << e.what() << "\n";
        return 1;
    }

        // ... inside main() ...

    auto client = std::make_shared<WebSocketClient>(ioc, ctx);
    UserInterface ui(client);
    
    // CONFIGURATION: Postman Echo Server
    // This server is reliable and requires no API Key.
    std::string host = "ws.postman-echo.com";
    std::string port = "443";
    std::string path = "/raw";
    
    std::cout << "Connecting to " << host << "...\n";
    client->Connect(host, port, path);

    client->SetOnMessage([](const std::string& msg) {
        std::cout << "\n[Server]: " << msg << "\n> ";
    });

    std::thread io_thread([&ioc]() {
        ioc.run();
    });

    ui.StartLoop();

    if(io_thread.joinable()) io_thread.join();

    return 0;
}