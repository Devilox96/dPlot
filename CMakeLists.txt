cmake_minimum_required(VERSION 3.15)
project(dPlot)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(SDL2 REQUIRED FATAL_ERROR)

add_executable(dPlot
        main.cpp
        Renderer.cpp
        dVulkanMesh.cpp
        dDensityPlot2D.cpp dPlotMeshBase.cpp dPlotMeshBase.h)

target_include_directories(dPlot
        PRIVATE Vulkan::Vulkan
        PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(dPlot
        Vulkan::Vulkan
        ${SDL2_LIBRARIES})