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
  if (wav == NULL) {
    fprintf(stderr, "ERROR: could not read file '%s' as wav file\n", path);
    return -1;
  }

  int num_samples = wav_num_samples(wav);
  double dt = 1./wav->header.fmt.sample_rate;

  double* t = malloc(sizeof(double)*num_samples);
  double* y = malloc(sizeof(double)*num_samples);

  char* buf = wav->data.data;
  for (int i = 0; i < num_samples; i++) {
    t[i] = dt * i;
    buf += interpolate(buf, &y[i], wav->header.fmt.bits_per_sample);
  }

  if (gnuplot(path, t, y, num_samples) < 0) {
    fprintf(stderr,"ERROR: could not plot");
  }
  printf("writing gnuplot file to '%s.gp'\n",path);

  free(t);
  free(y);
  free(wav);
}
