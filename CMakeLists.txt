cmake_minimum_required(VERSION 3.2)
project(CoesiusLibrary VERSION 0.0.3)

#=============================================================================
# Local settings

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT MSVC)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(
            -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization
            -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept
            -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo
            -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Og -g3 -std=c++2b
        )
    endif()
    if(CMAKE_BUILD_TYPE MATCHES Release)
        add_compile_options(
            -O3 -march=native -std=c++2b
        )
    endif()
endif()

#=============================================================================
# Dependencies

include_directories()

find_package(benchmark REQUIRED)

#=============================================================================
# Targets

# Coesius
add_library(Finst INTERFACE)
target_sources(Finst INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/finst.hpp>
)
target_compile_features(Finst INTERFACE cxx_std_23)
target_include_directories(Finst INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Finst INTERFACE)

# v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^

# Coesius DEV
add_library(Finst_DEV INTERFACE)
target_sources(Finst_DEV INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/finst.hpp>
)
target_compile_features(Finst_DEV INTERFACE cxx_std_23)
target_include_directories(Finst_DEV INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(Finst_DEV INTERFACE benchmark::benchmark)

#=============================================================================

# CoesiusLibrary LOCAL
add_executable(DEV dev/main.cpp)
target_link_libraries(DEV PRIVATE benchmark::benchmark Finst_DEV)

#=============================================================================
# Install

install(TARGETS Finst EXPORT Finst)
install(DIRECTORY include/ TYPE INCLUDE)
install(EXPORT Finst DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/finst NAMESPACE finst::)