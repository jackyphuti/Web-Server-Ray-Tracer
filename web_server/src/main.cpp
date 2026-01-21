#include "server.h"
#include <iostream>

int main(int argc, char* argv[]) {
    int port = 8080;

    // Parse command line arguments
    if (argc > 1) {
        port = std::stoi(argv[1]);
    }

    std::cout << "Starting HTTP Server on port " << port << "...\n";

    try {
        HTTPServer server(port);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
