#include "chess.h"



void gameplay() {
	SDL_Color col = { 0, 0, 255 };

	renderBoard();

	drawText(renderer, 500, 400, "First player will start", NULL, &col);

	// Ask user for input
	drawText(renderer, 500, 500, "Please select a piece to move", NULL, &col);
	for (bool whois = 0; !winner(); whois = !whois) {
		renderBoard();
    SDL_RenderPresent(renderer);
		getPieceMovement(whois);
	}
	SDL_RenderPresent(renderer);
}

//TODO: ask for movement and then move, also do a check for a valid move
void getPieceMovement(bool whois) {

	// Which piece would you move
	int piecexstart = 0;
	int pieceystart = 0;
	//to wich point you want o move this piece
	int piecexend = 0;
	int pieceyend = 0;
	/*
	The position got devided and multiplied by 100 to round them down to the
	downer 100, so we can use the exact pos for the movement of the pieces
	*/
	SDL_Event event;
	while (!SDL_PollEvent(&event)) {
		switch (event.type) {
			// Handle mouse button press
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					piecexstart = 600/*floor(event.button.x / 100) * 100*/;
					pieceystart = 700 /*floor(event.button.y / 100) * 100*/;
					break;
				}
		}
		while (!SDL_PollEvent(&event)) {
			switch (event.type) {
				// Handle mouse button press
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						piecexend = 400/*floor(event.button.x / 100) * 100*/;
						pieceyend = 400/*floor(event.button.y / 100) * 100*/;
						break;
					}
			}
			moveValidCheck(piecexstart, pieceystart, piecexend, pieceyend, whois);
		}
	}
}
void moveValidCheck(int piecexstart, int pieceystart, int piecexend, int pieceyend, bool whois) {
	/*
	First check, which piece is on the pointet position
	*/
	int i = 0;
	for (; i < PIECE_COUNT; i++) {
		if (pieces[i].rect.x == piecexstart && pieces[i].rect.y == pieceystart);
		{
			break;
		}
		piecexstart /= 100, pieceystart /= 100, piecexend /= 100, pieceyend /= 100;
	}
	pieces[i].rect.x = (piecexend * 100);
	pieces[i].rect.y = (pieceyend * 100);

	/*
	switch (pieces[i].name[1]) {
		case 'N': {
			if (pow((piecexstart - piecexend), 2) + pow((pieceystart - pieceyend), 2) == 5) {
				pieces[i].rect.x = piecexend * 100;
				pieces[i].rect.y = pieceyend * 100;
			}
			break;
		}
		case 'P':

			break;
	}
	*/
}

/*
Programm should check if there is a white or black King.
If there is no black King, white is the winner and the programm returns 1
otherwise the programm returns 2
if there are both kings alive, the programm returns 0

wKing on 5th position
bKing on 28th position
*/
int winner() {
	return (pieces[4].name[1] != 'K') + (pieces[27].name[1] != 'K') * 2;
}

//NormalBoard sets Board to default including Pieces
