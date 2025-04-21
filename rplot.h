#pragma once
#include "raylib.h"
#include "raymath.h"

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

Rectangle rplot_ax_area(struct rplot_param_t* p);
Vector2 rplot_ax_offset(struct rplot_param_t* p);
Vector2 rplot_ax_size(struct rplot_param_t* p);
Vector2 rplot_pt_to_px(struct rplot_param_t* p, Vector2 pt);
void rplot(struct rplot_param_t* p, double* x, double* y, int num_points);
void rplot_ax(struct rplot_param_t* p);
void rplot_box(struct rplot_param_t* p);
void rplot_box_timerange(struct rplot_param_t* p);

