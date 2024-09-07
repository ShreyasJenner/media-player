/*
 * The program and all sub-programs assume little endianess in any read data
 * MAKE PROGRAM ENDIAN INDEPENDENT
 */

#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdbool.h>

#include "../include/structs.h"
#include "../include/mp3_lut.h"


/*
 * Function swaps the value of the two integer pointers passed to it
 */
void swap(uint8_t *a, uint8_t *b) {
    uint8_t tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* 
 * Function changes order of bytes in passed array pointer 
 * into big endian order
 */
void to_big_endian(uint8_t *arr, int n) {
    int i;

    for(i=0;i<n/2;i++) {
        swap(&arr[i], &arr[n-1-i]);
    }
}


/*
 * Prints passed 8bit integer to it in bit representation
 */
void bit_representation(uint8_t byte) {
    int i;

    for(i=7;i>=0;i--) {
        printf("%u", (byte&128u)>>7);
        byte <<= 1;
    }
}

/*
 * Reverses the bit order of the passed 8bit integer
 */
int reverse_bit_order(int byte) {
    return (byte * 0x0202020202ULL & 0x010884422010ULL) % 1023;
}


int main(int argc, char **argv) {
    int fd, bytes, i, tmp, flag, pos;
    uint8_t frame[32], byte;
    struct mp3_frame_header_data mfhd;

    fd = open(argv[1], O_RDONLY, 0);
    if(fd < 0) {
        printf("opening file error\n");
        exit(1);
    }

    // loop to read file till mp3 magic byte location is found
    // mp3 magic bytes: ff fb OR ff f3 OR ff f2
    tmp = 1;
    flag = 0;
    pos = 0;
    while(tmp) {
        tmp = read(fd, &byte, 1);
        pos++;

        if(byte == 0xff)
            flag++;
        else if(flag==1 && ( (byte==0xfb) || (byte==0xf3) || (byte==0xf2)))
            break;
        else
            flag = 0;
    }

    // change this line to use id3reader to get tag size 
    lseek(fd, pos-2, SEEK_SET);
    read(fd, frame, 4);

    /* temp code */
    int itr = 0;
    int fd2 = open(argv[2], O_WRONLY|O_CREAT, 0666);
    if(fd2<0) {
        printf("fd2 error\n");
        exit(1);
    }
    while(itr++<2000) {
    for(i=0;i<4;i++) {
        bit_representation(frame[i]);
        printf(":%02x\n",frame[i]);
    }

    // print mp3 frame details and store in struct
    show_mp3FrameHeader(frame);
    get_mp3FrameHeader(frame, &mfhd);


    /* temp code */
    lseek(fd, -4, SEEK_CUR);
    
    uint8_t temp_buff[mfhd.frame_length];
    read(fd, temp_buff, mfhd.frame_length);
    write(fd2, temp_buff, mfhd.frame_length);
    read(fd, frame, 4);
    printf("%02x%02x%02x%02x\n", frame[0], frame[1], frame[2], frame[3]);
    }
    close(fd2); 
    /* temp code */

    close(fd);

    return 0;
}
