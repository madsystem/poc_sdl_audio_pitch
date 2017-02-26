#include "SDL2/SDL.h"
#include <iostream>


void audioCallback(void* userdata, Uint8 *stream, int len);

Uint8 *audioBuff = nullptr;
Uint8 *audioBuffEnd = nullptr;
bool quit = false;
char pitch = 1;

int main()
{
    if(SDL_Init(SDL_INIT_AUDIO) < 0)
        return -1;
    
    Uint32 wavLen = 0;
    Uint8 *wavBuff = nullptr;
    SDL_AudioSpec wavSpec;

    if(SDL_LoadWAV("test.wav", &wavSpec, &wavBuff, &wavLen) == nullptr)
    {
        return 1;
    } 
    wavSpec.callback = audioCallback;
    wavSpec.userdata = nullptr;
    wavSpec.format = AUDIO_S16;
    wavSpec.samples = 2048;
    audioBuff = wavBuff;
    audioBuffEnd = &wavBuff[wavLen];

    if( SDL_OpenAudio(&wavSpec, NULL) < 0)
    {
        fprintf(stderr, "Could not open audio: %s\n", SDL_GetError());
        return 1;
    }


    fprintf(stderr, "fmt  %u: ", wavSpec.format);

    SDL_PauseAudio(0);
    while(!quit)
    {
        SDL_Delay(500);
        //pitch ++;
        
    }

    SDL_CloseAudio();
    SDL_FreeWAV(wavBuff);
    return 0;
}


void audioCallback(void* userdata, Uint8 *stream, int len)
{
    Uint32 length = (Uint32)len;
    for(Uint32 i = 0; i < length; i++)
    {
        if(audioBuff > audioBuffEnd)
        {
            quit = true;
            return;
        }
        
        stream[i] = audioBuff[0];
        stream[++i] = audioBuff[1];
        audioBuff += 2 * pitch;
    }
}

