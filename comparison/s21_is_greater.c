#include "../s21_decimal.h"

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;

  int sign_1 = getSign(value_1);
  if (sign_1 == getSign(value_2)) {
    normalize(&value_1, &value_2);

    int end = 0;
    for (int i = 3; i >= 0 && !end; i--) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = (sign_1 == 0) ? 1 : 0;
        end = 1;
      } else if (value_1.bits[i] < value_2.bits[i]) {
        result = (sign_1 == 0) ? 0 : 1;
        end = 1;
      }
    }
  } else
    result = (sign_1 == 0) ? 1 : 0;

  return result;
}