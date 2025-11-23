#include "ws_client.h"
#include <iostream>

WebSocketClient::WebSocketClient(net::io_context& ioc, ssl::context& ctx)
    : resolver_(net::make_strand(ioc))
    , ws_(net::make_strand(ioc), ctx) {
}

void WebSocketClient::Connect(const std::string& host, const std::string& port, const std::string& path) {
    host_ = host;
    path_ = path; // Store path
    resolver_.async_resolve(host, port,
        beast::bind_front_handler(&WebSocketClient::OnResolve, shared_from_this()));
}

void WebSocketClient::OnResolve(beast::error_code ec, tcp::resolver::results_type results) {
    if(ec) return (void)(std::cerr << "Resolve Error: " << ec.message() << "\n");

    beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));
    beast::get_lowest_layer(ws_).async_connect(results,
        beast::bind_front_handler(&WebSocketClient::OnConnect, shared_from_this()));
}

void WebSocketClient::OnConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
    if(ec) return (void)(std::cerr << "Connect Error: " << ec.message() << "\n");

    // [IMPORTANT FIX] Set SNI Hostname.
    // Many servers (cloud hosted) require this to know which certificate to serve.
    if(!SSL_set_tlsext_host_name(ws_.next_layer().native_handle(), host_.c_str())) {
        beast::error_code error(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category());
        std::cerr << "SNI Error: " << error.message() << "\n";
        return;
    }

    // Perform SSL Handshake
    ws_.next_layer().async_handshake(ssl::stream_base::client,
        beast::bind_front_handler(&WebSocketClient::OnSslHandshake, shared_from_this()));
}

void WebSocketClient::OnSslHandshake(beast::error_code ec) {
    if(ec) return (void)(std::cerr << "SSL Error: " << ec.message() << "\n");

    beast::get_lowest_layer(ws_).expires_never();
    
    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Decorator to add User-Agent
    ws_.set_option(websocket::stream_base::decorator(
        [](websocket::request_type& req)
        {
            req.set(http::field::user_agent, "CPP-WebSocket-Client");
        }));

    // [FIX] Use the specific path (with API key) instead of just "/"
    ws_.async_handshake(host_, path_,
        beast::bind_front_handler(&WebSocketClient::OnHandshake, shared_from_this()));
}

void WebSocketClient::OnHandshake(beast::error_code ec) {
    if(ec) return (void)(std::cerr << "Handshake Error: " << ec.message() << "\n");
    
    std::cout << "[System] Connected securely to " << host_ << "\n";
    
    ws_.async_read(buffer_,
        beast::bind_front_handler(&WebSocketClient::OnRead, shared_from_this()));
}

void WebSocketClient::Send(const std::string& message, bool is_binary) {
    ws_.binary(is_binary);
    ws_.async_write(net::buffer(message),
        beast::bind_front_handler(&WebSocketClient::OnWrite, shared_from_this()));
}

void WebSocketClient::OnWrite(beast::error_code ec, std::size_t bytes_transferred) {
    if(ec) std::cerr << "Write Error: " << ec.message() << "\n";
}

void WebSocketClient::OnRead(beast::error_code ec, std::size_t bytes_transferred) {
    if(ec) return (void)(std::cerr << "Read Error: " << ec.message() << "\n"); // Don't stop loop on read error immediately unless fatal

    if (on_message_) {
        on_message_(beast::buffers_to_string(buffer_.data()));
    }
    buffer_.consume(buffer_.size());

    ws_.async_read(buffer_,
        beast::bind_front_handler(&WebSocketClient::OnRead, shared_from_this()));
}

void WebSocketClient::Close() {
    ws_.async_close(websocket::close_code::normal,
        beast::bind_front_handler(&WebSocketClient::OnClose, shared_from_this()));
}

void WebSocketClient::OnClose(beast::error_code ec) {
    if(ec) std::cerr << "Close Error: " << ec.message() << "\n";
    std::cout << "[System] Connection closed.\n";
}

void WebSocketClient::SetOnMessage(MessageCallback cb) {
    on_message_ = cb;
}