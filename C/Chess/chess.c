#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

//Sizes in pixels
#define PIECE_SIZE 100
#define BOARD_SIZE 1000
#define WINDOW_SIZE 1000

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* defaultFont;

//Forward declarations
void play();
void normalBoard();
void gameplay();
void terminate();
void drawText(SDL_Renderer* renderer, int x, int y, char* text, TTF_Font** font, SDL_Color* color);

//The board
char pieceFilePaths[32][3] = { "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR", "bP", "bP", "bP",
															 "bP", "bP", "bP", "bP", "bP", "wP", "wP", "wP", "wP", "wP", "wP",
															 "wP", "wP", "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR" };

struct SDL_Rect piecerects[32];

int main() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
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

	play();

	drawText(renderer, 500, 350, "Aomgus", NULL, NULL);

	SDL_RenderPresent(renderer);

	SDL_Delay(3000);

	terminate();
}

void terminate() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	exit(0);
}

void play() {
	normalBoard();
	gameplay();
}

//NormalBoard sets Board to default including Pieces
void normalBoard() {
	SDL_Surface* surface;

	//Draw the board
	SDL_Rect boardrect;

	surface = IMG_Load("textures/Chess.jpg");

	SDL_Texture* boardTex = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_QueryTexture(boardTex, NULL, NULL, &boardrect.w, &boardrect.h);

	//resize and change pos of the board rect.
	boardrect.w, boardrect.h = BOARD_SIZE;
	boardrect.x, boardrect.y = 0;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(renderer, boardTex, NULL, &boardrect);

	SDL_Texture* pieceTex;

	for (int i = 0, counter = 1; i < 32; i++, counter = ((counter >= 8) ? 1 : counter + 1)) {
		char path[32] = { "textures/" };
		strcat(path, pieceFilePaths[i]);
		strcat(path, ".png");

		surface = IMG_Load(path);

		pieceTex = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_QueryTexture(pieceTex, NULL, NULL, &piecerects[i].w, &piecerects[i].h);

		piecerects[i].w = PIECE_SIZE, piecerects[i].h = PIECE_SIZE;

		if (i < 16) {
			piecerects[i].x = (100 * counter);
			piecerects[i].y = ((i < 8) ? 100 : 200);
		} else {
			piecerects[i].x = (100 * counter);
			piecerects[i].y = ((i < 24) ? 700 : 800);
		}

		SDL_FreeSurface(surface);
		SDL_RenderCopy(renderer, pieceTex, NULL, &piecerects[i]);
		SDL_DestroyTexture(pieceTex);
	}

	SDL_DestroyTexture(boardTex);
}

void gameplay() {}

void drawText(SDL_Renderer* renderer, int x, int y, char* text, TTF_Font** font, SDL_Color* textColor) {
	const SDL_Color defaultColor = { 0, 0, 0, 0 };

	//Draw with the according color
	SDL_Surface* surface =
		TTF_RenderText_Solid((font == NULL) ? defaultFont : *font, text, (textColor == NULL) ? defaultColor : *textColor);

	//Draw to texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	//Adjust position
	SDL_Rect textRect;

	SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);

	textRect.x = x, textRect.y = y;

	SDL_RenderCopy(renderer, texture, NULL, &textRect);
}
