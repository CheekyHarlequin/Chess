#include "chess.h"

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* defaultFont;

int main() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO)) {
		printf("Error: %s\n", SDL_GetError());
		exit(0);
	}

	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, 0);

	renderer = SDL_CreateRenderer(window, -1, 0);
	//SDL_RENDERER_ACCELERATED
	TTF_Init();

	if ((defaultFont = TTF_OpenFont("fonts/good_dog.ttf", 32)) == NULL) {
		printf("Font not found");
		terminate();
	}

	start();
	terminate();
}

//every important function will got here
void start() {
	loadTextures();
	initBoard();

	gameplay();
	terminate();
}

void terminate() {
	SDL_DestroyTexture(boardTexture);

	for (int i = 0; i < UNIQUE_PIECE_COUNT; i++) {
		SDL_DestroyTexture(pieceTextures[i]);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	exit(0);
}
