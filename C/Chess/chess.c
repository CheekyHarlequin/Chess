#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define PIECES 100;
#define BOARD 1000;

//global things like renderer
Uint32 render_flags;
SDL_Renderer* rend;
SDL_Surface* surface;


char srcofpieces[32][24]=
                      {
                      "textures/bR.svg", "textures/bN.svg", "textures/bB.svg", "textures/bQ.svg", "textures/bK.svg", "textures/bB.svg", "textures/bN.svg", "textures/bR.svg",
                      "textures/bP.svg", "textures/bP.svg", "textures/bP.svg", "textures/bP.svg", "textures/bP.svg", "textures/bP.svg", "textures/bP.svg", "textures/bP.svg",
                      "textures/wP.svg", "textures/wP.svg", "textures/wP.svg", "textures/wP.svg", "textures/wP.svg", "textures/wP.svg", "textures/wP.svg", "textures/wP.svg",
                      "textures/wR.svg", "textures/wN.svg", "textures/wB.svg", "textures/wQ.svg", "textures/wK.svg", "textures/wB.svg", "textures/wN.svg", "textures/wR.svg"
                        };


/*
  backupPieces[32][2]= {
                      "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR",
                      "bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP",
                      "wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP",
                      "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR"
                        };
*/
void play();
void normalBoard();

int main()
{
  if(SDL_Init(SDL_INIT_VIDEO))
  {
    printf("error initializing SDL: %s\n", SDL_GetError());
    return 0;
  }
  SDL_Window* win =SDL_CreateWindow("Test",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    1000, 1000, 0);
                                    render_flags = SDL_RENDERER_ACCELERATED;
  render_flags = SDL_RENDERER_ACCELERATED;
  rend = SDL_CreateRenderer(win, -1, render_flags);


    play();
    while(1);


  return 0;
}

void play()
  {
    normalBoard();


 }

void normalBoard()
{
  // making the Board
  SDL_Rect boardrect;
  surface = IMG_Load("textures/Chess.jpg");
  SDL_Texture* boardTex = SDL_CreateTextureFromSurface(rend, surface);
  SDL_QueryTexture(boardTex, NULL, NULL, &boardrect.w, &boardrect.h);

  //resize and change pos of the board rect.
  boardrect.w = BOARD;
  boardrect.h = BOARD;
  boardrect.x = 0;
  boardrect.y = 0;
  SDL_FreeSurface(surface);
  SDL_RenderCopy(rend, boardTex, NULL, &boardrect);

  //puting pieces on it

  SDL_Rect piecerect;
  SDL_Texture* pieceTex;

  char counter = 1;

  for(int i=0; i<32;i++)
  {
    surface = IMG_Load(srcofpieces[i]);
    pieceTex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_QueryTexture(pieceTex, NULL, NULL, &piecerect.w, &piecerect.h);
    piecerect.w = PIECES;
    piecerect.h = PIECES;
    if(i<15)
    {
      if(i<8)
      {
        piecerect.x = (100*counter);
        piecerect.y = 100;
        counter++;
      }
      else
      {
        counter = 1;
        piecerect.x = (100*counter);
        piecerect.y = 200;
      }

      SDL_FreeSurface(surface);
      SDL_RenderCopy(rend, pieceTex, NULL, &piecerect);
    }
  }

  SDL_RenderPresent(rend);
  SDL_DestroyTexture(boardTex);
}
