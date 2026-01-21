#include "cache.h"

ResponseCache::ResponseCache(int default_ttl) : default_ttl_(default_ttl) {
}

void ResponseCache::put(const std::string& key, const std::string& response, int ttl_seconds) {
    std::unique_lock<std::mutex> lock(mutex_);
    int ttl = (ttl_seconds < 0) ? default_ttl_ : ttl_seconds;
    cache_[key] = {response, std::chrono::steady_clock::now(), ttl};
}

std::shared_ptr<std::string> ResponseCache::get(const std::string& key) {
    std::unique_lock<std::mutex> lock(mutex_);
    auto it = cache_.find(key);
    
    if (it == cache_.end()) {
        return nullptr;
    }

    if (it->second.is_expired()) {
        cache_.erase(it);
        return nullptr;
    }

    return std::make_shared<std::string>(it->second.response);
}

void ResponseCache::clear() {
    std::unique_lock<std::mutex> lock(mutex_);
    cache_.clear();
}

void ResponseCache::remove(const std::string& key) {
    std::unique_lock<std::mutex> lock(mutex_);
    cache_.erase(key);
}

size_t ResponseCache::size() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return cache_.size();
}
