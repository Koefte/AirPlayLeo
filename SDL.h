

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

void renderText(SDL_Renderer* renderer, const char* text, int x, int y, int fontSize, bool centered);
SDL_Window* initWindow(int width, int height);
SDL_Renderer* initRenderer(SDL_Window* window);
void clearScreen(SDL_Renderer* renderer, int r, int g, int b, int alpha);
