#include <stdio.h>
#include <stdlib.h>
#include <mpg123.h>

#include "../include/decode_mp3.h"

#define PCM_BUFFER_SIZE 4096 // Adjust as needed


/*
 * Function: decode_to_pcm
 * -----------------------
 *  Decodes input mp3 file and writes as signed 16bit little-endian pcm 
 *
 *  mp3_file: name of input mp3 file
 *  pcm_file: name of output pcm file
 *
 *  returns:0 => Function successfully converted mp3 to pcm
 *          -1 => Error
 */
int decode_to_pcm(char *mp3_file, char *pcm_file) {

    // Initialize the mpg123 library
    if (mpg123_init() != MPG123_OK) {
        fprintf(stderr, "Failed to initialize mpg123.\n");
        return -1;
    }

    // Create a mpg123 handle
    mpg123_handle *mh = mpg123_new(NULL, NULL);
    if (!mh) {
        fprintf(stderr, "Failed to create mpg123 handle.\n");
        return -1;
    }

    // Open the MP3 file
    if (mpg123_open(mh, mp3_file) != MPG123_OK) {
        fprintf(stderr, "Failed to open input file: %s\n", mp3_file);
        mpg123_delete(mh);
        return -1;
    }

    // Get the audio format of the MP3 file
    int channels, encoding;
    long rate;
    if (mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK) {
        fprintf(stderr, "Failed to get MP3 format.\n");
        mpg123_close(mh);
        mpg123_delete(mh);
        return -1;
    }

    // Ensure the output format is signed 16-bit little-endian PCM
    mpg123_format_none(mh);
    mpg123_format(mh, rate, channels, MPG123_ENC_SIGNED_16);

    // Open the output file for writing
    FILE *output = fopen(pcm_file, "wb");
    if (!output) {
        fprintf(stderr, "Failed to open output file: %s\n", pcm_file);
        mpg123_close(mh);
        mpg123_delete(mh);
        return -1;
    }

    // Decode and write to the output file
    unsigned char buffer[PCM_BUFFER_SIZE];
    size_t bytes_read;
    while (mpg123_read(mh, buffer, PCM_BUFFER_SIZE, &bytes_read) == MPG123_OK) {
        fwrite(buffer, 1, bytes_read, output);
    }

    // Cleanup
    fclose(output);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();

    return 0;
}

