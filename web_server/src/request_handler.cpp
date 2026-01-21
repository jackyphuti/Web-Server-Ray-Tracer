#include "request_handler.h"
#include "cache.h"
#include <sstream>
#include <algorithm>
#include <iostream>

RequestHandler::RequestHandler() : cache_(std::make_unique<ResponseCache>(300)) {
}

std::string RequestHandler::parse_method(const std::string& raw_request) {
    std::istringstream iss(raw_request);
    std::string method;
    iss >> method;
    return method;
}

std::string RequestHandler::parse_path(const std::string& raw_request) {
    std::istringstream iss(raw_request);
    std::string method, path;
    iss >> method >> path;
    return path;
}

std::string RequestHandler::parse_version(const std::string& raw_request) {
    std::istringstream iss(raw_request);
    std::string method, path, version;
    iss >> method >> path >> version;
    return version;
}

std::string RequestHandler::parse_body(const std::string& raw_request) {
    // Find the double CRLF that separates headers from body
    size_t pos = raw_request.find("\r\n\r\n");
    if (pos == std::string::npos) {
        return "";
    }
    return raw_request.substr(pos + 4);
}

std::string RequestHandler::parse_header(const std::string& raw_request, 
                                         const std::string& header_name) {
    std::string search = header_name + ": ";
    size_t pos = raw_request.find(search);
    if (pos == std::string::npos) {
        return "";
    }
    
    size_t start = pos + search.length();
    size_t end = raw_request.find("\r\n", start);
    if (end == std::string::npos) {
        return raw_request.substr(start);
    }
    
    return raw_request.substr(start, end - start);
}

std::string RequestHandler::get_status_text(int status_code) const {
    switch (status_code) {
        case 200: return "OK";
        case 201: return "Created";
        case 204: return "No Content";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        default: return "Unknown";
    }
}

std::string RequestHandler::generate_html_response(const std::string& html_content,
                                                    const std::string& content_type) {
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: " + content_type + "\r\n";
    response += "Content-Length: " + std::to_string(html_content.size()) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += html_content;
    return response;
}

std::string RequestHandler::generate_json_response(const std::string& json_content) {
    return generate_html_response(json_content, "application/json");
}

std::string RequestHandler::generate_error_response(int status_code, 
                                                     const std::string& message) {
    std::string status_text = get_status_text(status_code);
    std::string html = "<html><body><h1>Error " + std::to_string(status_code) + 
                       " - " + status_text + "</h1><p>" + message + "</p></body></html>";
    
    std::string response = "HTTP/1.1 " + std::to_string(status_code) + " " + status_text + "\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + std::to_string(html.size()) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += html;
    return response;
}

std::string RequestHandler::handle_get(const std::string& path) {
    // Check cache first
    auto cached = cache_->get(path);
    if (cached) {
        std::cout << "Cache hit for: " << path << "\n";
        return *cached;
    }

    std::string response;
    
    if (path == "/" || path == "/index.html") {
        std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Welcome</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 50px; }
        h1 { color: #333; }
        .api { background: #f0f0f0; padding: 10px; margin: 10px 0; }
    </style>
</head>
<body>
    <h1>Welcome to C++ Web Server!</h1>
    <p>Server is running successfully with HTTP/1.1 support.</p>
    
    <h2>API Endpoints</h2>
    <div class="api">
        <strong>GET /about</strong> - About page<br>
        <strong>GET /api/data</strong> - Get JSON data<br>
        <strong>POST /api/submit</strong> - Submit data<br>
        <strong>PUT /api/update</strong> - Update data<br>
        <strong>DELETE /api/remove</strong> - Delete data<br>
        <strong>OPTIONS /</strong> - Get allowed methods
    </div>
</body>
</html>
        )";
        response = generate_html_response(html);
        cache_->put(path, response, 300);
    } else if (path == "/about") {
        std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>About</title>
</head>
<body>
    <h1>About</h1>
    <p>This is a high-performance HTTP server written in C++</p>
    <p>Features:</p>
    <ul>
        <li>Multi-threaded request handling</li>
        <li>Response caching with TTL</li>
        <li>Support for multiple HTTP methods</li>
        <li>HTTPS ready with OpenSSL</li>
    </ul>
    <p><a href="/">Home</a></p>
</body>
</html>
        )";
        response = generate_html_response(html);
        cache_->put(path, response, 600);
    } else if (path == "/api/data") {
        std::string json = R"({"status":"success","data":{"server":"C++ HTTP Server","version":"1.1","cached":true}})";
        response = generate_json_response(json);
        cache_->put(path, response, 60);
    } else {
        response = generate_error_response(404, "Page Not Found");
    }

    return response;
}

std::string RequestHandler::handle_post(const std::string& path, const std::string& body) {
    if (path == "/api/submit") {
        std::cout << "POST /api/submit - Body: " << body << "\n";
        std::string json = R"({"status":"success","message":"Data received","length":)" 
                         + std::to_string(body.size()) + "}";
        return generate_json_response(json);
    }
    return generate_error_response(404, "Endpoint not found");
}

std::string RequestHandler::handle_put(const std::string& path, const std::string& body) {
    if (path == "/api/update") {
        std::cout << "PUT /api/update - Body: " << body << "\n";
        std::string json = R"({"status":"success","message":"Resource updated"})";
        return generate_json_response(json);
    }
    return generate_error_response(404, "Endpoint not found");
}

std::string RequestHandler::handle_delete(const std::string& path) {
    if (path == "/api/remove") {
        std::cout << "DELETE /api/remove\n";
        // Clear cache for this path
        cache_->remove(path);
        std::string json = R"({"status":"success","message":"Resource deleted"})";
        return generate_json_response(json);
    }
    return generate_error_response(404, "Endpoint not found");
}

std::string RequestHandler::handle_head(const std::string& path) {
    // HEAD is like GET but without body
    std::string response = handle_get(path);
    size_t body_pos = response.find("\r\n\r\n");
    if (body_pos != std::string::npos) {
        return response.substr(0, body_pos + 4);
    }
    return response;
}

std::string RequestHandler::handle_options(const std::string& path) {
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Allow: GET, POST, PUT, DELETE, HEAD, OPTIONS\r\n";
    response += "Content-Length: 0\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    return response;
}

std::string RequestHandler::generate_response(const std::string& method,
                                               const std::string& path,
                                               const std::string& version,
                                               const std::string& body) {
    if (method == "GET") {
        return handle_get(path);
    } else if (method == "POST") {
        return handle_post(path, body);
    } else if (method == "PUT") {
        return handle_put(path, body);
    } else if (method == "DELETE") {
        return handle_delete(path);
    } else if (method == "HEAD") {
        return handle_head(path);
    } else if (method == "OPTIONS") {
        return handle_options(path);
    }
    
    return generate_error_response(405, "Method Not Allowed");
}

std::string RequestHandler::handle_request(const std::string& raw_request) {
    std::string method = parse_method(raw_request);
    std::string path = parse_path(raw_request);
    std::string version = parse_version(raw_request);
    std::string body = parse_body(raw_request);

    return generate_response(method, path, version, body);
}
