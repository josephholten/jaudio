#include "wav.h"
#include "synth.h"
#include "raylib.h"
#include "raymath.h"
#include "rplot.h"

#include "util.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <string.h>

void slow_fourier_transform(double* y, double complex* f, size_t n) {
  const double pi = atan(1)*4;
  for (size_t k = 0; k < n; k++) {
    f[k] = 0;
    for (size_t j = 0; j < n; j++) {
      f[k] += y[j] * cexp(-I*2*pi*k/n*j);
    }
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
  const double fmin = 1/tmax;
  const double fmax = 1/dt;

  double* t = malloc(sizeof(double)*num_samples);
  double* f = malloc(sizeof(double)*num_samples);
  double* y = malloc(sizeof(double)*num_samples);
  double complex* yf = malloc(sizeof(double complex)*num_samples);
  double* ya = malloc(sizeof(double)*num_samples);
  if (t == NULL || y == NULL || yf == NULL || ya == NULL) {
    fprintf(stderr, "ERROR: could not allocate memory\n");
    ret = 1;
    goto end_free;
  }

  char* buf = wav->data.data;
  for (int i = 0; i < num_samples; i++) {
    t[i] = dt * i;
    f[i] = (double)i / tmax;
    buf += interpolate(buf, &y[i], wav->header.fmt.bits_per_sample);
  }
  double ya_max = 0;
  slow_fourier_transform(y,yf,num_samples);
  for (int i = 0; i < num_samples; i++) {
    ya[i] = cabs(yf[i]);
    if (ya[i] > ya_max)
      ya_max = ya[i];
  }
  gnuplot("dft",f,ya,num_samples);

  const int screen_width = 800;
  const int screen_height = 800;
  const double zoom_factor = 1.05;

  InitWindow(screen_width, screen_height, "wav_view");
  SetTargetFPS(60);

  struct rplot_param_t param_y = {
    .area = {.x = 0, .y = 0, .width = screen_width, .height = screen_height/2},
    .padding = {60, 20},

    .xmin =  0,
    .xmax =  tmax,
    .ymin = -1,
    .ymax =  1,

    .thick = 1,
    .color = WHITE,
  };

  struct rplot_param_t param_f = {
    .area = {.x = 0, .y = 400, .width = screen_width, .height = screen_height/2},
    .padding = {60, 20},

    .xmin =  fmin,
    .xmax =  fmax,
    .ymin =  0,
    .ymax =  ya_max,

    .thick = 1,
    .color = WHITE,
  };

  struct rplot_param_t* py = &param_y;
  struct rplot_param_t* pf = &param_f;

  while(!WindowShouldClose()) {
    Vector2 mouse_px = GetMousePosition();
    Vector2 mouse_pt = rplot_px_to_pt(py,mouse_px);
    float mouse_wheel = GetMouseWheelMove();

    py->xmax = mouse_pt.x + (py->xmax - mouse_pt.x)*pow(zoom_factor,-mouse_wheel);
    py->xmin = mouse_pt.x - (mouse_pt.x - py->xmin)*pow(zoom_factor,-mouse_wheel);


    if (IsKeyPressed(KEY_ZERO)) {
      py->xmin =  0;
      py->xmax =  tmax;
      py->ymin = -1;
      py->ymax =  1;
    }

    BeginDrawing(); {
      ClearBackground(BLACK);

      // y
      rplot_box(py);
      rplot_box_timerange(py);
      rplot_box_yrange(py);
      mouse_pt = rplot_px_to_pt(py,mouse_px);
      rplot_box_pos_label(py,mouse_pt,mouse_px,"te");
      rplot(py,t,y,num_samples);

      // f
      rplot_box(pf);
      // rplot_box_timerange(pf);
      rplot_box_yrange(pf);
      mouse_pt = rplot_px_to_pt(pf,mouse_px);
      rplot_box_pos_label(pf,mouse_pt,mouse_px,"ee");
      rplot(pf,f,ya,num_samples);

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
