#include "../s21_decimal.h"

// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result) {
  int err = 1;
  if (result) {
    InitializationDecimal(result);
    int sign = getSign(value);
    s21_decimal fractional = getZeroDecimal(),
                valueUnsignedTruncated = getZeroDecimal();

    s21_decimal valueUnsigned = value;
    setSign(&valueUnsigned, 0);
    s21_truncate(valueUnsigned, &valueUnsignedTruncated);
    s21_sub(valueUnsigned, valueUnsignedTruncated, &fractional);
    setScale(&fractional, getScale(valueUnsigned));
    *result = s21_roundBanking(valueUnsignedTruncated, fractional);
    setSign(result, sign);
    err = 0;
  }
  return err;
}