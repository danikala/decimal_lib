#include "../s21_decimal.h"

int s21_is_equal(s21_decimal v1, s21_decimal v2) {
  int result = 0;

  if (normalize(&v1, &v2) == 0) {
    int both_zero = isZero(v1) && isZero(v2);
    int same_sign = getSign(v1) == getSign(v2);
    int same_bits = (v1.bits[0] == v2.bits[0]) && (v1.bits[1] == v2.bits[1]) &&
                    (v1.bits[2] == v2.bits[2]) &&
                    ((v1.bits[3] & 0x00FF0000) == (v2.bits[3] & 0x00FF0000));

    result = both_zero || (same_sign && same_bits);
  }

  return result;
}
