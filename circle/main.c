
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

bool continueCycle = true;

int filter(void *a, SDL_Event *event) {
    return event->type == SDL_QUIT || event->type == SDL_WINDOWEVENT;
}

void processKeyboard()
{
    const Uint8 *state = SDL_GetKeyboardState(0);
    if(state[SDL_SCANCODE_ESCAPE] || (state[SDL_SCANCODE_F4] && (state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT]))) {
        continueCycle = false;
    }
}

void renderScene() {

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main()
{

    SDL_Window* window;
    SDL_GLContext context;
    SDL_Init(SDL_INIT_VIDEO);

    SDL_DisplayMode display;
    SDL_GetCurrentDisplayMode(0, &display);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    int width = 500, height = 500;
    window = SDL_CreateWindow("Hyperbolic",  50, 50, width, height, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);

    SDL_ShowWindow(window);


    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_SetEventFilter(filter,
                       0);



    while(continueCycle) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                continueCycle = false;
            }
            if(event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_SHOWN: {
                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    SDL_GL_SwapWindow(window);
                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    SDL_GL_SwapWindow(window);
                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    glViewport(0, 0, width, height);
                } break;
                case SDL_WINDOWEVENT_RESIZED: {
                    width = event.window.data1;
                    height = event.window.data2;

                    glViewport(0, 0, width, height);
                } break;
                }
            }
        }
        processKeyboard();
        renderScene();
        SDL_GL_SwapWindow(window);
    }
    printf("Hello World!\n");
    return 0;
}
