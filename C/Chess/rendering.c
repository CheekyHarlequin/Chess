#include "chess.h"

SDL_Texture* pieceTextures[UNIQUE_PIECE_COUNT];
SDL_Texture* boardTexture;

void render() {
	SDL_RenderCopy(renderer, boardTexture, NULL, &boardRect);

	for (int i = 0; i < PIECE_COUNT; i++) {
		SDL_RenderCopy(renderer, pieceTextures[getPieceTextureIndex(pieces[i].name)], NULL, &pieces[i].rect);
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

void loadTextures() {
	SDL_Surface* surface = IMG_Load("textures/board.png");

	boardTexture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	//Load the textures for the pieces
	const char loadingSeq[] = { 'R', 'N', 'B', 'Q', 'K', 'P' };
	for (int i = 0; i < UNIQUE_PIECE_COUNT; i++) {
		//Create the full path
		char path[32] = { "textures/" };

		//Texture id
		char pieceID[3] = { 0 };
		pieceID[0] = i < 6 ? 'w' : 'b';
		pieceID[1] = loadingSeq[i % (UNIQUE_PIECE_COUNT / 2)];

		strcat(path, pieceID);
		strcat(path, ".png");

		//Load onto surface
		surface = IMG_Load(path);

		//Texture time
		pieceTextures[getPieceTextureIndex(pieceID)] = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_FreeSurface(surface);
	}
}
