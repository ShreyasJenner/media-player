#ifndef MP3_LUT_H
#define MP3_LUT_H

#include <stdint.h>

struct mp3_frame_data {
    // normal data
    float v_id;
    int layer;
    bool crc;
    int bitrate;
    int samplerate;
    bool padding;
    int priv;
    char channel[14];    
    int mode_extension;
    bool copyright;
    bool original;
    char emphasis[10];

    //derived data
    int frame_size;
    int frame_length;
    int channel_no;
};

void mp3_frame_lookup(uint8_t *bytes);

struct mp3_frame_data *mp3_frame_store(uint8_t *bytes);

#endif
