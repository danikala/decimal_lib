#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst) return 1;

  int scale = getScale(src);
  if (scale < 0 || scale > 28) return 1;
  if ((src.bits[3] & 0x0000FFFF) != 0) return 1;
  if ((src.bits[3] & 0x7F000000) != 0) return 1;

  int err = 0;

  *dst = 0.0f;
  int sign = getSign(src) ? -1 : 1;
  double res = 0.0;
  for (int i = 0; i < 3; i++) res += (double)src.bits[i] * (1ULL << (32 * i));
  if (scale > 0) res /= pow(10.0, scale);
  res *= sign;
  if (res > FLT_MAX || res < -FLT_MAX) err = 1;
  *dst = (float)res;
  return err;
}