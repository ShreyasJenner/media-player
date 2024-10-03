#ifndef STRUCTS_H
#define STRUCTS_H

// NOTE: global
#include "audio-metadata-reader/include/stdheader.h"
#include "stdheader.h"
// NOTE: local
#include "audio-metadata-reader/include/flac/flac_structs.h"
#include "audio-metadata-reader/include/mp3/id3_structs.h"

#define FILE_NAME_SZ 256

typedef struct Node File;
typedef struct Tree RootDir;
// typedef struct mp3_frame_header_data MP3FrameHeader;
typedef union MusicFormatDetails MusicFormat;

/* Audio Format details
 * Stores the audio format of the selected file
 * Members ->
 * name: audio file format name
 * musicformat: stores the corresponding audio format details */
union MusicFormatDetails {
  FLACMetadata *flacmetadata;
  ID3Tag *mp3metadata;
};

/*
 * name: name of file
 * type: 'f' for file; 'd' for dir
 * format: audio format details if the node stores a audio file; set to NULL if
 * the node is not an audio file
 * childcount: number of immediate children and sub-children files/dir
 * parent: parent node of current child node
 * children: array of children files/dir
 */
struct Node {
  char *absname;
  char *name;
  char type;
  int childcount;
  MusicFormat *format;
  File *parent;
  File **children;
};

/*
 * root: pointer to root node of tree
 * filecount: number of files
 * dircount: number of directories including root
 */
struct Tree {
  File *root;
  int filecount;
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
// struct mp3_frame_header_data {
//   // normal data
//   float v_id;
//   int layer;
//   bool crc;
//   int bitrate;
//   int samplerate;
//   bool padding;
//   int priv;
//   char channel[14];
//   int mode_extension;
//   bool copyright;
//   bool original;
//   char emphasis[10];
//
//   // derived data
//   int frame_size;
//   int frame_length;
//   int channel_no;
// };

#endif
