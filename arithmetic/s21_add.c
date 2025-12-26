#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  int res_sign = 0;
  int scale = 0;
  s21_decimal_big tmp = getZeroDecimalBig();

  InitializationDecimal(result);

  status = normalize(&value_1, &value_2);

  if (status == 0) {
    int sign1 = getSign(value_1);
    int sign2 = getSign(value_2);
    scale = getScale(value_1);

    s21_decimal_big b1 = convertBig(value_1);
    s21_decimal_big b2 = convertBig(value_2);
    SetScaleBig(&b1, 0);
    SetScaleBig(&b2, 0);
    setSignBig(&b1, 0);
    setSignBig(&b2, 0);

    if (sign1 == sign2) {
      s21_addBig(b1, b2, &tmp);
      res_sign = sign1;
      while (status == 0 && overflowCheckBig(tmp)) {
        if (scale == 0) {
          status = res_sign ? 2 : 1;
        } else {
          div10(&tmp);
          --scale;
        }
      }
    } else {
      int cmp = comparisonBig(&b1, &b2);
      if (cmp == 0) {
      } else {
        if (cmp > 0) {
          s21_subBig(b1, b2, &tmp);
          res_sign = sign1;
        } else {
          s21_subBig(b2, b1, &tmp);
          res_sign = sign2;
        }

        while (status == 0 && overflowCheckBig(tmp)) {
          if (scale == 0) {
            status = res_sign ? 2 : 1;
          } else {
            div10(&tmp);
            --scale;
          }
        }
      }
    }
  }

  if (status == 0) {
    *result = convertBack(tmp);
    setSign(result, res_sign);
    setScale(result, scale);
  }

  return status;
}
