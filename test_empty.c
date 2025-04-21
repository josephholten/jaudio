#include "wav.h"
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define KiB 1024
#define MiB KiB * KiB

int main() {
  struct wav_header_t header;
  short num_channels = 1;
  int sample_rate = 16000; // in Hz
  short bytes_per_sample = sizeof(char);
  short bits_per_sample = bytes_per_sample * 8;
  wav_header_pcm_init(&header, num_channels, sample_rate, bits_per_sample);

  double duration = 0;
  struct wav_t* wav = wav_alloc(&header, duration);

  const char* path = "empty.wav";
  printf("writing empty wav file to %s\n", path);

  int fd = open(path, O_RDWR | O_CREAT, 00664);
  write(fd, wav, sizeof(struct wav_t));
  close(fd);

  free(wav);
}
