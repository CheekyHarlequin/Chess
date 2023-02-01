#include "chess.h"


void renderBoard() {

  SDL_Rect boardrect;

	SDL_Surface* surface = IMG_Load("textures/Chess.jpg");

	SDL_Texture* boardTex = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	boardrect.w = BOARD_SIZE, boardrect.h = BOARD_SIZE;
	boardrect.x = 0, boardrect.y = 0;

	SDL_RenderCopy(renderer, boardTex, NULL, &boardrect);

	for (int i = 0; i < PIECE_COUNT; i++) {
		SDL_RenderCopy(renderer, pieces[i].texture, NULL, &pieces[i].rect);
	}
}

void drawText(SDL_Renderer* renderer,
							const int x,
							const int y,
							const char* text,
							TTF_Font** font,
							const SDL_Color* textColor) {
	const SDL_Color defaultColor = { 0, 0, 0 };

	//Draw with the according color
	SDL_Surface* surface =
		TTF_RenderText_Solid((font == NULL) ? defaultFont : *font, text, (textColor == NULL) ? defaultColor : *textColor);

	//Draw to texture
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	//Adjust position
	SDL_Rect textRect;

	SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);

	textRect.x = x - textRect.w / 2, textRect.y = y - textRect.h / 2;

	SDL_RenderCopy(renderer, texture, NULL, &textRect);
}

void normalBoard() {
	//Draw the board

	for (int i = 0; i < PIECE_COUNT; i++) {
		//Now place it on the right positions
		pieces[i].rect.x = (100 * (i % 8)) + 100;
		pieces[i].rect.y = ((i / 8) + (i >= 16) * 4) * 100 + 100;
	}
}
