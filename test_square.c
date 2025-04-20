#include "wav.h"
#include "synth.h"

#include <stdio.h>

#define KiB 1024
#define MiB KiB*KiB

int main() {
  struct wav_header_t header;
  short num_channels = 1;
  int sample_rate = 16000; // in Hz
  short bytes_per_sample = 1;
  short bits_per_sample = bytes_per_sample * 8;
  wav_header_pcm_init(&header, num_channels, sample_rate, bits_per_sample);

  int duration = 5; // in s
  struct wav_t* wav = wav_alloc(&header,duration);

  int freq = 220; // in Hz

  printf("writing to square.wav, sin.wav\n");

  sample_wave(&square_wave, freq, wav);
  wav_to_file("square.wav", wav);

  sample_wave(&sin_wave, freq, wav);
  wav_to_file("sin.wav", wav);
}
