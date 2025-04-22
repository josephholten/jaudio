#include "rplot.h"
#include <stdio.h>

Rectangle rplot_ax_area(struct rplot_param_t* p) {
  Rectangle area = {
    .x = p->area.x + p->padding.x,
    .y = p->area.y + p->padding.y,
    .width = p->area.width - 2*p->padding.x,
    .height = p->area.height - 2*p->padding.y,
  };
  return area;
}

Vector2 rplot_ax_offset(struct rplot_param_t* p) {
  Vector2 off = {
    .x = p->area.x + p->padding.x,
    .y = p->area.y + p->padding.y,
  };
  return off;
}

Vector2 rplot_ax_size(struct rplot_param_t* p) {
  Vector2 size = {
    .x = p->area.width - 2*p->padding.x,
    .y = p->area.height - 2*p->padding.y,
  };
  return size;
}

Vector2 rplot_pt_to_px(struct rplot_param_t* p, Vector2 pt) {
  Vector2 ax_root = {p->area.x + p->padding.x, p->area.y+p->area.height-p->padding.y};
  Vector2 size = rplot_ax_size(p);
  Vector2 px = {
      .x = ax_root.x + (pt.x - p->xmin) / (p->xmax - p->xmin) * size.x,
      .y = ax_root.y - (pt.y - p->ymin) / (p->ymax - p->ymin) * size.y,
  };
  return px;
}

Vector2 rplot_px_to_pt(struct rplot_param_t* p, Vector2 px) {
  Vector2 ax_root = {p->area.x + p->padding.x, p->area.y+p->area.height-p->padding.y};
  Vector2 size = rplot_ax_size(p);

  Vector2 pt = {
    .x = p->xmin + (px.x - ax_root.x) / size.x * (p->xmax - p->xmin),
    .y = p->ymin + (ax_root.y - px.y) / size.y * (p->ymax - p->ymin),
  };
  return pt;
}

void rplot_line(struct rplot_param_t* p, Vector2 pt_start, Vector2 pt_end) {
  Vector2 px_start = rplot_pt_to_px(p,pt_start);
  Vector2 px_end   = rplot_pt_to_px(p,pt_end);
  DrawLineEx(px_start,px_end,p->thick,p->color);
}

void rplot_lines(struct rplot_param_t* p, Vector2* points, int num_points) {
  for (int i = 0; i < num_points-1; i++) {
    rplot_line(p,points[i],points[i+1]);
  }
}

void rplot(struct rplot_param_t* p, double* x, double* y, int num_points) {
  bool lastValid = false;
  Vector2 last;

  for (int i = 0; i < num_points; i++) {
    if (x[i] < p->xmin || x[i] > p->xmax || y[i] < p->ymin || y[i] > p->ymax)
      continue;

    Vector2 next = {x[i],y[i]};
    if (lastValid) {
      rplot_line(p,last,next);
    }
    last = next;
    lastValid = true;
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

void rplot_box(struct rplot_param_t* p) {
  Vector2 NE = { .x = p->xmin, .y = p->ymax };
  Vector2 NW = { .x = p->xmax, .y = p->ymax };
  Vector2 SW = { .x = p->xmax, .y = p->ymin };
  Vector2 SE = { .x = p->xmin, .y = p->ymin };

  Vector2 points[5] = {NE, NW, SW, SE, NE};

  rplot_lines(p, points, 5);
}

void rplot_box_timerange(struct rplot_param_t* p) {
  char buf[128];
  Rectangle ax_area = rplot_ax_area(p);
  int fontSize = 10;
  Color fontColor = WHITE;

  sprintf(buf,"%02d:%02.3f",(int)(p->xmin/60),fmod(p->xmin,60));
  int x = ax_area.x;
  int y = ax_area.y+ax_area.height;
  DrawText(buf,x,y,fontSize,fontColor);

  sprintf(buf,"%02d:%02.3f",(int)(p->xmax/60),fmod(p->xmax,60));
  x += ax_area.width;
  DrawText(buf,x,y,10,fontColor);
}

void rplot_box_yrange(struct rplot_param_t* p) {
  char buf[128];
  Rectangle ax_area = rplot_ax_area(p);
  int fontSize = 10;
  Color fontColor = WHITE;
  int text_len, x, y;

  sprintf(buf,"%.3e",p->ymax);
  text_len = MeasureText(buf,fontSize);
  x = ax_area.x - text_len;
  y = ax_area.y - fontSize;
  DrawText(buf,x,y,fontSize,fontColor);

  sprintf(buf,"%.3e",p->ymin);
  text_len = MeasureText(buf,fontSize);
  x = ax_area.x - text_len;
  y = ax_area.y + ax_area.height - fontSize;
  DrawText(buf,x,y,fontSize,fontColor);
}

void rplot_box_pos_label(struct rplot_param_t* p, Vector2 pt, Vector2 px) {
  char buf[128];
  Rectangle ax_area = rplot_ax_area(p);
  int fontSize = 10;
  Color fontColor = WHITE;
  int text_len, x, y;

  // y label
  sprintf(buf,"%.3e",pt.y);
  text_len = MeasureText(buf,fontSize);
  x = ax_area.x - text_len;
  y = px.y - fontSize;
  DrawText(buf,x,y,fontSize,fontColor);

  // x label
  sprintf(buf,"%02d:%02.3f",(int)(pt.x/60),fmod(pt.x,60));
  text_len = MeasureText(buf,fontSize);
  x = px.x;
  y = ax_area.y + ax_area.height;
  DrawText(buf,x,y,fontSize,fontColor);
}

