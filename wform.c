#include "wav.h"
#include "synth.h"
#include "util.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "USAGE: %s <path>\n", argv[0]);
    return 1;
  }
  char* path = argv[1];
  struct wav_t* wav = wav_from_file(path);

  int num_samples = wav_num_samples(wav);
  printf("num samples = %d\n", num_samples);
  double dt = 1./wav->header.fmt.sample_rate;

  double* t = malloc(sizeof(double)*num_samples);
  double* y = malloc(sizeof(double)*num_samples);

  char* buf = wav->data.data;
  for (int i = 0; i < num_samples; i++) {
    t[i] = dt * i;
    buf += interpolate(buf, &y[i], wav->header.fmt.bits_per_sample);
  }

  gnuplot("wform.data", t, y, num_samples);

  free(t);
  free(y);
  free(wav);
}
