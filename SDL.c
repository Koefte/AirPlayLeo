#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, int fontSize) {
    // Set text color
    SDL_Color color = {0, 0, 0, 255}; // Black color

    // Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
    if (textSurface == NULL) {
        printf("Failed to render text: %s\n", TTF_GetError());
        return;
    }

    // Create texture from surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (textTexture == NULL) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        return;
    }

    // Query texture size
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Set destination rectangle
    SDL_Rect dstRect = { x, y, textWidth, textHeight };

    // Render texture
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

    // Clean up texture
    SDL_DestroyTexture(textTexture);
}


int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
        printf("SDL or SDL_ttf initialization error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
		SDL_ShowCursor(SDL_DISABLE);

    // Load a system font
    TTF_Font* font = TTF_OpenFont("WorkSans-Regular.ttf", 24); // NULL loads the default font with size 24
    if (font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create text surface
    SDL_Color color = {0, 0, 0}; // White color
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Hello, SDL!", color);
    if (textSurface == NULL) {
        printf("Failed to render text: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); // Free the surface as it's no longer needed
    if (texture == NULL) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 255,255, 255, 255); // Black color
    SDL_RenderClear(renderer);

    // Render texture
    SDL_Rect dstRect = {100, 100, 0, 0}; // Position of the text
    SDL_QueryTexture(texture, NULL, NULL, &dstRect.w, &dstRect.h);
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Main loop
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                quit = true;
            }
        }
    }

    // Clean up resources
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
