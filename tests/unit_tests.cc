#include "../src/client/ws_client.h"
#include <iostream>
#include <cassert>

void TestInitialization() {
    net::io_context ioc;
    ssl::context ctx(ssl::context::tlsv12_client);
    auto client = std::make_shared<WebSocketClient>(ioc, ctx);
    assert(client != nullptr);
    std::cout << "[PASS] Initialization Test\n";
}

int main() {
    std::cout << "Running Unit Tests...\n";
    TestInitialization();

    std::cout << "All Tests Passed.\n";
    return 0;
}