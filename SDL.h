#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

static bool isDestroyed = false;
void renderText(SDL_Renderer* renderer,const char* text, int x, int y, int fontSize,bool centered) {
    // Set text color
    SDL_Color color = {0, 0, 0, 255}; // Black color
    
    TTF_Font* font = TTF_OpenFont("WorkSans-Regular.ttf",fontSize);
    if(font == NULL){
    	printf("Failed to open font: %s \n",TTF_GetError());
	return;
    }
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
    
    if(centered){
        x = (1920 - textWidth)  / 2;
        y = (1080 - textHeight) / 2;
    }

    // Set destination rectangle
    SDL_Rect dstRect = { x, y, textWidth, textHeight };

    // Render texture
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

    // Clean up texture
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

SDL_Window*  initWindow(int width,int height){
	SDL_Window* window = SDL_CreateWindow("SDL Window",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_SHOWN);
        if (window == NULL) {
	        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
	        TTF_Quit();
	        SDL_Quit();
	        return NULL;
	}
	return window;

}

SDL_Renderer* initRenderer(SDL_Window* window){
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return NULL;
    }
    return renderer;

}

void clearScreen(SDL_Renderer* renderer,int r,int g,int b,int alpha){
	SDL_SetRenderDrawColor(renderer,r,g,b,alpha);
	SDL_RenderClear(renderer);
}

void init(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0) {
        printf("SDL or SDL_ttf initialization error: %s\n", SDL_GetError());
    }

}

void destroy(SDL_Renderer *renderer,SDL_Window *window){
	if(!isDestroyed){
	    SDL_DestroyRenderer(renderer);
	    SDL_DestroyWindow(window);
	    TTF_Quit();
	    SDL_Quit();
	}
	isDestroyed = false;
}


