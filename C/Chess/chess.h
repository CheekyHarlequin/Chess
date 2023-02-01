#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


//Sizes in pixels
#define PIECE_SIZE 100
#define BOARD_SIZE 1000
#define WINDOW_SIZE 1000
//equals number off all pieces
#define PIECE_COUNT 32


extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern TTF_Font* defaultFont;


// actual game
void start();
void gameplay();
void getPieceMovement(bool whois);
void moveValidCheck(int piecexstart, int pieceystart, int piecexend, int pieceyend, bool whois);
void loadPieces();
int winner();
void renderBoard();
void normalBoard();
void drawText(SDL_Renderer* renderer,
							const int x,
							const int y,
							const char* text,
							TTF_Font** font,
							const SDL_Color* color);
//close everything, end of programm
void terminate();



//The board
struct Piece {
	char name[2];
	SDL_Texture* texture;
	SDL_Rect rect;
};

extern struct Piece pieces[PIECE_COUNT];
