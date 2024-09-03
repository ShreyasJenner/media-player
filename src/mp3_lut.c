#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mp3_lut.h"

/*
 * 00 - v2.5
 * 01 - reserved
 * 10 - v2
 * 11 - v1
 */
static const float mpeg_version_id[4] = {
    2.5, 0, 2, 1
};

/*
 * 00 - reserved
 * 01 - 3
 * 10 - 2
 * 00 - 1
 */
static const int layer[4] = {
    0, 3, 2, 1 
};

/*
 * 0 - true
 * 1 - false
 */
static const bool crc_protection[2] = {
    true, false
};

/*
 * 11 11 - v1, l1
 * 11 10 - v1, l2
 * 11 01 - v1, l3
 * 10 11 - v2, l1
 * 10 10 - v2, l2
 * 10 01 - v2, l3
 */
static const int bitrate_index[2][3][16] = {
    {
        {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1},
        {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1},
        {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, -1}
    },
    {
        {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1},
        {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, -1},
        {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1}
    }
};

/*
 * 00 - 44.1Khz
 * 01 - 48Khz
 * 10 - 3.2Khz
 * 11 - reserved
 */
static const int sampling_rate[4][4] = {
    {},
    {11025, 12000, 8000, 0},
    {22050, 24000, 16000, 0},
    {44100, 48000, 32000, 0}
};

static const bool padding[2] = {
    false, true
};

static const int private[2] = {
    -1, -1 
};

static const char *channel_mode[4] = {
    "stereo", "joint(stereo)", "dual(stereo)", "mono"
};

static const int mode_extension[4] = {
    -1, -1, -1, -1 
};

static const bool copyright[2] = {
    false, true
};

static const bool original[2] = {
    true, false
};

static const char *emphasis[4] = {
    "none", "50/15 ms", "reserved", "CCIT J.17"
};

// pass an array of 4 bytes
void mp3_frame_lookup(uint8_t *bytes) {
    float v_id;
    int mp3_layer;
    bool crc;
    int bitrate;
    int samp_rate;
    bool pad;
    char *chan;
    bool copyrit;
    bool orig;
    char *emph;


    v_id = mpeg_version_id[(bytes[1]&24u)>>3];
    mp3_layer = layer[(bytes[1]&6u)>>1];
    crc = crc_protection[(bytes[1]&1u)>>0];
    bitrate = bitrate_index
        [( (bytes[1]&24u) >>3)-2]
        [( (bytes[1]&6u) >>1)-1]
        [(bytes[2]&240u)>>4];
    samp_rate = sampling_rate[(bytes[1]&24u)>>3][(bytes[2]&12u)>>2];
    pad = padding[(bytes[2]&2u)>>1];
    chan = (char *)channel_mode[(bytes[3]&192u)>>6];
    copyrit = copyright[(bytes[3]&8u)>>3];
    orig = original[(bytes[3]&4u)>>2];
    emph = (char *)emphasis[(bytes[3]&2u)>>0];
    

    printf("MPEG Version-%.1f\n",v_id);
    printf("Layer %d\n",mp3_layer);
    printf("CRC: %s\n",crc?"True":"False");
    printf("Bitrate: %dKbps\n",bitrate);
    printf("Sampling Rate: %dKHz\n",samp_rate);
    printf("Padding: %s\n",pad?"True":"False");
    printf("Channel: %s\n",chan);
    printf("Copyright: %s\n",copyrit?"True":"False");
    printf("Original Copy: %s\n",orig?"True":"False");
    printf("Emphasis: %s\n",emph);
}

/* store mp3 frame data into struct */ 
struct mp3_frame_data *mp3_frame_store(uint8_t *bytes) {
    struct mp3_frame_data *mfd;

    float v_id;
    int mp3_layer;
    bool crc;
    int bitrate;
    int samp_rate;
    bool pad;
    char *chan;
    bool copyrit;
    bool orig;
    char *emph;

    v_id = mpeg_version_id[(bytes[1]&24u)>>3];
    mp3_layer = layer[(bytes[1]&6u)>>1];
    crc = crc_protection[(bytes[1]&1u)>>0];
    bitrate = bitrate_index
        [( (bytes[1]&24u) >>3)-2]
        [( (bytes[1]&6u) >>1)-1]
        [(bytes[2]&240u)>>4];
    samp_rate = sampling_rate[(bytes[1]&24u)>>3][(bytes[2]&12u)>>2];
    pad = padding[(bytes[2]&2u)>>1];
    chan = (char *)channel_mode[(bytes[3]&192u)>>6];
    copyrit = copyright[(bytes[3]&8u)>>3];
    orig = original[(bytes[3]&4u)>>2];
    emph = (char *)emphasis[(bytes[3]&2u)>>0];

    // allocate space for struct
    mfd = malloc(sizeof(struct mp3_frame_data));

    // store normal data
    mfd->v_id = v_id;
    mfd->layer = mp3_layer;
    mfd->crc = crc;
    mfd->bitrate = bitrate;
    mfd->samplerate = samp_rate;
    mfd->padding = pad;
    strcpy(mfd->channel, chan);
    mfd->copyright = copyrit;
    mfd->original = orig;
    strcpy(mfd->emphasis, emph);

    // store derived data
    if(mfd->layer==3 || mfd->layer==2) {
        mfd->frame_size = 1152;
        mfd->frame_length = 144*mfd->bitrate*1000/mfd->samplerate+mfd->padding;
    }
    else if(mfd->layer==1) {
        mfd->frame_size = 384;
        mfd->frame_length = (12*mfd->bitrate*1000/mfd->samplerate+(4*mfd->padding))*4;
    }

    if(!strcmp(chan,"stereo") || !strcmp(chan,"joint(stereo)") || 
                !strcmp(chan,"dual(stereo)"))
        mfd->channel_no = 2;
    else
        mfd->channel_no = 1;

    return mfd;
}
