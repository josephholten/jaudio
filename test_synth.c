#include "wav.h"
#include "synth.h"

#include <stdio.h>
#include <stdlib.h>

#define KiB 1024
#define MiB KiB*KiB

int main(int argc, char** argv) {
  int ret = 0;
  if (argc < 3) {
    fprintf(stderr, "USAGE: %s <sample_rate> <duration> <freq>\n", argv[0]);
    return 1;
  }

  int sample_rate = atof(argv[1]); // in Hz
  double duration = atof(argv[2]); // in s
  double freq = atof(argv[3]); // in Hz

  struct wav_header_t header;
  short num_channels = 1;
  short bytes_per_sample = 1;
  short bits_per_sample = bytes_per_sample * 8;
  wav_header_pcm_init(&header, num_channels, sample_rate, bits_per_sample);

  struct wav_t* wav = wav_alloc(&header,duration);

  printf("writing to square.wav, sin.wav\n");

  sample_wave(&square_wave, freq, wav);
  ret = wav_to_file("square.wav", wav);
  if (ret < 0) {
    fprintf(stderr,"ERROR: could not write file 'square.wav'\n");
    return 1;
  }

  sample_wave(&sin_wave, freq, wav);
  ret = wav_to_file("sin.wav", wav);
  if (ret < 0) {
    fprintf(stderr,"ERROR: could not write file 'sin.wav'\n");
    return 1;
  }

  free(wav);
  return 0;
}
