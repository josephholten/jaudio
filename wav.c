#include <stdlib.h>

#include "wav.h"

void wav_header_pcm_init(struct wav_header_t* header,
    short num_channels, int sample_rate, short bits_per_sample)
{
  struct riff_header_t riff_header = {
    .chunk_id = "RIFF",
    .chunk_size = 0,
    .format = "WAV",
  };
  int chunk_size = sizeof(riff_header.format) + sizeof(struct wav_fmt_t) + sizeof(struct wav_data_t);
  riff_header.chunk_size = chunk_size;
  header->riff_header = riff_header;

  struct wav_fmt_t fmt = {
    .chunk_id = "fmt",
    .chunk_size = sizeof(struct wav_fmt_t),
    .audio_format = 1, // PCM
    .num_channels = num_channels,
    .sample_rate = sample_rate,
    .byte_rate = sample_rate * num_channels * bits_per_sample / 8,
    .block_align = num_channels * bits_per_sample / 8,
    .bits_per_sample = bits_per_sample,
  };
  header->fmt = fmt;
}

struct wav_data_t* wav_data_alloc(size_t buffer_size) {
  struct wav_data_t data_init = {
    .chunk_id = "data",
    .chunk_size = buffer_size,
  };

  struct wav_data_t* data = malloc(sizeof(struct wav_data_t) + buffer_size);
  *data = data_init;

  return data;
}

