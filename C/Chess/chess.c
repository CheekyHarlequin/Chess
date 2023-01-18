#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <string.h>

//Sizes in pixels
#define PIECE_SIZE 100;
#define BOARD_SIZE 1000;

SDL_Renderer* renderer;
SDL_Surface* surface;
SDL_Window* window;

char pieceFilePaths[32][3] = { "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR", "bP", "bP", "bP",
																"bP", "bP", "bP", "bP", "bP", "wP", "wP", "wP", "wP", "wP", "wP",
																"wP", "wP", "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR" };

struct SDL_Rect piecerects[32];



void play();
void normalBoard();

int main() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("Error: %s\n", SDL_GetError());
		return 0;
	}

	window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	play();

	SDL_Delay(3000);

	return 0;
}

void play() {
	normalBoard();
}

void normalBoard() {
	//Draw the board
	SDL_Rect boardrect;
	surface = IMG_Load("textures/Chess.jpg");
	SDL_Texture* boardTex = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(boardTex, NULL, NULL, &boardrect.w, &boardrect.h);

	//resize and change pos of the board rect.
	boardrect.w = BOARD_SIZE;
	boardrect.h = BOARD_SIZE;
	boardrect.x = 0;
	boardrect.y = 0;
	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, boardTex, NULL, &boardrect);

	SDL_Texture* pieceTex;
	char counter = 1;

	for (int i = 0; i < 32; i++) {
		char path[32] = { "textures/" };
		strcat(path, pieceFilePaths[i]);
		strcat(path, ".png");

		surface = IMG_Load(path);

		pieceTex = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_QueryTexture(pieceTex, NULL, NULL, &piecerects[i].w, &piecerects[i].h);

		piecerects[i].w = PIECE_SIZE;
		piecerects[i].h = PIECE_SIZE;

		if (i < 16) {
			counter = ((counter >= 9) ? 1 : counter);
			piecerects[i].x = (100 * counter);
			piecerects[i].y = ((i < 8 && i != 15) ? 100 : 200);
		} else {
			counter = ((counter >= 9) ? 1 : counter);
			piecerects[i].x = (100 * counter);
			piecerects[i].y = ((i < 24) ? 700 : 800);
		}

		SDL_FreeSurface(surface);
		SDL_RenderCopy(renderer, pieceTex, NULL, &piecerects[i]);
		SDL_DestroyTexture(pieceTex);
		counter++;
	}

	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(boardTex);
}
