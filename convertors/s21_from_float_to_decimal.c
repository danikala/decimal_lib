#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  InitializationDecimal(dst);
  int err = 0;

  if ((!dst || src >= MAX_DECIMAL || src <= MIN_DECIMAL) ||
      (fabsf(src) < pow(10.0f, -MAX_POW)))
    err = 1;

  if (!err) {
    int sign = src < 0.0f ? 1 : 0;
    if (sign) src = fabsf(src);

    int count = 0;
    int scale = 0;
    float int_part = 0.0;
    float float_part = modff(src, &int_part);
    while (int_part >= 1e7) {
      int_part /= 10;
      count = 7;
    }
    float digits_counter = fabs(int_part);
    if (count < 7) {
      while (digits_counter >= 1) {
        digits_counter /= 10;
        count++;
      }
      scale = 7 - count;
      if (scale <= 7) {
        float_part *= pow(10, scale);
        int_part *= pow(10, scale);
        int_part += float_part;
      }
    }
    int normalized_value = int_part;

    for (int i = 0; i < 96; i++) {
      if ((normalized_value & 1) == 1) {
        setBit(i, dst);
      }
      normalized_value >>= 1;
    }
    setScale(dst, scale);
    setSign(dst, sign);
  }
  return err;
}
