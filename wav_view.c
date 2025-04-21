#include "wav.h"
#include "synth.h"
#include "raylib.h"
#include "raymath.h"

#include "util.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct rplot_param_t {
  Rectangle area;
  int padding;

  double xmax;
  double xmin;
  double ymax;
  double ymin;

  float thick;
  Color color;
};


Rectangle rplot_ax_area(struct rplot_param_t* p) {
  Rectangle area = {
    .x = p->area.x + p->padding,
    .y = p->area.y + p->padding,
    .width = p->area.width - 2*p->padding,
    .height = p->area.height - 2*p->padding,
  };
  return area;
}

Vector2 rplot_ax_offset(struct rplot_param_t* p) {
  Vector2 off = {
    .x = p->area.x + p->padding,
    .y = p->area.y + p->padding,
  };
  return off;
}

Vector2 rplot_ax_size(struct rplot_param_t* p) {
  Vector2 size = {
    .x = p->area.width - 2*p->padding,
    .y = p->area.height - 2*p->padding,
  };
  return size;
}

Vector2 rplot_pt_to_px(struct rplot_param_t* p, Vector2 pt) {
  Vector2 size = rplot_ax_size(p);
  Vector2 px = {
      .x = (pt.x - p->xmin) / (p->xmax - p->xmin) * size.x,
      .y = (pt.y - p->ymin) / (p->ymax - p->ymin) * size.y,
  };
  return px;
}

void rplot(struct rplot_param_t* p, double* x, double* y, int num_points) {
  Vector2 ax_off = rplot_ax_offset(p);
  Vector2 last;

  for (int i = 0; i < num_points; i++) {
    if (x[i] < p->xmin || x[i] > p->xmax || y[i] < p->ymin || y[i] > p->ymax)
      continue;

    Vector2 pt = {x[i],y[i]};
    Vector2 new = rplot_pt_to_px(p, pt);

    if (i != 0) {
      Vector2 start = Vector2Add(last,ax_off);
      Vector2 end   = Vector2Add(new,ax_off);
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
