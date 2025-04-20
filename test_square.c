#include "wav.h"
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#define KiB 1024
#define MiB KiB * KiB

short discretize(char* buf, double amplitude, short bits_per_sample) {
  if (bits_per_sample == 8) {
    buf[0] = 255./2 * (amplitude+1);
    return 1;
  }

  long v = amplitude * ((1<<(bits_per_sample-1))-1);
  short  bits_remaining = bits_per_sample;
  int idx = 0;
  while (bits_remaining > 0) {
    buf[idx] = (char)v;
    idx++;
    buf++;
    v >>= 8;
    bits_remaining -= 8;
  }
  return bits_per_sample / 8;
}

double square_wave(double t, double freq) {
  double wave_length = 1/freq;
  double tt = fmod(t,wave_length);
  if (tt < wave_length/2)
    return 1;
  else
    return -1;
}

double sin_wave(double t, double freq) {
  double pi = atan(1)*4;
  return sin(2*pi*freq*t);
}

typedef double (*wave_fptr)(double,double);

void sample_wave(wave_fptr wave, double freq, struct wav_t* wav) {
  struct wav_fmt_t* fmt = &wav->header.fmt;
  double sample_duration = 1./fmt->sample_rate;
  short bytes_per_sample = fmt->bits_per_sample / 8;
  int num_samples = wav->data.chunk_size / bytes_per_sample;

  char* buf = wav->data.data;
  for (int sample = 0; sample < num_samples; sample++) {
    double time = sample * sample_duration;
    double amplitude = wave(time,freq); // should be <= 1
    buf += discretize(buf, amplitude, fmt->bits_per_sample);
  }
}

void wav_to_file(const char* path, struct wav_t* wav) {
  int fd = open(path, O_RDWR | O_CREAT, 00664);
  write(fd, wav, sizeof(*wav) + wav->data.chunk_size);
  close(fd);
}

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
