#include <math.h>
#include "synth.h"

short discretize(char* buf, double* amplitude, short bits_per_sample) {
  if (bits_per_sample == 8) {
    *buf = 255./2 * (*amplitude+1);
    return 1;
  }

  long peak = (1<<(bits_per_sample-1))-1;
  long v = (*amplitude) * peak;

  short  bits_remaining = bits_per_sample;
  while (bits_remaining > 0) {
    *buf = (char)v;
    buf++;
    v >>= 8;
    bits_remaining -= 8;
  }
  return bits_per_sample / 8;
}

short interpolate(char* buf, double* amplitude, short bits_per_sample) {
  if (bits_per_sample == 8) {
    *amplitude = 2./255 * (unsigned char)(*buf) - 1;
    return 1;
  }

  long v = 0;

  short bits_remaining = bits_per_sample;
  while (bits_remaining > 0) {
   v <<= 8;
   v |= *buf;
   buf++;
   bits_remaining -= 8;
  }

  long peak = (1<<(bits_per_sample-1))-1;
  *amplitude = v / peak;

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
    buf += discretize(buf, &amplitude, fmt->bits_per_sample);
  }
}

