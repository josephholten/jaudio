#include "wav.h"
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#define KiB 1024
#define MiB KiB * KiB

int main() {
  struct wav_header_t header;
  short num_channels = 1;
  int sample_rate = 16000; // in Hz
  short bytes_per_sample = sizeof(char);
  short bits_per_sample = bytes_per_sample * 8;
  wav_header_pcm_init(&header, num_channels, sample_rate, bits_per_sample);

  int buffer_size = 0;
  struct wav_data_t* data = wav_data_alloc(buffer_size);

  const char* path = "empty.wav";
  printf("writing empty wav file to %s\n", path);

  int fd = open(path, O_RDWR | O_CREAT, 00664);
  write(fd, &header, sizeof(header));
  write(fd, data, sizeof(data) + buffer_size);
  close(fd);
}
