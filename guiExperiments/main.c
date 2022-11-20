#include <stdio.h>
#include <SDL2/SDL.h>
SDL_Window* rootWindow;
SDL_Renderer* rootWindowRenderer;
int main()
{
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO  ) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s \n", SDL_GetError());
        abort();
    }

    rootWindow = SDL_CreateWindow(
       "experiments",
       700,0,700, 700,
//       0, 0,
       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    rootWindowRenderer = SDL_CreateRenderer(rootWindow, -1,  0);
    SDL_Event* e;
ссссссс
    GuiButton = {};


    while(    SDL_WaitEvent(e)) {
        if(e->type == SDL_QUIT) {
            break;
        }
    }




    return 0;
}
