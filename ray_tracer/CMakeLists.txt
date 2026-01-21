project(RayTracer)

set(RAY_TRACER_SOURCES
    src/main.cpp
    src/ray_tracer.cpp
    src/vector3.cpp
    src/scene.cpp
    src/ppm_writer.cpp
    src/texture.cpp
)

set(RAY_TRACER_HEADERS
    include/ray_tracer.h
    include/vector3.h
    include/scene.h
    include/ppm_writer.h
    include/texture.h
)

add_executable(ray_tracer ${RAY_TRACER_SOURCES})

target_include_directories(ray_tracer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ray_tracer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Optional: Link math library and threading
find_package(Threads REQUIRED)
target_link_libraries(ray_tracer PRIVATE m Threads::Threads)
