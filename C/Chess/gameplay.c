#include "chess.h"

SDL_Rect boardRect;

uint16_t pawnBits;
struct Piece pieces[PIECE_COUNT];

int currentlyHeldPiece = -1;

void gameplay() {
	SDL_Color col = { 0, 0, 255 };

	render();

	drawText(renderer, 500, 400, "First player will start", NULL, &col);
	drawText(renderer, 500, 500, "Please select a piece to move", NULL, &col);

	SDL_RenderPresent(renderer);

	for (bool whois = 0; !getWinner(); whois = !whois) {
		handleInput(whois);
	}
}

int getRoundedPosition(int x) {
	return floor(x / PIECE_SIZE) * PIECE_SIZE;
}

void handleInput(bool whois) {
	//Piece movement handling
	int startX, startY, endX, endY;

	for (SDL_Event event; SDL_PollEvent(&event);) {
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button != SDL_BUTTON_LEFT) {
					continue;
				}

				/*
				The position got devided and multiplied by 100 to round them down to the
				downer 100, so we can use the exact pos for the movement of the pieces
				*/
				for (int i = 0; i < PIECE_COUNT; i++) {
					//Compare rounded coordinates
					if (getRoundedPosition(event.button.x) == pieces[i].rect.x &&
							getRoundedPosition(event.button.y) == pieces[i].rect.y) {

						startX = event.button.y;
						startY = event.button.x;

						currentlyHeldPiece = i;

						break;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (1 /*moveValidCheck*/) {
						pieces[currentlyHeldPiece].rect.x = getRoundedPosition(event.motion.x);
						pieces[currentlyHeldPiece].rect.y = getRoundedPosition(event.motion.y);
						currentlyHeldPiece = -1;
					}
				}
				break;
			case SDL_MOUSEMOTION:
				if (currentlyHeldPiece != -1) {
					pieces[currentlyHeldPiece].rect.x = event.motion.x - PIECE_SIZE / 2;
					pieces[currentlyHeldPiece].rect.y = event.motion.y - PIECE_SIZE / 2;
				}
				break;
			case SDL_QUIT:
				terminate();
				break;
		}
	}

	render();
	SDL_RenderPresent(renderer);
}

void isMoveValid(int startX, int startY, int endX, int endY, char* piece) {
	/*
	Bauer:
	posto - posnow = quotient
	if relatively positive and eq 2 check if going to x or x2
	if diagonal check for a piece, if to front same but !
	we need to store the last move made for highlighting it or checking for en passant
	if last move diff is on the coord of jump + type is pawn take it
	if arrived at either x11 or x12 show ui for piece picking
	overwrite piece texture?
	*/
	//for pawns: (pawnBitfield >> n) & 1U
}

void initBoard() {
	boardRect.w = BOARD_SIZE, boardRect.h = BOARD_SIZE;
	boardRect.x = 0, boardRect.y = 0;

	//The board's initial setup
	const char defaultBoard[PIECE_COUNT] = {
		'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
		'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R',
	};

	for (int i = 0; i < PIECE_COUNT; i++) {
		//Name the pieces accordingly
		pieces[i].name[0] = (i < 16) ? 'b' : 'w';
		pieces[i].name[1] = defaultBoard[i];

		//Position them
		pieces[i].rect.x = (100 * (i % 8)) + 100;
		pieces[i].rect.y = ((i / 8) + (i >= 16) * 4) * 100 + 100;

		//Set the sizes
		pieces[i].rect.w = PIECE_SIZE, pieces[i].rect.h = PIECE_SIZE;
	}
}

//0 for none, 1 for white 2 for black
int getWinner() {
	return (pieces[4].name[1] != 'K') + (pieces[27].name[1] != 'K') * 2;
}
