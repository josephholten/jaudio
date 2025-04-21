#include "rplot.h"

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
  Vector2 ax_root = {p->area.x + p->padding, p->area.y+p->area.height-p->padding};
  Vector2 size = rplot_ax_size(p);
  Vector2 px = {
      .x = ax_root.x + (pt.x - p->xmin) / (p->xmax - p->xmin) * size.x,
      .y = ax_root.y - (pt.y - p->ymin) / (p->ymax - p->ymin) * size.y,
  };
  return px;
}

void rplot_line(struct rplot_param_t* p, Vector2 pt_start, Vector2 pt_end) {
  Vector2 px_start = rplot_pt_to_px(p,pt_start);
  Vector2 px_end   = rplot_pt_to_px(p,pt_end);
  DrawLineEx(px_start,px_end,p->thick,p->color);
}

void rplot(struct rplot_param_t* p, double* x, double* y, int num_points) {
  Vector2 last;

  for (int i = 0; i < num_points; i++) {
    if (x[i] < p->xmin || x[i] > p->xmax || y[i] < p->ymin || y[i] > p->ymax)
      continue;

    Vector2 next = {x[i],y[i]};
    if (i != 0)
      rplot_line(p,last,next);
    last = next;
  }
}

void rplot_ax(struct rplot_param_t* p) {
  Vector2 a = { .x = p->xmin, .y = 0 };
  Vector2 b = { .x = p->xmax, .y = 0 };
  rplot_line(p, a, b);

  Vector2 c = { .x = 0, .y = p->ymin };
  Vector2 d = { .x = 0, .y = p->ymax };
  rplot_line(p, c, d);
}

