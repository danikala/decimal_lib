#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int err = 1;
  if (dst) {
    int scale = (src.bits[3] >> 16) & 0xFF;
    int sign = (src.bits[3] & MINUS) ? -1 : 1;

    if (scale <= MAX_POW) {
      double temp = (double)src.bits[0];

      for (int i = 0; i < scale; i++) temp /= 10.0;

      temp = round(temp);
      *dst = (int)temp * sign;
      err = 0;
    }
  }

  return err;
}
