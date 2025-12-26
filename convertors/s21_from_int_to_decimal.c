#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int err = 1;
  if (dst) {
    for (int i = 0; i < 4; i++) dst->bits[i] = 0;

    if (src < 0) {
      src = -src;
      setSign(dst, 1);
    }
    dst->bits[0] = src;
    err = 0;
  }
  return err;
}
