#include "../s21_decimal.h"

// Округляет указанное Decimal до ближайшего целого в сторону отрицательной
// бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result) {
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

    // Если дробная часть была больше нуля и число было отрицательным, то
    // прибавляем 1
    s21_decimal tempDecimal = getZeroDecimal();
    if (sign == S21_NEGATIVE && s21_is_greater(fractional, tempDecimal)) {
      setBit(0, &tempDecimal);
      s21_add(valueUnsignedTruncated, tempDecimal, &valueUnsignedTruncated);
    }
    *result = valueUnsignedTruncated;
    setSign(result, sign);
    err = 0;
  }
  return err;
}