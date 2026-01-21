project(WebServer)

set(WEB_SERVER_SOURCES
    src/main.cpp
    src/server.cpp
    src/request_handler.cpp
    src/thread_pool.cpp
    src/cache.cpp
)

set(WEB_SERVER_HEADERS
    include/server.h
    include/request_handler.h
    include/thread_pool.h
    include/cache.h
)

add_executable(web_server ${WEB_SERVER_SOURCES})

target_include_directories(web_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(web_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Link pthread for threading support
find_package(Threads REQUIRED)
target_link_libraries(web_server PRIVATE Threads::Threads)

# Link OpenSSL for HTTPS support
find_package(OpenSSL REQUIRED)
target_link_libraries(web_server PRIVATE OpenSSL::SSL OpenSSL::Crypto)
