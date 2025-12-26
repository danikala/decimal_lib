#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;
  if (result) InitializationDecimal(result);

  if (isZero(value_2)) {
    code = 3;
  } else {
    int res_sign = getSign(value_1) ^ getSign(value_2);

    s21_decimal_big big1 = convertBig(value_1);
    s21_decimal_big big2 = convertBig(value_2);
    SetScaleBig(&big1, 0);
    SetScaleBig(&big2, 0);
    setSignBig(&big1, 0);
    setSignBig(&big2, 0);

    int Scale1 = getScale(value_1);
    int Scale2 = getScale(value_2);

    int res_scale = Scale1 - Scale2;
    s21_decimal_big tmp = big1;
    while (res_scale < 0 && (!Mul10Big(&tmp))) {
      big1 = tmp;
      ++res_scale;
    }

    s21_decimal_big q = getZeroDecimalBig(), r = getZeroDecimalBig();
    s21_divBig(&big1, &big2, &q, &r);

    while (!isZeroBig(&r) && res_scale < 28) {
      if (Mul10Big(&r)) break;
      if (Mul10Big(&q)) break;

      s21_decimal_big digit_q, digit_r;
      s21_divBig(&r, &big2, &digit_q, &digit_r);
      addAndWritint(digit_q, &q);
      r = digit_r;

      ++res_scale;
    }

    if (res_scale == 28 && !isZeroBig(&r)) {
      s21_decimal_big twice_r = r;
      addAndWritint(r, &twice_r);
      int cmp = comparisonBig(&twice_r, &big2);
      int need_round_up = (cmp > 0 || (cmp == 0 && (getBitBig(q, 0) & 1)));

      if (need_round_up) {
        s21_decimal_big one = getZeroDecimalBig();
        setBitBig(&one, 0);
        addAndWritint(one, &q);
      }
    }
    if (res_scale < 0) {
      while (res_scale < 0 && !Mul10Big(&q)) {
        ++res_scale;
      }
      if (res_scale < 0) code = res_sign ? 2 : 1;
    }
    while (overflowCheckBig(q) && code == 0) {
      if (res_scale == 0)
        code = res_sign ? 2 : 1;
      else {
        div10(&q);
        --res_scale;
      }
    }

    if (code == 0) {
      *result = convertBack(q);
      setSign(result, res_sign);
      setScale(result, res_scale);

      if (overflowCheckBig(q)) code = res_sign ? 2 : 1;
    }
  }

  return code;
}
