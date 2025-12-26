#include "../s21_decimal.h"

// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int err = 1;
  if (result) {
    int sign = getSign(value);
    int scale = getScale(value);
    s21_decimal res = getZeroDecimal();
    s21_decimal temp = value;
    temp.bits[3] &= ~(0xFFu << 16);

    for (int i = 0; i < scale; i++) {
      int rem = 0;
      for (int j = 2; j >= 0; j--) {
        uint64_t cur = ((uint64_t)rem << 32) | temp.bits[j];
        temp.bits[j] = (unsigned int)(cur / 10);
        rem = (unsigned int)(cur % 10);
      }
    }
    res.bits[0] = temp.bits[0];
    res.bits[1] = temp.bits[1];
    res.bits[2] = temp.bits[2];
    res.bits[3] = sign ? MINUS : 0u;
    *result = res;

    err = 0;
  }
  return err;
}