#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <memory>

/**
 * CacheEntry - A single cached response with TTL
 */
struct CacheEntry {
    std::string response;
    std::chrono::steady_clock::time_point timestamp;
    int ttl_seconds;

    bool is_expired() const {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - timestamp);
        return duration.count() > ttl_seconds;
    }
};

/**
 * ResponseCache - Simple HTTP response caching with TTL
 */
class ResponseCache {
public:
    ResponseCache(int default_ttl = 300);

    // Store response in cache
    void put(const std::string& key, const std::string& response, int ttl_seconds = -1);

    // Retrieve response from cache (returns nullptr if expired or not found)
    std::shared_ptr<std::string> get(const std::string& key);

    // Clear entire cache
    void clear();

    // Remove specific entry
    void remove(const std::string& key);

    // Get cache size
    size_t size() const;

private:
    std::unordered_map<std::string, CacheEntry> cache_;
    mutable std::mutex mutex_;
    int default_ttl_;
};
