#include "../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) return 3;

  InitializationDecimal(result);

  int sign_res = (getSign(value_1) == getSign(value_2)) ? 0 : 1;
  int sum_scale = getScale(value_1) + getScale(value_2);

  s21_decimal_big big1 = convertBig(value_1);
  s21_decimal_big big2 = convertBig(value_2);

  big1.bits[3] = big1.bits[BIG_META_INDEX] = 0;
  big2.bits[3] = big2.bits[BIG_META_INDEX] = 0;

  s21_decimal_big big_res = s21_mulBig(big1, big2);

  if (sum_scale > 28) {
    Div10countBig(&big_res, sum_scale - 28);
    sum_scale = 28;
  }

  int code = 0;
  if (overflowCheckBig(big_res)) code = sign_res ? 2 : 1;

  if (code == 0) {
    s21_decimal tmp = convertBack(big_res);
    setScale(&tmp, sum_scale);
    setSign(&tmp, sign_res);
    *result = tmp;
  }

  return code;
}
