cmake_minimum_required(VERSION 3.16)
project(CPPProjects)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories for each project
add_subdirectory(web_server)
add_subdirectory(ray_tracer)
