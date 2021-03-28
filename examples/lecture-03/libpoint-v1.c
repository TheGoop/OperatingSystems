#include "libpoint.h"

#include <stdlib.h>

struct point {
  int y;
  int x;
};

struct point *point_create(int x, int y) {
  struct point* p = (struct point *)malloc(sizeof(struct point));
  p->x = x;
  p->y = y;
  return p;
}

int point_get_x(struct point *p) {
  return p->x;
}

int point_get_y(struct point *p) {
  return p->y;
}

void point_destroy(struct point *p) {
  free(p);
}
