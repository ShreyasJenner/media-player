#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    SDL_AudioStream *stream;
    SDL_AudioSpec desired, obtained;
    
    Sint16 bytes[2048];
    int fd, err, id, fd2;
    char *device_name;

    // init sdl 
    err = SDL_Init(SDL_INIT_AUDIO);
    if(err<0) {
        printf("Failed to init SDL\n");
    }

    // set up audio device
    err = SDL_GetAudioDeviceSpec(4, 0, &desired);

    device_name = (char *)SDL_GetAudioDeviceName(4, 0);
    id = SDL_OpenAudioDevice(device_name, 0, &desired, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if(id == 0) {
        printf("open audio device error\n");
        exit(1);
    }


    // create a new stream
    stream = SDL_NewAudioStream(AUDIO_S16MSB, 2, 44100, AUDIO_U16LSB, 2, 44100);
   
    if(stream == NULL) {
        printf("Stream creation failed\n");
        exit(1);
    }


    // open file 
    fd = open(argv[1], O_RDONLY, 0);
    fd2 = open(argv[2], O_WRONLY|O_CREAT, 0666);

    if(fd < 0) {
        printf("fd1 error\n");
        exit(1);
    }

    if(fd2 < 0) {
        printf("fd2 error\n");
        exit(1);
    }

    while(read(fd, bytes, 2048)!=0) {
        if(SDL_AudioStreamPut(stream, bytes, 2048)!=0) {
            printf("Stream put error\n");
            exit(1);
        }

        err = SDL_AudioStreamAvailable(stream);
        if(err>=2048) {
            SDL_AudioStreamGet(stream, bytes, err);
            err = write(fd2, bytes, err);
            printf("Wrote:%d\n", err);
        }
    }


    SDL_CloseAudioDevice(id);
    SDL_Quit();
    close(fd);
    close(fd2);

    return 0;
}
