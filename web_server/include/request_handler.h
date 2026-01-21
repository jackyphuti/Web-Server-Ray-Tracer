#pragma once

#include <string>
#include <memory>

class ResponseCache;

/**
 * RequestHandler - Processes HTTP requests and generates responses
 * Supports GET, POST, PUT, DELETE, HEAD, OPTIONS methods
 */
class RequestHandler {
public:
    RequestHandler();

    // Parse and handle an HTTP request, return response
    std::string handle_request(const std::string& raw_request);

    // Get cache instance
    ResponseCache& get_cache() { return *cache_; }

private:
    std::unique_ptr<ResponseCache> cache_;

    // Helper methods for parsing
    std::string parse_method(const std::string& raw_request);
    std::string parse_path(const std::string& raw_request);
    std::string parse_version(const std::string& raw_request);
    std::string parse_body(const std::string& raw_request);
    std::string parse_header(const std::string& raw_request, const std::string& header_name);

    // Response generators
    std::string generate_response(const std::string& method, 
                                  const std::string& path,
                                  const std::string& version,
                                  const std::string& body);
    std::string generate_html_response(const std::string& html_content, 
                                       const std::string& content_type = "text/html");
    std::string generate_json_response(const std::string& json_content);
    std::string generate_error_response(int status_code, 
                                       const std::string& message);
    std::string get_status_text(int status_code) const;

    // HTTP method handlers
    std::string handle_get(const std::string& path);
    std::string handle_post(const std::string& path, const std::string& body);
    std::string handle_put(const std::string& path, const std::string& body);
    std::string handle_delete(const std::string& path);
    std::string handle_head(const std::string& path);
    std::string handle_options(const std::string& path);
};
