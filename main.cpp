#include "SDL2/SDL.h"
#include <iostream>


void audioCallback(void* userdata, Uint8 *stream, int len);

Uint8 *audioBuff = nullptr;
Uint8 *audioBuffEnd = nullptr;
Uint32 audioLen = 0;
bool quit = false;
Uint16 pitch = 0;

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
    audioBuff = wavBuff;
    audioBuffEnd = &wavBuff[wavLen];
    audioLen = wavLen;

    if( SDL_OpenAudio(&wavSpec, NULL) < 0)
    {
        fprintf(stderr, "Could not open audio: %s\n", SDL_GetError());
        return 1;
    }


    SDL_PauseAudio(0);
    while(!quit)
    {
        SDL_Delay(100);
    }

    SDL_CloseAudio();
    SDL_FreeWAV(wavBuff);
    return 0;
}


Uint32 sampleIndex = 0;
void audioCallback(void* userdata, Uint8 *stream, int len)
{
    Uint32 length = (Uint32)len;
    length = (length > audioLen ? audioLen : length);

    for(Uint32 i = 0; i < length; i++)
    {
        if(audioBuff > audioBuffEnd)
        {
            quit = true;
            return;
        }
        stream[i] = audioBuff[0];
        audioBuff += 1 + pitch * 2;
        fprintf(stdout, "pitch: %u\n", pitch);
    }
}

