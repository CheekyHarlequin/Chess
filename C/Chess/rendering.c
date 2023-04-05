#include "chess.h"

SDL_Texture* pieceTextures[UNIQUE_PIECE_COUNT];
SDL_Texture *boardTexture, *highlightTexture, *highlightDotTexture;

void render() {
	SDL_RenderCopy(renderer, boardTexture, NULL, &boardRect);

	for (int i = 0; i < PIECE_COUNT; i++) {
		if (!pieces[i].dead) {
			SDL_RenderCopy(renderer, pieceTextures[getPieceTextureIndex(pieces[i].name)], NULL, &pieces[i].rect);
		}
	}

	if (currentlyHeldPiece == NULL) {
		return;
	}

	SDL_Rect highlightRect = { .x = startX - 0, .y = startY - 0, .w = PIECE_SIZE, .h = PIECE_SIZE };

	SDL_RenderCopy(renderer, highlightTexture, NULL, &highlightRect);

	for (int i = 0; i < 64; i++) {
		highlightRect.x = (i % 8) * PIECE_SIZE + BOARD_X_OFFSET;
		highlightRect.y = floor((i - (i % 8)) / 8) * PIECE_SIZE + BOARD_Y_OFFSET;

		if (isMoveValid(startX, startY, highlightRect.x, highlightRect.y, currentlyHeldPiece->name)) {
			//????
			highlightRect.x -= 4 * WINDOW_SIZE / 1000;
			highlightRect.y -= 4 * WINDOW_SIZE / 1000;

			SDL_RenderCopy(renderer, highlightDotTexture, NULL, &highlightRect);
		}
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

SDL_Texture* loadTexture(char* path) {
	SDL_Texture* texPtr;

	SDL_Surface* surface = IMG_Load(path);

	texPtr = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	return texPtr;
}

void loadTextures() {
	boardTexture = loadTexture("textures/board.png");

	highlightTexture = loadTexture("textures/highlight.png");
	highlightDotTexture = loadTexture("textures/highlight-dot.png");

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

		//Texture time
		pieceTextures[getPieceTextureIndex(pieceID)] = loadTexture(path);
	}
}
