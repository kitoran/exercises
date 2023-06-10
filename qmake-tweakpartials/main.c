

#include <stdio.h>
#include <sndfile.h>

#include <SDL.h>
#include <SDL_main.h>
int main(int argc, char* argv[]) {
    SF_INFO SN;
    SDL_memset(&SN, 0, sizeof(SN));
    SNDFILE* fl = sf_open    ("franz-liszt-dreams-of-love-1-8499.mp3", SFM_READ, &SN);

    SF_INFO SNPR = SN;
    SNDFILE* pr = sf_open    ("proccessed.mp3", SFM_WRITE, &SNPR);
    for(int i = 0; i < SN.frames; i++) {
        int d;
        sf_read_int(fl, &d, 1);
        sf_write_int(pr, &d, 1);
    }
    sf_close(fl);
    sf_close(pr);
    return 0;
}
