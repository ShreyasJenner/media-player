#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_quit.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>

#include "../include/decode_mp3.h"

int main(int argc, char **argv) {
    int fd, err, play_time;
    uint8_t data[500];
    SDL_AudioDeviceID id;
    SDL_AudioSpec desired, obtained;
    char dev_name[50];
    
    // initalize SDL with audio syb system
    if(SDL_Init(SDL_INIT_AUDIO)<0) {
        printf("Failed to init SDL\n");
        exit(1);
    }

    // get the audio driver name
    printf("Audio Driver:%s\n", SDL_GetCurrentAudioDriver());

    // get number of playback devices
    printf("Number of Audio Devices:%d\n",SDL_GetNumAudioDevices(0));

    // print current audio devices
    for(int i=0; i<SDL_GetNumAudioDevices(0); i++) {
        printf("%d:%s\n",i,SDL_GetAudioDeviceName(i, 0));
    }

    strcpy(dev_name, SDL_GetAudioDeviceName(4, 0));

    printf("Device Name:%s\n",dev_name);

    /*
    // load desired struct
    desired.freq = 441000;
    desired.format = AUDIO_S16SYS;
    desired.samples = 1024;
    */

    // fill desired with necessary data
    SDL_GetAudioDeviceSpec(4, 0, &desired);

    printf("freq:%d\n", desired.freq);
    
    // Open a device
    id = SDL_OpenAudioDevice(dev_name, 0, &desired, &obtained, 0);


    // check for error
    printf("Device Id:%d\n", id);
    if(id==0) {
        // print error if exists
        printf("%s\n",SDL_GetError());
        exit(1);
    }

    // unpause audio device
    SDL_PauseAudioDevice(id, 0);

    // convert mp3 to pcm temporarily
    decode_to_pcm(argv[1], argv[2]);

    // open 2nd file
    fd = open(argv[2], O_RDONLY, 0);
        
    if(fd<0) {
        printf("File opening error\n");
        exit(1);
    }
    
    while(read(fd, data, 500)!=0)
        SDL_QueueAudio(id, data, sizeof(data));


    // check if quit is requested
    play_time = 0;
    while(play_time++<5) {
        printf("Queued Data Size:%d\n", SDL_GetQueuedAudioSize(id));
        SDL_Delay(1000);
    } 

    SDL_CloseAudioDevice(id);
    SDL_Quit();
    close(fd);
    
    return 0;
}
