#include "chess.h"

SDL_Rect boardRect;

uint16_t pawnBits;
struct Piece pieces[PIECE_COUNT];

struct Piece* currentlyHeldPiece = NULL;

void gameplay() {
	SDL_Color col = { 0, 0, 255 };

	render();

	drawText(renderer, 500, 400, "First player will start", NULL, &col);
	drawText(renderer, 500, 500, "Please select a piece to move", NULL, &col);

	SDL_RenderPresent(renderer);

	for (bool whois = 0; !getWinner();) {
		handleInput(&whois);
		SDL_Delay(10);
	}
}

inline const int getRoundedPosition(int x) {
	return floor(x / PIECE_SIZE) * PIECE_SIZE;
}

struct Piece* getPieceOnPos(int x, int y) {
	for (int i = 0; i < PIECE_COUNT; i++) {
		if (!pieces[i].dead && getRoundedPosition(x) == pieces[i].rect.x && getRoundedPosition(y) == pieces[i].rect.y) {
			return &pieces[i];
		}
	}

	return NULL;
}

int startX = 0, startY = 0, endX = 0, endY = 0;
void handleInput(bool* whois) {
	//Piece movement handling
	for (SDL_Event event; SDL_PollEvent(&event);) {
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button != SDL_BUTTON_LEFT) {
					continue;
				}

				currentlyHeldPiece = getPieceOnPos(event.button.x, event.button.y);

				if (currentlyHeldPiece == NULL || (currentlyHeldPiece->name[0] == 'b') != *whois) {
					currentlyHeldPiece = NULL;

					return;
				}

				startX = currentlyHeldPiece->rect.x;
				startY = currentlyHeldPiece->rect.y;

				currentlyHeldPiece->rect.x = event.motion.x - PIECE_SIZE / 2;
				currentlyHeldPiece->rect.y = event.motion.y - PIECE_SIZE / 2;

				render();
				SDL_RenderPresent(renderer);

				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT && currentlyHeldPiece != NULL) {
					endX = getRoundedPosition(event.button.x);
					endY = getRoundedPosition(event.button.y);

					if (isMoveValid(startX, startY, endX, endY, currentlyHeldPiece->name)) {
						*whois = !*whois;

						struct Piece* nomPiece = getPieceOnPos(endX, endY);

						if (nomPiece != NULL) {
							nomPiece->dead = true;
						}
					} else {
						endX = startX;
						endY = startY;
					}

					currentlyHeldPiece->rect.x = endX;
					currentlyHeldPiece->rect.y = endY;

					currentlyHeldPiece = NULL;

					render();
					SDL_RenderPresent(renderer);
				}
				break;
			case SDL_MOUSEMOTION:
				if (currentlyHeldPiece != NULL) {
					currentlyHeldPiece->rect.x = event.motion.x - PIECE_SIZE / 2;
					currentlyHeldPiece->rect.y = event.motion.y - PIECE_SIZE / 2;

					render();
					SDL_RenderPresent(renderer);
				}
				break;
			case SDL_QUIT:
				terminate();
				break;
		}
	}
}

bool isMoveValid(int startX, int startY, int endX, int endY, char* piece) {
	struct Piece* endPiecePtr = getPieceOnPos(endX, endY);

	if (endPiecePtr != NULL && endPiecePtr->name[0] == piece[0]) {
		return false;
	}

	if (startX == endX && startY == endY) {
		return false;
	}

	if (endX < BOARD_X_OFFSET || endY < BOARD_Y_OFFSET || endX > WINDOW_SIZE - BOARD_X_OFFSET * 2 ||
			endY > WINDOW_SIZE - BOARD_Y_OFFSET * 2) {
		return false;
	}

	switch (piece[1]) {
		case 'N':
			return (pow((startX - endX), 2) + pow((startY - endY), 2) == 5 * PIECE_SIZE * PIECE_SIZE);
			break;
		case 'B':
			if (abs(endX - startX) == abs(endY - startY)) {
				int tempX = startX, tempY = startY;
				int diffX = endX - tempX, diffY = endY - tempY;

				for (; tempX != endX; tempX += diffX / abs(diffX) * PIECE_SIZE, tempY += diffY / abs(diffY) * PIECE_SIZE) {
					if (getPieceOnPos(tempX, tempY) != NULL) {
						return false;
					}
				}
				return true;
			} else {
				return false;
			}
			break;
		case 'R':
			if ((endX != startX && endY == startY) || (endX == startX && endY != startY)) {
				int tempX = startX, tempY = startY;
				int diffX = endX - tempX, diffY = endY - tempY;
				int piecesPierced = 0;

				for (; tempX != endX || tempY != endY;) {
					if (diffX == 0) {
						tempY += diffY / abs(diffY) * PIECE_SIZE;
					} else {
						tempX += diffX / abs(diffX) * PIECE_SIZE;
					}

					struct Piece* piecePtr = getPieceOnPos(tempX, tempY);

					if (piecesPierced > 0) {
						return false;
					}

					if (piecePtr != NULL) {
						piecesPierced += (piecePtr->name[0] != piece[0]) ? 1 : 2;
					}
				}
				return true;
			} else {
				return false;
			}
			break;
		case 'Q':
			char pieceID[2];
			pieceID[0] = piece[0];

			pieceID[1] = 'B';
			bool bishop = isMoveValid(startX, startY, endX, endY, pieceID);

			pieceID[1] = 'R';
			bool rook = isMoveValid(startX, startY, endX, endY, pieceID);

			return bishop || rook;
			break;
		case 'K':
			return (abs(startX - endX) <= PIECE_SIZE && abs(startY - endY) <= PIECE_SIZE);
			break;
		case 'P':
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
			return true;
			break;
		default:
			return true;
			break;
	}
}

void initBoard() {
	boardRect.w = BOARD_SIZE, boardRect.h = BOARD_SIZE;
	boardRect.x = 0, boardRect.y = 0;

	//The board's initial setup
	const char defaultBoard[PIECE_COUNT] = {
		'R', 'N', 'B', 'K', 'Q', 'B', 'N', 'R', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
		'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
	};

	for (int i = 0; i < PIECE_COUNT; i++) {
		//Name the pieces accordingly
		pieces[i].name[0] = (i < 16) ? 'b' : 'w';
		pieces[i].name[1] = defaultBoard[i];

		//Position them
		pieces[i].rect.x = (100 * (i % 8)) + BOARD_X_OFFSET;
		pieces[i].rect.y = ((i / 8) + (i >= 16) * 4) * 100 + BOARD_Y_OFFSET;

		//Set the sizes
		pieces[i].rect.w = PIECE_SIZE, pieces[i].rect.h = PIECE_SIZE;
	}
}

//0 for none, 1 for white 2 for black
int getWinner() {
	return (pieces[3].dead) + (pieces[28].dead) * 2;
}
