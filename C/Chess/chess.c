#include "chess.h"

//Forward declarations

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* defaultFont;
struct Piece pieces[PIECE_COUNT];

void loadPieces() {

	//Name the pieces accordingly

	const char defaultBoard[PIECE_COUNT] = {
		'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
		'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R',
	};

	for (int i = 0; i < PIECE_COUNT; i++) {
		pieces[i].name[0] = (i < 16) ? 'b' : 'w';
		pieces[i].name[1] = defaultBoard[i];
	}

	//Load the textures
	for (int i = 0; i < PIECE_COUNT; i++) {
		//Create the full path
		char path[32] = { "textures/" };
		strcat(path, pieces[i].name);
		strcat(path, ".png");
		//Load onto surface
		SDL_Surface* surface = IMG_Load(path);

		//Texture time
		pieces[i].texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_FreeSurface(surface);

		//Set the sizes
		pieces[i].rect.w = PIECE_SIZE, pieces[i].rect.h = PIECE_SIZE;
	}
}

int main() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO)) {
		printf("Error: %s\n", SDL_GetError());
		exit(0);
	}

	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, 0);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

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
	loadPieces();
	normalBoard();
	gameplay();
	terminate();
}

void terminate() {

	SDL_Delay(4000);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	exit(0);
}
