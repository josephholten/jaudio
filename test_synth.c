#include "wav.h"
#include "synth.h"

#include <stdio.h>
#include <stdlib.h>

#define KiB 1024
#define MiB KiB*KiB

int main(int argc, char** argv) {
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
  wav_to_file("square.wav", wav);

  sample_wave(&sin_wave, freq, wav);
  wav_to_file("sin.wav", wav);

  free(wav);
}
