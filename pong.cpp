#include <SDL2/SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BALL_SIZE = 15;

int main(int argc, char* argv[]) {
    // 1. Initialize SDL Video Subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. Create Window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Pong",          // Window title
        SDL_WINDOWPOS_UNDEFINED,    // Initial x position
        SDL_WINDOWPOS_UNDEFINED,    // Initial y position
        WINDOW_WIDTH,                        // Width, in pixels
        WINDOW_HEIGHT,                        // Height, in pixels
        SDL_WINDOW_SHOWN            // Flags
    );

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 3. Create Hardware-Accelerated Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 4. Main Loop Flag and Event Handler
    bool isRunning = true;
    SDL_Event event;

    // 5. Game Loop
    while (isRunning) {
        // Handle Events (Input, Window Closing, etc.)
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // Clear Screen (Set color to Dark Blue: R=0, G=20, B=60, A=255)
        SDL_SetRenderDrawColor(renderer, 0, 20, 60, 255);
        SDL_RenderClear(renderer);

        // Render your graphics here (Draw shapes, textures, etc.)
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_Rect filledRect = {WINDOW_WIDTH / 2 - BALL_SIZE, WINDOW_HEIGHT / 2 - BALL_SIZE, BALL_SIZE, BALL_SIZE};
        SDL_RenderFillRect(renderer, &filledRect);
        // Update Screen
        SDL_RenderPresent(renderer);
    }

    // 6. Clean Up and Free Memory
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

