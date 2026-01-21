#include "server.h"
#include "request_handler.h"
#include "cache.h"
#include "thread_pool.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

HTTPServer::HTTPServer(int port, Protocol protocol)
    : port_(port), protocol_(protocol), server_socket_(-1), running_(false),
      thread_pool_(std::make_unique<ThreadPool>(4)),
      request_handler_(std::make_unique<RequestHandler>()),
      ssl_context_(nullptr) {
}

HTTPServer::~HTTPServer() {
    stop();
    if (protocol_ == Protocol::HTTPS && ssl_context_) {
        SSL_CTX_free(static_cast<SSL_CTX*>(ssl_context_));
    }
}

void HTTPServer::setup_ssl() {
    if (protocol_ != Protocol::HTTPS) return;

    // Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    // Create SSL context
    const SSL_METHOD* method = TLS_server_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        throw std::runtime_error("Failed to create SSL context");
    }

    ssl_context_ = ctx;
    std::cout << "SSL/TLS context initialized\n";
    std::cout << "Note: For production, use proper certificates. This is a demo.\n";
}

void HTTPServer::setup_socket() {
    // Create socket
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    // Set socket options to allow reuse
    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Failed to set socket options");
    }

    // Bind socket to port
    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port_);

    if (bind(server_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Failed to bind socket to port " + std::to_string(port_));
    }

    // Listen for incoming connections
    if (listen(server_socket_, 5) < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }

    std::string protocol_str = (protocol_ == Protocol::HTTPS) ? "HTTPS" : "HTTP";
    std::cout << "Server listening on " << protocol_str << " port " << port_ << "\n";
}

void HTTPServer::accept_connections() {
    while (running_) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int client_socket = accept(server_socket_, 
                                   (struct sockaddr*)&client_addr, 
                                   &client_addr_len);
        if (client_socket < 0) {
            if (running_) {
                std::cerr << "Error accepting connection\n";
            }
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "New connection from " << client_ip << ":" << ntohs(client_addr.sin_port) << "\n";

        // Submit client handling task to thread pool
        thread_pool_->enqueue([this, client_socket]() {
            char buffer[8192] = {0};
            int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

            if (bytes_read > 0) {
                std::string request(buffer);
                std::string response = request_handler_->handle_request(request);
                send(client_socket, response.c_str(), response.size(), 0);
            }

            close(client_socket);
        });
    }
}

void HTTPServer::start() {
    setup_socket();
    if (protocol_ == Protocol::HTTPS) {
        setup_ssl();
    }
    running_ = true;
    accept_connections();
}

void HTTPServer::stop() {
    running_ = false;
    if (server_socket_ >= 0) {
        close(server_socket_);
    }
}
