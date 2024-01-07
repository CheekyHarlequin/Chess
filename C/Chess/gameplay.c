#include "chess.h"

SDL_Rect boardRect, pPBackground;
SDL_Rect promotePiecesRect[4];

uint16_t pawnBits;
struct Piece pieces[PIECE_COUNT] = { 0 };

struct Piece *currentlyHeldPiece = NULL, lastPiece;
int lastPawnDiff = 0;

bool threatahead = false;
bool pawnPromoting = false;
bool whois;
bool isButtondown = false;

void gameplay() {
	SDL_Color col = { 0, 0, 255 };

	render();

	drawText(renderer, 5 * PIECE_SIZE, 4 * PIECE_SIZE, "First player will start", NULL, &col);
	drawText(renderer, 5 * PIECE_SIZE, 5 * PIECE_SIZE, "Please select a piece to move", NULL, &col);

	SDL_RenderPresent(renderer);

	// whois 0 for white; 1 for black
	for (whois = 0; !getWinner();) {
		handleInput();
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
void handleInput() {
	//Piece movement handling
	for (SDL_Event event; SDL_PollEvent(&event);) {
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button != SDL_BUTTON_LEFT) {
					continue;
				}
				if (pawnPromoting) {
					if (getRoundedPosition(event.button.y) == whois ? BOARD_Y_OFFSET : BOARD_Y_OFFSET * 8) {
						/*
						X-Positions for Pawn Promotions:
						Rook BOARD_X_OFFSET*3
						Knight BOARD_X_OFFSET*4
						Bishop BOARD_X_OFFSET*5
						Queen BOARD_X_OFFSET*6
						*/
						if (getRoundedPosition(event.button.x) >= (BOARD_X_OFFSET * 3) &&
								getRoundedPosition(event.button.x) < (BOARD_X_OFFSET * 7)) {
							switch (getRoundedPosition(event.button.x)) {
								case BOARD_X_OFFSET * 3:
									promotePawn('R');
									break;
								case BOARD_X_OFFSET * 4:
									promotePawn('N');
									break;
								case BOARD_X_OFFSET * 5:
									promotePawn('B');
									break;
								case BOARD_X_OFFSET * 6:
									promotePawn('Q');
									break;
							}
						} else {
							continue;
						}
					} else {
						continue;
					}

					pawnPromoting = false;

					render();
					SDL_RenderPresent(renderer);

					continue;
				}

				currentlyHeldPiece = getPieceOnPos(event.button.x, event.button.y);

				if (currentlyHeldPiece == NULL || (currentlyHeldPiece->name[0] == 'b') != whois) {
					currentlyHeldPiece = NULL;

					return;
				}
				if (threattest(false) && currentlyHeldPiece != (whois ? &pieces[4] : &pieces[28])) {
					continue;
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
					int result = isMoveValid(startX, startY, endX, endY);

					if (result && !threattest(true)) {

						whois = !whois;
						struct Piece* nomPiece = getPieceOnPos(endX, endY);
						if (nomPiece != NULL) {
							nomPiece->dead = true;
						}

						// Rochade
						currentlyHeldPiece->wasmoved = true;

						// left Rochade
						if (result == 3) {
							getPieceOnPos(PIECE_SIZE, startY)->rect.x = (PIECE_SIZE * 4);
						}
						// right Rochade
						else if (result == 4) {
							getPieceOnPos((PIECE_SIZE * 8), startY)->rect.x = (PIECE_SIZE * 6);
						}

						if (result == 2) {
							if (lastPiece.name[1] == 'P') {

								getPieceOnPos(lastPiece.rect.x, lastPiece.rect.y)->dead = true;
							}
						}

						lastPiece.rect.x = endX;
						lastPiece.rect.y = endY;
						lastPiece.name[1] = currentlyHeldPiece->name[1];
						lastPawnDiff = abs(startY - endY);

						if (lastPiece.name[1] == 'P' && whois ? endY == PIECE_SIZE : endY == (PIECE_SIZE * 8))
							pawnPromoting = true;

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

int isMoveValid(int startX, int startY, int endX, int endY) {
	struct Piece* endPiecePtr = getPieceOnPos(endX, endY);

	if (endPiecePtr != NULL && endPiecePtr->name[0] == currentlyHeldPiece->name[0]) {
		return false;
	}

	if (startX == endX && startY == endY) {
		return false;
	}

	if (endX < BOARD_X_OFFSET || endY < BOARD_Y_OFFSET || endX > WINDOW_SIZE - BOARD_X_OFFSET * 2 ||
			endY > WINDOW_SIZE - BOARD_Y_OFFSET * 2) {
		return false;
	}

	switch (currentlyHeldPiece->name[1]) {
		case 'N':
			return ((pow((startX - endX), 2) + pow((startY - endY), 2) == 5 * PIECE_SIZE * PIECE_SIZE));
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
						piecesPierced += (piecePtr->name[0] != currentlyHeldPiece->name[0]) ? 1 : 2;
					}
				}
				return true;
			} else {
				return false;
			}
			break;

		case 'Q':
			currentlyHeldPiece->name[1] = 'B';
			bool bishop = isMoveValid(startX, startY, endX, endY);

			currentlyHeldPiece->name[1] = 'R';
			bool rook = isMoveValid(startX, startY, endX, endY);

			currentlyHeldPiece->name[1] = 'Q';

			return bishop || rook;
			break;

		case 'K':
			// #TODO
			// Control if endX is on the Rochade pos for King
			//For Rochade:

			if (currentlyHeldPiece->wasmoved == false) {
				if (endX == PIECE_SIZE * 3 && endY == startY && getPieceOnPos(PIECE_SIZE, startY) != NULL &&
						getPieceOnPos(PIECE_SIZE, startY)->wasmoved == false) {

					char countCaseA;
					for (countCaseA = 2; countCaseA < 6; countCaseA++) {
						if ((startX - (PIECE_SIZE * countCaseA)) == PIECE_SIZE) {
							// For Rochade to the left
							return 3;
						}
						if (getPieceOnPos(startX - (PIECE_SIZE * countCaseA), startY) != NULL) {
							break;
						}
					}
				}
				if (endX == PIECE_SIZE * 7 && endY == startY && getPieceOnPos(PIECE_SIZE * 8, startY) != NULL &&
						getPieceOnPos(PIECE_SIZE * 8, startY)->wasmoved == false) {

					char countCaseB;
					for (countCaseB = 2; countCaseB < 6; countCaseB++) {
						if ((startX + (PIECE_SIZE * countCaseB)) == (PIECE_SIZE * 8)) {
							// for Rochade to the right
							return 4;
						}
						if (getPieceOnPos(startX + (PIECE_SIZE * countCaseB), startY) != NULL) {
							break;
						}
					}
				}
			}
			// für Kontrolle: printf("StartX: %d \n StartY: %d \n",startX, startY);

			return (abs(startX - endX) <= PIECE_SIZE && abs(startY - endY) <= PIECE_SIZE);

			break;

		case 'P':
			bool isInRightDirection = (currentlyHeldPiece->name[0] == 'w') ? (startY > endY) : (startY < endY);

			if (!isInRightDirection) {
				return false;
			}

			//For normal movement of Pawn
			if (startX == endX) {
				if (currentlyHeldPiece->name[0] == 'w' && getPieceOnPos(startX, endY) == NULL) {
					return (endY == PIECE_SIZE * 5) ? true : (startY - endY == PIECE_SIZE);
				} else if (currentlyHeldPiece->name[0] == 'b' && getPieceOnPos(startX, endY) == NULL) {
					return (endY == PIECE_SIZE * 4) ? true : (startY - endY == -PIECE_SIZE);
				}
			}

			//For killing pieces
			else if (abs(startY - endY) == PIECE_SIZE && abs(startX - endX) == PIECE_SIZE) {
				int dirX = (startX + PIECE_SIZE == endX) ? (startX + PIECE_SIZE) : (startX - PIECE_SIZE);

				bool isKill = (getPieceOnPos(dirX, endY) != NULL);

				bool isEnPassent = (lastPawnDiff == PIECE_SIZE * 2) && (lastPiece.rect.x == dirX) &&
													 (lastPiece.rect.y == startY) && (lastPiece.name[1] == 'P');
				if (isEnPassent) {
					return 2;
				}
				return isKill;
			}
			return false;
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
		'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
		'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
	};

	for (int i = 0; i < PIECE_COUNT; i++) {
		//Name the pieces accordingly
		pieces[i].name[0] = (i < 16) ? 'b' : 'w';
		pieces[i].name[1] = defaultBoard[i];

		//Position them
		pieces[i].rect.x = (PIECE_SIZE * (i % 8)) + BOARD_X_OFFSET;
		pieces[i].rect.y = ((i / 8) + (i >= 16) * 4) * PIECE_SIZE + BOARD_Y_OFFSET;

		//Set the sizes
		pieces[i].rect.w = PIECE_SIZE, pieces[i].rect.h = PIECE_SIZE;

		//pawnPromoting Background:

		pPBackground.w = PIECE_SIZE * 5;
		pPBackground.h = PIECE_SIZE * 2;
		for (char i = 0; i < 4; i++) {
			promotePiecesRect[i].x = ((PIECE_SIZE * 3) + (PIECE_SIZE * i));
			promotePiecesRect[i].w = PIECE_SIZE;
			promotePiecesRect[i].h = PIECE_SIZE;
		}
	}
}

/*
		#TODO : ?


		Checks if King is in danger, validcheck does nothing more than changing the
		"dead"-status of "currentlyHeldPiece"
*/
bool threattest(bool validcheck) {
	currentlyHeldPiece->dead = validcheck;
	currentlyHeldPiece->name[0] = (whois) ? 'w' : 'b';

	char whichenemy = (whois ? 16 : 0);
	char myKing = (whois ? 4 : 28);
	bool gothit = false;

	for (char i = 0; i < 16; i++) {

		if (!pieces[i + whichenemy].dead &&
				isMoveValid(
					pieces[i + whichenemy].rect.x, pieces[i + whichenemy].rect.y,
					pieces[myKing].rect.x, pieces[myKing].rect.y)) {
			gothit = true;
			break;
		}
	}

	currentlyHeldPiece->dead = false;
	currentlyHeldPiece->name[0] = (whois) ? 'b' : 'w';

	return gothit;
}

void promotePawn(char toWhatPromoteTo) {
	getPieceOnPos(lastPiece.rect.x, lastPiece.rect.y)->name[1] = toWhatPromoteTo;
}
//0 for none, 1 for white 2 for black
int getWinner() {
	return (pieces[4].dead) + (pieces[28].dead) * 2;
}
