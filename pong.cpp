#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 750;
const int BALL_SIZE = 15;
const int PLAYER_HEIGHT = 15;
const int PLAYER_WIDTH = 100;
const int ENEMY_HEIGHT = 15;
const int ENEMY_WIDTH = 100;
const int SPEED = 10;
double BALL_SPEEDX = 2.0;
double BALL_SPEEDY = 2.0;
const int playerY = WINDOW_HEIGHT - PLAYER_HEIGHT - 15;
const int enemyY = WINDOW_HEIGHT / 50;
int playerX = WINDOW_WIDTH / 2 - PLAYER_WIDTH / 2;
int enemyX = WINDOW_WIDTH / 2 - PLAYER_WIDTH / 2;
int ballX = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
int ballY = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
int playerScore = 0;
int enemyScore = 0;

int main(int argc, char* argv[]) {
    // 1. Initialize SDL Video Subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
      std::cout << "TTF Init Error: " << TTF_GetError() << std::endl;
      SDL_Quit();
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
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 24);

    if (!font) {
        std::cout << "Font Load Error: " << TTF_GetError() << std::endl;
        // Cleanup if font missing
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* pScoreSurface = TTF_RenderText_Blended(font, std::to_string(playerScore).c_str(), textColor);
    SDL_Texture* pScoreTexture = SDL_CreateTextureFromSurface(renderer, pScoreSurface);

    SDL_Surface* eScoreSurface = TTF_RenderText_Blended(font, std::to_string(enemyScore).c_str(), textColor);
    SDL_Texture* eScoreTexture = SDL_CreateTextureFromSurface(renderer, eScoreSurface);

        // Define placement and scale matching text dimensions
    SDL_Rect pScoreRect;
    pScoreRect.x = 100; // X coordinate
    pScoreRect.y = 100; // Y coordinate
    pScoreRect.w = pScoreSurface->w; // Use text surface width
    pScoreRect.h = pScoreSurface->h; // Use text surface height

    SDL_FreeSurface(pScoreSurface);


    SDL_Rect eScoreRect;
    eScoreRect.x = 675; // X coordinate
    eScoreRect.y = 100; // Y coordinate
    eScoreRect.w = eScoreSurface->w; // Use text surface width
    eScoreRect.h = eScoreSurface->h; // Use text surface height

    SDL_FreeSurface(eScoreSurface);

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

        ballX += BALL_SPEEDX;
        ballY += BALL_SPEEDY;
        
        playerX = std::clamp(playerX, 0, WINDOW_WIDTH - PLAYER_WIDTH);
        enemyX = std::clamp(enemyX, 0, WINDOW_WIDTH - PLAYER_WIDTH);
        // Grab a pointer to the entire keyboard state array
        const Uint8* state = SDL_GetKeyboardState(NULL);
        // 3. Check for held down keys using Scancodes
        if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
          playerX -= SPEED;
        }

        if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
          playerX += SPEED;
        }
        
        if (enemyX < ballX) {
          enemyX += SPEED;
        } else if (enemyX > ballX) {
          enemyX += -SPEED;
        }

        if (ballX <= 0) {
          ballX = 0;
          BALL_SPEEDX = -BALL_SPEEDX;
        } else if (ballX + BALL_SIZE >= WINDOW_WIDTH) {
          ballX = WINDOW_WIDTH - BALL_SIZE;
          BALL_SPEEDX =-BALL_SPEEDX;
        }

        if (ballY <= 0) {
          ballX = WINDOW_WIDTH / 2;
          ballY = WINDOW_HEIGHT / 2;
          playerScore += 1;

          if (pScoreTexture != nullptr) {
            SDL_DestroyTexture(pScoreTexture);
          }

          SDL_Surface* pScoreSurface = TTF_RenderText_Blended(font, std::to_string(playerScore).c_str(), textColor);

          pScoreTexture = SDL_CreateTextureFromSurface(renderer, pScoreSurface);

          pScoreRect.w = pScoreSurface->w;
          pScoreRect.h = pScoreSurface->h;

          SDL_FreeSurface(pScoreSurface);

        } else if (ballY >= WINDOW_HEIGHT) {
          ballX = WINDOW_WIDTH / 2;
          ballY = WINDOW_HEIGHT / 2;
          enemyScore += 1;

          if (eScoreTexture != nullptr) {
            SDL_DestroyTexture(eScoreTexture);
          }

          SDL_Surface* eScoreSurface = TTF_RenderText_Blended(font, std::to_string(enemyScore).c_str(), textColor);

          eScoreTexture = SDL_CreateTextureFromSurface(renderer, eScoreSurface);

          pScoreRect.w = pScoreSurface->w;
          pScoreRect.h = pScoreSurface->h;

          SDL_FreeSurface(eScoreSurface);

        }
        // Clear Screen (Set color to Dark Blue: R=0, G=20, B=60, A=255)
        SDL_SetRenderDrawColor(renderer, 0, 20, 60, 255);
        SDL_RenderClear(renderer);

        // Render your graphics here (Draw shapes, textures, etc.)

        // Render ball 1st we decide its color using RGB 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // Then we define its values (X Position, Y Position, Width, Height)
        SDL_Rect BALL = {ballX, ballY, BALL_SIZE, BALL_SIZE};
        SDL_Rect PLAYER = {playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT};
        SDL_Rect ENEMY = {enemyX, enemyY, ENEMY_WIDTH, ENEMY_HEIGHT};
        // Render Rect to Screen
        SDL_RenderFillRect(renderer, &BALL);
        SDL_RenderFillRect(renderer, &PLAYER);
        SDL_RenderFillRect(renderer, &ENEMY);

        if (SDL_HasIntersection(&BALL, &PLAYER)) {
          ballY = playerY - PLAYER_HEIGHT; // Push ball to the top of paddle
          BALL_SPEEDY = -BALL_SPEEDY; // Reverse Vertical Direction                           
          BALL_SPEEDY += 0.1;
          BALL_SPEEDX += 0.1;

          if (ballX > 0 && ballX < playerX / 2) {
            BALL_SPEEDX *= -BALL_SPEEDX;
          }
        }

        if (SDL_HasIntersection(&BALL, &ENEMY)) {
          ballY = enemyY + ENEMY_HEIGHT; // Push ball to the top of paddle
          BALL_SPEEDY = -BALL_SPEEDY; // Reverse Vertical Direction                           
          BALL_SPEEDY += 0.1;
          BALL_SPEEDX += 0.1;
        }

        SDL_RenderCopy(renderer, pScoreTexture, NULL, &pScoreRect);
        SDL_RenderCopy(renderer, eScoreTexture, NULL, &eScoreRect);
        // Update Screen
        SDL_RenderPresent(renderer);
    }

    // 6. Clean Up and Free Memory
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

