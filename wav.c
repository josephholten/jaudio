#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "util.h"
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

int wav_num_samples(struct wav_t* wav) {
  short bytes_per_sample = wav->header.fmt.bits_per_sample / 8;
  return wav->data.chunk_size / bytes_per_sample;
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
  int chunk_size = duration_in_s * header->fmt.byte_rate;
  header->riff_header.chunk_size += chunk_size;

  struct wav_data_t data_init = {
    .chunk_id = "data",
    .chunk_size = chunk_size,
  };

  struct wav_t* wav = calloc(sizeof(struct wav_t) + chunk_size, 1);
  memcpy(&wav->header, header, sizeof(struct wav_header_t));
  memcpy(&wav->data, &data_init, sizeof(struct wav_data_t));

  return wav;
}

void wav_to_file(const char* path, struct wav_t* wav) {
  int fd = open(path, O_WRONLY | O_CREAT, 00664);
  write(fd, wav, sizeof(*wav) + wav->data.chunk_size);
  close(fd);
}

struct wav_t* wav_from_file(const char* path) {
  ssize_t ret;
  int fd = open(path, O_RDONLY | O_CREAT, 00664);
  struct wav_header_t header;
  struct wav_t* wav = NULL;

  // read only header to get total data size
  size_t max_tries = 100;
  ret = rread(fd,&header,sizeof(header),max_tries);
  if (ret < 0) {
    fprintf(stderr,"ERROR: could not read header\n");
    goto end;
  }

  // allocate enough space
  int size = header.riff_header.chunk_size + 8;
  wav = calloc(size,1);
  ret = rread(fd,wav,size,max_tries);
  if (ret < 0) {
    fprintf(stderr,"ERROR: could not read file\n");
    goto end;
  }

end:
  close(fd);
  return wav;
}
