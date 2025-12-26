#include "../s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal neg_b = value_2;
  setSign(&neg_b, !getSign(neg_b));

  return s21_add(value_1, neg_b, result);
}