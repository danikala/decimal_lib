#include "../s21_decimal.h"

// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result) {
  int err = 1;
  if (result) {
    *result = value;
    setSign(result, !getSign(value));
    err = 0;
  }
  return err;
}