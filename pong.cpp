#include <SDL2/SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 750;
const int BALL_SIZE = 15;
const int PLAYER_HEIGHT = 15;
const int PLAYER_WIDTH = 100;
const int ENEMY_HEIGHT = 15;
const int ENEMY_WIDTH = 100;
const int SPEED = 10;
int playerX = WINDOW_WIDTH / 2 - PLAYER_WIDTH / 2;

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
        
        // Grab a pointer to the entire keyboard state array
        const Uint8* state = SDL_GetKeyboardState(NULL);
        // 3. Check for held down keys using Scancodes
        if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
          playerX -= SPEED;
          std::cout << playerX;
          if (playerX >= WINDOW_WIDTH + PLAYER_WIDTH) {
            playerX += 10;
          }
        }
        if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
          playerX += SPEED;
          std::cout << playerX;
        }



        // Clear Screen (Set color to Dark Blue: R=0, G=20, B=60, A=255)
        SDL_SetRenderDrawColor(renderer, 0, 20, 60, 255);
        SDL_RenderClear(renderer);

        // Render your graphics here (Draw shapes, textures, etc.)

        // Render ball 1st we decide its color using RGB 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // Then we define its values (X Position, Y Position, Width, Height)
        SDL_Rect BALL = {WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE};
        SDL_Rect PLAYER = {playerX, WINDOW_HEIGHT - PLAYER_HEIGHT - 15, PLAYER_WIDTH, PLAYER_HEIGHT};
        SDL_Rect ENEMY = {WINDOW_WIDTH / 2 - ENEMY_WIDTH / 2, ENEMY_HEIGHT, ENEMY_WIDTH, ENEMY_HEIGHT};
        // Render Rect to Screen
        SDL_RenderFillRect(renderer, &BALL);
        SDL_RenderFillRect(renderer, &PLAYER);
        SDL_RenderFillRect(renderer, &ENEMY);
        // Update Screen
        SDL_RenderPresent(renderer);
    }

    // 6. Clean Up and Free Memory
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

