#include "config.h"
#include "combobox.h"
#include <gui.h>
//struct editorInstance
//{
//    MediaItem_Take take;
//    editorInstance();
//};

#include <persistent.h>
#include <gridlayout.h>
#include "settings.h"
#include "actions.h"
#include <stdbool.h>
#include <stdio.h>
#include "stb_ds.h"
#include "playback.h"
#include "actions.h"
#include "extmath.h"

#include "misc.h"
#include "midi.h"
#include <SDL.h>

#include "newFile.h"
#include "mainprogram.h"




#include <gui.h>

SDL_Renderer* renderer;

char* appName = "piano roll continous";
#ifdef _WIN32
#define FILE_DIALOG_PATH "C:\src\exercises\build-FileDialog-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug\debug\FileDialog.exe"
#else
#define FILE_DIALOG_PATH "/home/n/exercises/build-FileDialog-Desktop-Debug/FileDialog"
#endif
SDL_mutex* mutex_; // I use SDL_mutex because MSVC doesn't provide thread.h
SDL_cond* condVar;
extern void SetThreadName(u32 dwThreadID, const char* threadName);
extern int PlaybackEvent = 0;
extern int intervalTrainerGui(void) {
//    running = true;
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" );
//    STATIC(bool, inited, guiInit());
    guiStartDrawingEx(false);
    mutex_ = SDL_CreateMutex();

    condVar = SDL_CreateCond();

    SetThreadName(-1, "midicont");
    openAudio();
    Grid grid = allocateGrid(100, 100, 5);
    grid.gridStart.x = grid.gridStart.y = 0;
    pushLayout(&grid);

    SDL_LockMutex(mutex_);


    PlaybackEvent = SDL_RegisterEvents(1);


    {
        bool succ = true;
//        Rect windowRect={0};
        LOAD_INT(config.windowGeometry.x, &succ);
        LOAD_INT(config.windowGeometry.y, &succ);
        LOAD_INT(config.windowGeometry.w, &succ);
        LOAD_INT(config.windowGeometry.h, &succ);
        if(!succ) {
            config.windowGeometry = (Rect){ 400, 400, 700, 700 };
        }
        guiMoveWindow(rootWindow, config.windowGeometry.x, config.windowGeometry.y);
        SDL_SetWindowSize(rootWindow, config.windowGeometry.w, config.windowGeometry.h);
        SDL_ShowWindow(rootWindow);
        SDL_RaiseWindow(rootWindow);

    }

    while(1) {
        SDL_UnlockMutex(mutex_);
        guiNextEvent(/*.dontblock = *//*playing*/); // let's hope that i don't add any
        // support for hooks that are to be run inside guiNextEvent
        // and potentially have access to things that should
        // be protected with mutex_
        SDL_LockMutex(mutex_);

//        if(playing) {
//            static int frames = 0;
//            STATIC(int, time, SDL_GetTicks());
//            int newTime = SDL_GetTicks();
//            frames++;
//            currentPositionInSamples += 44100*(newTime-time)/1000.0; //TODO: 44100
//            if((newTime/1000)*1000 > time) {
//                message("%d frames\n", frames);
//                frames = 0;
//            }
//            time = newTime;
//        }

        guiSetForeground(&rootWindowPainter,0);
        guiClearWindow(rootWindow);
        setCurrentGridPos(0,0);
        int keyPressed = -1;
        if(event.type==SDL_WINDOWEVENT) {
            if(event.window.event == SDL_WINDOWEVENT_CLOSE) {
                message("closing the window"
                        "\n");
//                arrsetlen(piece, 0);
                break;
            }
            if(event.window.event == SDL_WINDOWEVENT_MOVED) {
                config.windowGeometry.x = event.window.data1;
                config.windowGeometry.y = event.window.data2;
            }
            if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                config.windowGeometry.w = event.window.data1;
                config.windowGeometry.h = event.window.data2;
            }
        }
        static double intervalInCents;
        static char label[30] = {0};
        if(guiButtonZT(&rootWindowPainter, "play an interval")) {
            //60 - 4000
            double rangeInCents = log(4000.0/60)/log(2)*12*100;
            intervalInCents = (double)rand()/(double)(RAND_MAX/500.0);
            double first = (double)rand()/(double)(RAND_MAX/(rangeInCents-intervalInCents));
            double second = first+intervalInCents;

            playInterval(pow(2, first/1200)*60, pow(2, second/1200)*60);
//            SDL_PauseAudioDevice(audioDevice, 0);
            label[0]=0;
        } gridNextColumn();
        if(guiButtonZT(&rootWindowPainter, "reveal")) {
//            SDL_PauseAudioDevice(audioDevice, 1);
//            stop();
            snprintf(label, 30, "%lf", intervalInCents);
        } gridNextColumn();
        guiLabelZT(&rootWindowPainter, label);

        if(event.type == SDL_QUIT) {
            break;
        }
    }
    message("broke out of loop"
            "\n");

    popLayout();
    SAVE_INT(config.windowGeometry.x);
    SAVE_INT(config.windowGeometry.y);
    SAVE_INT(config.windowGeometry.w);
    SAVE_INT(config.windowGeometry.h);
    SDL_FlushEvents(0, SDL_LASTEVENT);
//    SDL_HideWindow(rootWindow);
//    running = false;

    return 0;
}
//*/
