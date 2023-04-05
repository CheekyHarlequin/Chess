#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//Sizes in pixels
#define WINDOW_SIZE 500

#define PIECE_SIZE (WINDOW_SIZE / 10)
#define BOARD_SIZE WINDOW_SIZE

#define BOARD_X_OFFSET PIECE_SIZE
#define BOARD_Y_OFFSET PIECE_SIZE

//Number of pieces
#define PIECE_COUNT 32

//How many different pieces there are texture wise
#define UNIQUE_PIECE_COUNT 12

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern TTF_Font* defaultFont;

extern SDL_Texture* pieceTextures[UNIQUE_PIECE_COUNT];
extern SDL_Texture *boardTexture, *highlightTexture, *highlightDotTexture;

struct Piece {
	char name[2];
	SDL_Rect rect;
	bool dead;
};

extern SDL_Rect boardRect;

extern struct Piece pieces[PIECE_COUNT];
extern uint16_t pawnBits; //Which pawns have been moved

extern struct Piece *currentlyHeldPiece, lastPiece;

extern int startX, startY, endX, endY, lastPawnDiff;

void start();
void initBoard();
void gameplay();
void terminate();

void handleInput(bool* whois);
int isMoveValid(int startX, int startY, int endX, int endY, char* piece);
int getWinner();

void loadTextures();
void render();
void drawText(SDL_Renderer* renderer,
							const int x,
							const int y,
							const char* text,
							TTF_Font** font,
							const SDL_Color* color);

static inline const int getPieceTextureIndex(char* strTex) {
	int index;
	//This seems fucking stupid... Too bad!
	switch (strTex[1]) {
		case 'R':
			index = 0;
			break;
		case 'N':
			index = 1;
			break;
		case 'B':
			index = 2;
			break;
		case 'Q':
			index = 3;
			break;
		case 'K':
			index = 4;
			break;
		case 'P':
			index = 5;
			break;
	}

	return (strTex[0] == 'b') * 6 + index;
}
