#include "wav.h"
#include "synth.h"
#include "raylib.h"
#include "raymath.h"

#include "util.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct rplot_param_t {
  Vector2 offset;
  int width;
  int height;
  double xmax;
  double xmin;
  double ymax;
  double ymin;
  float thick;
  Color color;
};


void rplot(double* x, double* y, int num_points, struct rplot_param_t* p) {
  Vector2 last;

  for (int i = 0; i < num_points; i++) {
    if (x[i] < p->xmin || x[i] > p->xmax || y[i] < p->ymin || y[i] > p->ymax)
      continue;

    Vector2 new = {
      .x = (x[i] - p->xmin) / (p->xmax - p->xmin) * p->width,
      .y = (y[i] - p->ymin) / (p->ymax - p->ymin) * p->height,
    };

    if (i != 0) {
      Vector2 start = Vector2Add(last,p->offset);
      Vector2 end   = Vector2Add(new,p->offset);
      DrawLineEx(start,end,p->thick,p->color);
    }

    last = new;
  }
}

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
    .offset = {0,0},
    .width = screenWidth,
    .height = screenHeight,
    .xmin = 0,
    .xmax = tmax,
    .ymin = -1,
    .ymax = 1,
    .thick = 3,
    .color = WHITE,
  };

  while(!WindowShouldClose()) {
    BeginDrawing(); {
      ClearBackground(BLACK);
      rplot(t,y,num_samples,&param);
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
