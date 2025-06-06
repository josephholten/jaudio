#pragma once
#include "raylib.h"
#include "raymath.h"

struct rplot_param_t {
  Rectangle area;
  Vector2 padding;

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
Vector2 rplot_px_to_pt(struct rplot_param_t* p, Vector2 pt);
void rplot(struct rplot_param_t* p, double* x, double* y, int num_points);
void rplot_ax(struct rplot_param_t* p);
void rplot_box(struct rplot_param_t* p);
void rplot_box_pos_label(struct rplot_param_t* p, Vector2 pt, const char* f);
bool rplot_inside(struct rplot_param_t* p, Vector2 px);
void rplot_zoom(struct rplot_param_t* p, double zoom_factor, Vector2 px, float wheel);
void rplot_pan(struct rplot_param_t* p, double pan_speed, float wheel);
