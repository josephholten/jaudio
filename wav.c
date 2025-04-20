#include <stdlib.h>
#include <string.h>

#include "wav.h"

void wav_header_pcm_init(struct wav_header_t* header,
    short num_channels, int sample_rate, short bits_per_sample)
{
  int chunk_size;
  struct riff_header_t riff_header = {
    .chunk_id = "RIFF",
    .chunk_size = 0,
    .format = "WAVE",
  };
  chunk_size = sizeof(riff_header.format) + sizeof(struct wav_fmt_t) + sizeof(struct wav_data_t);
  riff_header.chunk_size = chunk_size;
  header->riff_header = riff_header;

  chunk_size = sizeof(struct wav_fmt_t) - sizeof(header->fmt.chunk_id) - sizeof(header->fmt.chunk_size);
  struct wav_fmt_t fmt = {
    .chunk_id = "fmt ",
    .chunk_size = chunk_size,
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

struct wav_t* wav_alloc(struct wav_header_t* header, double duration_in_s) {
  struct wav_fmt_t* fmt = &header->fmt;
  int chunk_size = duration_in_s * fmt->sample_rate * fmt->bits_per_sample / 8;

  struct wav_data_t data_init = {
    .chunk_id = "data",
    .chunk_size = chunk_size,
  };

  struct wav_t* wav = calloc(sizeof(struct wav_t) + chunk_size, 1);
  memcpy(&wav->header, header, sizeof(struct wav_header_t));
  memcpy(&wav->data, &data_init, sizeof(struct wav_data_t));

  return wav;
}

