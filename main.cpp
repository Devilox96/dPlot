#include <iostream>
//-----------------------------//
#include <SDL2/SDL.h>
//-----------------------------//
#include "Renderer.h"
//-----------------------------//
int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);

    auto Window = SDL_CreateWindow(
            "dPlot",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            800, 600,
            SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    if (!Window) {
        std::cout << "Couldn't create the window: " << SDL_GetError() << std::endl;
        exit(-1);
    }

    //----------//

    Renderer*   VulkanRenderer;
    SDL_Event   Event;
    bool        Run                 = true;

    try {
        VulkanRenderer = new Renderer(Window);

        VulkanRenderer -> setApplicationName("dPlot");
        VulkanRenderer -> setApplicationVersion(0, 1, 0);
        VulkanRenderer -> setDebugMode(true);

        VulkanRenderer -> init();

        while (Run) {
            while (SDL_PollEvent(&Event)) {
                if (Event.type == SDL_QUIT) {
                    Run = false;
                }
            }

//            VulkanRenderer -> draw();
        }
    } catch (const std::runtime_error& tExcept) {
        std::cout << tExcept.what() << std::endl;

        SDL_DestroyWindow(Window);
        SDL_Quit();
    }

    SDL_DestroyWindow(Window);
    SDL_Quit();

    //----------//

    return 0;
}