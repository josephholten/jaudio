#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define PACKED __attribute__((packed))
#define KiB 1024;

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

int main() {
  struct wav_header_t header;
  short num_channels = 1;
  int sample_rate = 1;
  short bits_per_sample = sizeof(char);
  wav_header_pcm_init(&header, num_channels, sample_rate, bits_per_sample);

  int buffer_size = 4*KiB;
  struct wav_data_t* data = wav_data_alloc(buffer_size);

  int fd = open("1.wav", O_RDWR | O_CREAT, 00664);
  write(fd, &header, sizeof(header));
  write(fd, data, sizeof(data) + buffer_size);
  close(fd);
}
