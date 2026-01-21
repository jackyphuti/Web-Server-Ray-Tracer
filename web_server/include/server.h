#pragma once

#include <string>
#include <memory>

class RequestHandler;
class ThreadPool;

/**
 * HTTPServer - A multi-protocol server supporting both HTTP and HTTPS
 * Handles incoming requests using a thread pool
 */
class HTTPServer {
public:
    enum class Protocol { HTTP, HTTPS };

    HTTPServer(int port = 8080, Protocol protocol = Protocol::HTTP);
    ~HTTPServer();

    // Start the server (blocks until server stops)
    void start();

    // Stop the server gracefully
    void stop();

    // Get the port the server is listening on
    int get_port() const { return port_; }

    // Get protocol
    Protocol get_protocol() const { return protocol_; }

private:
    int port_;
    Protocol protocol_;
    int server_socket_;
    bool running_;
    std::unique_ptr<ThreadPool> thread_pool_;
    std::unique_ptr<RequestHandler> request_handler_;

    // TLS/SSL members
    void* ssl_context_;  // Actually SSL_CTX*

    // Helper methods
    void setup_socket();
    void setup_ssl();
    void accept_connections();
};

