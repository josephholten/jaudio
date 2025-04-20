#pragma once
#include <stddef.h>

#define PACKED __attribute__((packed))

struct PACKED riff_header_t {
  char chunk_id[4];
  int  chunk_size;
  char format[4];
};

struct PACKED wav_fmt_t {
  char  chunk_id[4];
  int   chunk_size;
  short audio_format;
  short num_channels;
  int   sample_rate;
  int   byte_rate;
  short block_align;
  short bits_per_sample;
};

struct PACKED wav_header_t {
  struct riff_header_t riff_header;
  struct wav_fmt_t fmt;
};

struct PACKED wav_data_t {
  char chunk_id[4];
  int  chunk_size;
  char data[];
};

struct PACKED wav_t {
  struct wav_header_t header;
  struct wav_data_t data;
};

void wav_header_pcm_init(struct wav_header_t* header,
    short num_channels, int sample_rate, short bits_per_sample);

struct wav_data_t* wav_data_alloc(size_t buffer_size);

struct wav_t* wav_alloc(struct wav_header_t* header, double duration_in_s);
