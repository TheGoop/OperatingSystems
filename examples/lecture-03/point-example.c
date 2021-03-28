#include <stdio.h>
#include <stdlib.h>

#include "libpoint.h"

int main(int argc, char **argv)
{
  struct point *p = point_create(1, 2);
  printf("point (x, y) = %d, %d\n", point_get_x(p), point_get_y(p));

  int *point_abi = (int *) p;
  printf("ABI v1: point (x, y) = %d, %d\n", point_abi[1], point_abi[0]);
  point_destroy(p);
  return 0;
}
