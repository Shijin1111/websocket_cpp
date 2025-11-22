#include "client/ws_client.h"
#include "cli/user_interface.h"
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <thread>

int main() {
    // 1. Setup Networking Contexts
    net::io_context ioc;
    
    // 2. SSL Context Setup
    ssl::context ctx(ssl::context::tlsv12_client);
    try {
        ctx.set_default_verify_paths();
        // NOTE: For testing echo servers that might have self-signed certs,
        // we can set verify_none. In production, use verify_peer.
        ctx.set_verify_mode(ssl::verify_none); 
    } catch (std::exception& e) {
        std::cerr << "SSL Context Error: " << e.what() << "\n";
        return 1;
    }

    // 3. Initialize Client
    auto client = std::make_shared<WebSocketClient>(ioc, ctx);

    // 4. Setup UI
    UserInterface ui(client);
    
    // 5. Connect (using piehost echo server as it supports SSL reliably)
    // URL: wss://demo.piesocket.com/v3/channel_1?api_key=VCXCEuvhGcBDP7XhiJJUDvR1e1D3eiVjgZ9VRiaV&notify_self
    // Host: demo.piesocket.com, Port: 443
    std::string host = "demo.piesocket.com";
    std::string port = "443";
    
    std::cout << "Connecting to " << host << "...\n";
    client->Connect(host, port);

    client->SetOnMessage([](const std::string& msg) {
        std::cout << "\n[Server]: " << msg << "\n> ";
    });

    // 6. Run IO context in a separate thread
    std::thread io_thread([&ioc]() {
        ioc.run();
    });

    // 7. Run UI on main thread
    ui.StartLoop();

    // Cleanup
    if(io_thread.joinable()) io_thread.join();

    return 0;
}