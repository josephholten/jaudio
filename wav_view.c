#include "wav.h"
#include "synth.h"
#include "raylib.h"
#include "raymath.h"
#include "rplot.h"

#include "util.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  int ret = 0;

  if (argc < 2) {
    fprintf(stderr, "USAGE: %s <path>\n", argv[0]);
    return 1;
  }
  const char* path = argv[1];
  struct wav_t* wav = wav_from_file(path);
  if (wav == NULL) {
    fprintf(stderr, "ERROR: could not read file '%s' as wav file\n", path);
    return -1;
  }

  const int num_samples = wav_num_samples(wav);
  const double dt = 1./wav->header.fmt.sample_rate;
  const double tmax = num_samples * dt;

  double* t = malloc(sizeof(double)*num_samples);
  double* y = malloc(sizeof(double)*num_samples);
  if (t == NULL || y == NULL) {
    fprintf(stderr, "ERROR: could not allocate memory for t or y\n");
    ret = 1;
    goto end_free;
  }

  char* buf = wav->data.data;
  for (int i = 0; i < num_samples; i++) {
    t[i] = dt * i;
    buf += interpolate(buf, &y[i], wav->header.fmt.bits_per_sample);
  }

  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth,screenHeight, "wav_view");
  SetTargetFPS(60);

  struct rplot_param_t param = {
    .area = {.x = 0, .y = 0, .width = screenWidth, .height = screenHeight},
    .padding = 20,

    .xmin = 0,
    .xmax = tmax,
    .ymin = -1,
    .ymax = 1,

    .thick = 1,
    .color = WHITE,
  };

  while(!WindowShouldClose()) {
    BeginDrawing(); {
      ClearBackground(BLACK);
      rplot_box(&param);
      rplot(&param,t,y,num_samples);
    } EndDrawing();
  }

  CloseWindow();

  ret = 0;

end_free:
  free(t);
  free(y);
  free(wav);

  return ret;
}
