#ifndef STRUCTS_H
#define STRUCTS_H

#define FILE_NAME_SZ 256

/*
 * name: name of file
 * type: 'f' for file; 'd' for dir
 * format: audio format
 * count: number of children and sub-children files/dir
 * children: array of children files/dir 
 */
struct Node {
    char name[FILE_NAME_SZ];
    char type;
    char format[5];
    int count;
    struct Node **children;
};


/*
 * Refer mp3_lut.c file to get a list of all the flag details
 * OR 
 * Refer http://mpgedit.org/mpgedit/mpeg_format/mpeghdr.htm
 *
 * v_id: mpeg version
 * layer: mp3 layer
 * crc: is crc there
 * bitrate: bitrate of mp3 file in Kbs
 * samplerate: samplerate frequency in hz
 * padding: is padding there
 * channel: channel mode used
 * mode_extension: [refer mp3 docs]
 * copyright: is copyright there
 * original: is audio original
 * emphasis: [refer mp3 docs]
 *
 * frame_size: number of samples contained in frame [depends on layer]
 * frame_length: length of frame in bytes [depends on layer]
 * channel_no: number of channes [depends on channel mode]
 *
 */
struct mp3_frame_header_data {
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

#endif
