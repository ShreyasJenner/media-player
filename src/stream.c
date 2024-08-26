#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>

#include "../include/decode_mp3.h"

#define QUEUE_CHUNK 4096 

int main(int argc, char **argv) {
    int fd, err, play_time, dev_id;
    uint8_t data[QUEUE_CHUNK];
    SDL_AudioDeviceID id;
    SDL_AudioSpec desired, obtained;
    
    // initalize SDL with audio syb system
    if(SDL_Init(SDL_INIT_AUDIO)<0) {
        printf("Failed to init SDL\n");
        exit(1);
    }

    // get the audio driver name
    printf("Getting audio driver name: %s\n", SDL_GetCurrentAudioDriver());

    // get number of playback devices
    printf("Getting audio device count: %d\n",SDL_GetNumAudioDevices(0));

    // print current audio devices
    printf("Getting audio device list:\n");
    for(int i=0; i<SDL_GetNumAudioDevices(0); i++) {
        printf("\t%d:%s\n",i,SDL_GetAudioDeviceName(i, 0));
    }

    // get output device choice
    printf("Enter playback device choice[%d-%d]:",0,SDL_GetNumAudioDevices(0)-1);
    scanf("%d",&dev_id);

    // store preferred audio device specs into struct
    SDL_GetAudioDeviceSpec(dev_id, 0, &desired);

    // Open selected device using audio struct as format
    id = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(dev_id, 0), 0, &desired, &obtained, 0);

    // print error if exists
    if(id==0) {
        printf("%s\n",SDL_GetError());
        exit(1);
    }

    // unpause audio device
    SDL_PauseAudioDevice(id, 0);

    // convert mp3 to pcm and store in file
    decode_to_pcm(argv[1], argv[2]);

    fd = open(argv[2], O_RDONLY, 0);
       
    if(fd<0) {
        printf("File opening error\n");
        exit(1);
    }
    
    while(read(fd, data, QUEUE_CHUNK)!=0)
        // queue audio data into the audio device
        SDL_QueueAudio(id, data, sizeof(data));


    // play for certain number of seconds
    play_time = 0;
    while(play_time++<5) {
        printf("Queued Data Size:%d\n", SDL_GetQueuedAudioSize(id));
        SDL_Delay(1000);
    } 


    // close and clean up resources 
    SDL_CloseAudioDevice(id);
    SDL_Quit();
    close(fd);
    
    return 0;
}
