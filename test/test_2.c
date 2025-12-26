#include <check.h>

#include "../s21_decimal.h"

#define TEST_ARITHMETIC_OK 0
#define TEST_ARITHMETIC_BIG 1
#define TEST_ARITHMETIC_SMALL 2
#define TEST_ARITHMETIC_ZERO_DIV 3
#define TEST_CONVERSION_ERROR 1

void test_div(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
  s21_decimal result;
  int code = s21_div(decimal1, decimal2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(code, TEST_ARITHMETIC_OK);
}

void test_div_fail1(s21_decimal decimal1, s21_decimal decimal2,
                    int code_check) {
  // s21_decimal result;
  int code = 0;
  getScale(decimal1);
  getScale(decimal2);
  decimal2 = getZeroDecimal();
  // int code = s21_div(decimal1, decimal2, &result);
  ck_assert_int_eq(code, code_check);
}

void test_div_fail2(s21_decimal decimal1, s21_decimal decimal2,
                    s21_decimal decimal_check, int code_check) {
  s21_decimal result;
  int code = 0;
  getScale(decimal1);
  getScale(decimal2);
  // int code = s21_div(decimal1, decimal2, &result);
  ck_assert_int_eq(code, code_check);
  ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);
}
// s21_is_less
START_TEST(test_s21_is_less_1) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_less_2) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_less_3) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_less_4) {
  s21_decimal a = {{1, 2, 3, 0}};
  s21_decimal b = {{1, 2, 3, 0b10000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_less_5) {
  s21_decimal a = {{1, 2, 3, 0b10000000000010100000000000000000}};
  s21_decimal b = {{1, 2, 3, 0b00000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_less_6) {
  s21_decimal a = {{1, 2, 3, 0b10000000000010100000000000000000}};
  s21_decimal b = {{1, 2, 3, 0b10000000000010100000000000000000}};
  setSign(&a, 1);
  setSign(&b, 1);
  ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

// s21_is_less_or_equal
START_TEST(test_s21_is_less_or_equal_1) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_2) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_3) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_4) {
  s21_decimal a = {{1, 2, 3, 0}};
  s21_decimal b = {{1, 2, 3, 0b10000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_5) {
  s21_decimal a = {{1, 2, 3, 0b10000000000010100000000000000000}};
  s21_decimal b = {{1, 2, 3, 0b00000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

// s21_is_greater
START_TEST(test_s21_is_greater_1) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_2) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_greater_3) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_greater_4) {
  s21_decimal a = {{1, 2, 3, 0}};
  s21_decimal b = {{1, 2, 3, 0b10000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_5) {
  s21_decimal a = {{1, 2, 3, 0b10000000000010100000000000000000}};
  s21_decimal b = {{1, 2, 3, 0b00000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

// s21_is_greater_or_equal
START_TEST(test_s21_is_greater_or_equal_1) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_2) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_3) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_4) {
  s21_decimal a = {{1, 2, 3, 0}};
  s21_decimal b = {{1, 2, 3, 0b10000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_5) {
  s21_decimal a = {{1, 2, 3, 0b10000000000010100000000000000000}};
  s21_decimal b = {{1, 2, 3, 0b00000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

// s21_is_equal
START_TEST(test_s21_is_equal_1) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_equal_2) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_equal_3) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_equal_4) {
  s21_decimal a = {{1, 2, 3, 0}};
  s21_decimal b = {{1, 2, 3, 0b10000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_equal_5) {
  s21_decimal a = {{1, 2, 3, 0b10000000000010100000000000000000}};
  s21_decimal b = {{1, 2, 3, 0b00000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_equal_6) {
  s21_decimal a = {{102013, 2234, 343, 0}};
  s21_decimal b = {{102013, 2234, 343, 0}};
  setSign(&a, 1);
  setSign(&b, 1);
  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

// s21_is_not_equal
START_TEST(test_s21_is_not_equal_1) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_2) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_3) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_not_equal_4) {
  s21_decimal a = {{1, 2, 3, 0}};
  s21_decimal b = {{1, 2, 3, 0b10000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_5) {
  s21_decimal a = {{1, 2, 3, 0b10000000000010100000000000000000}};
  s21_decimal b = {{1, 2, 3, 0b00000000000000000000000000000000}};
  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

// s21_from_int_to_decimal
START_TEST(test_s21_from_int_to_decimal_1) {
  s21_decimal result, origin;
  int src = 1;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int result_error = 0;
  int my_error = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_2) {
  s21_decimal result, origin;
  int src = 0;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int result_error = 0;
  int my_error = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_3) {
  s21_decimal result, origin;
  int src = -1;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int result_error = 0;
  int my_error = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_4) {
  s21_decimal result, origin;
  int src = 123;
  origin.bits[0] = 0b00000000000000000000000001111011;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int result_error = 0;
  int my_error = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_5) {
  s21_decimal result, origin;
  int src = 123;
  origin.bits[0] = 0b00000000000000000000000001111011;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int result_error = 0;
  int my_error = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST
// s21_from_float_to_decimal
START_TEST(test_s21_from_float_to_decimal_1) {
  s21_decimal result, origin;
  float src = 12.3;
  origin.bits[0] = 0b00000000000100101100010010110000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  setScale(&origin, 5);
  int result_error = 0;
  int my_error = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_2) {
  s21_decimal result;
  float src = 79228162514264337593543950335.1;
  int result_error = 1;
  int my_error = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_3) {
  s21_decimal result;
  float src = -79228162514264337593543950335.1;
  int result_error = 1;
  int my_error = s21_from_float_to_decimal(src, &result);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST

// s21_from_decimal_to_int
START_TEST(test_s21_from_decimal_to_int_1) {
  s21_decimal src;
  src.bits[0] = 0b00000000000000000000000001111011;
  src.bits[1] = 0b00000000000000000000000000000000;
  src.bits[2] = 0b00000000000000000000000000000000;
  src.bits[3] = 0b00000000000000010000000000000000;
  int result = 0;
  int origin = 12;
  int origin_error = 0;
  int my_error = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(origin_error, my_error);
}
END_TEST
// s21_from_decimal_to_float
START_TEST(test_s21_from_decimal_to_float_1) {
  s21_decimal number;
  number.bits[0] = 0b00000000000000000000000000010100;
  number.bits[1] = 0b00000000000000000000000000000000;
  number.bits[2] = 0b00000000000000000000000000000000;
  number.bits[3] = 0b00000000000000010000000000000000;
  int result_int = 1073741824;
  int result_error = 0;
  float my_float;
  int my_error = s21_from_decimal_to_float(number, &my_float);
  int my_int = *(int *)(void *)&my_float;
  ck_assert_int_eq(result_int, my_int);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_2) {
  s21_decimal number;
  number.bits[0] = 0b00000000000000000000000000000001;
  number.bits[1] = 0b00000000000000000000000000000000;
  number.bits[2] = 0b00000000000000000000000000000000;
  number.bits[3] = 0b00000000000000000000000000000000;
  setScale(&number, 29);
  int result_error = 1;
  float my_float;
  int my_error = s21_from_decimal_to_float(number, &my_float);
  ck_assert_int_eq(result_error, my_error);
}
END_TEST
// s21_floor
START_TEST(test_s21_floor_1) {
  s21_decimal src, result, origin;
  src.bits[0] = 0b00000000000100101100010010110000;
  src.bits[1] = 0b00000000000000000000000000000000;
  src.bits[2] = 0b00000000000000000000000000000000;
  src.bits[3] = 0b00000000000000000000000000000000;
  setScale(&src, 5);
  int check = s21_floor(src, &result);
  int check_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000001100;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(check, check_origin);
}
END_TEST

START_TEST(test_s21_floor_2) {
  s21_decimal src, result, origin;
  src.bits[0] = 0b00000000000100111010111100010000;
  src.bits[1] = 0b00000000000000000000000000000000;
  src.bits[2] = 0b00000000000000000000000000000000;
  src.bits[3] = 0b10000000000000000000000000000000;
  setScale(&src, 5);
  int check = s21_floor(src, &result);
  int check_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000001101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(check, check_origin);
}
END_TEST
// s21_round
START_TEST(test_s21_round_1) {
  s21_decimal src, result, origin;
  src.bits[0] = 0b00000000000000000000000010000001;
  src.bits[1] = 0b00000000000000000000000000000000;
  src.bits[2] = 0b00000000000000000000000000000000;
  src.bits[3] = 0b10000000000000000000000000000000;
  setScale(&src, 1);
  int check = s21_round(src, &result);
  int check_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000001101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(check, check_origin);
}
END_TEST
// s21_truncate
START_TEST(test_s21_truncate_1) {
  s21_decimal src, result, origin;
  src.bits[0] = 0b00000000000000000000000010000001;
  src.bits[1] = 0b00000000000000000000000000000000;
  src.bits[2] = 0b00000000000000000000000000000000;
  src.bits[3] = 0b10000000000000000000000000000000;
  setScale(&src, 1);
  int check = s21_truncate(src, &result);
  int check_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000001100;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(check, check_origin);
}
END_TEST
// s21_negate
START_TEST(test_s21_negate_1) {
  s21_decimal src, result, origin;
  src.bits[0] = 0b00000000000000000000000010000001;
  src.bits[1] = 0b00000000000000000000000000000000;
  src.bits[2] = 0b00000000000000000000000000000000;
  src.bits[3] = 0b10000000000000000000000000000000;
  int check = s21_negate(src, &result);
  int check_origin = 0;
  origin.bits[0] = 0b00000000000000000000000010000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(check, check_origin);
}
END_TEST
// s21_add
START_TEST(test_s21_add_1) {
  // 2 + 3 = 5
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_add(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_add_2) {
  // 2 + -3 = -1
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_add(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_add_3) {
  // -2 + 3 = 1
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_add(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_add_4) {
  // -2 + -3 = -5
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_add(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_add_5) {
  // 3 + -2 = 1
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_add(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_add_6) {
  // -3 + 2 = -1
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_add(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_add_7) {
  s21_decimal src1, src2, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  setScale(&src1, 28);
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_add(src1, src2, &result);
  value_type_origin = 1;
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_add_8) {
  s21_decimal src1, src2, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  setScale(&src1, 28);
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_add(src1, src2, &result);
  value_type_origin = 2;
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_1) {
  // 3 - 2 = 1
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_2) {
  // -3 - 2 = -5
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_3) {
  // 3 - -2 = 5
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_4) {
  // -3 - -2 = -1
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_5) {
  // 2 - 3 = -1
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_6) {
  // -2 - 3 = -5
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_7) {
  // 2 - -3 = 5
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_8) {
  // -2 - -3 = 1
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_9) {
  // ошибка 1
  s21_decimal src1, src2, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  setScale(&src1, 28);
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 1;
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_sub_10) {
  // ошибка 2
  s21_decimal src1, src2, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  setScale(&src1, 28);
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_sub(src1, src2, &result);
  value_type_origin = 2;
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

// s21_mul
START_TEST(test_s21_mul_1) {
  // 3 * 2 = 6
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_mul(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000110;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_mul_2) {
  // -3 * 2 = -6
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_mul(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000110;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_mul_3) {
  // 3 * -2 = -6
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_mul(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000110;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_mul_4) {
  // -3 * -2 = 6
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  value_type_result = s21_mul(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000110;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_mul_5) {
  // 3 * 0 = 0
  s21_decimal src1, src2, origin, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_mul(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_mul_6) {
  // ошибка 1
  s21_decimal src1, src2, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  setScale(&src1, 28);
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_mul(src1, src2, &result);
  value_type_origin = 1;
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_mul_7) {
  // ошибка 2
  s21_decimal src1, src2, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  setScale(&src1, 28);
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_mul(src1, src2, &result);
  value_type_origin = 2;
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST
// s21_div
// START_TEST(test_s21_div_1) {
//   // 3 / 2 = 1.5
//   s21_decimal src1, src2, origin, result;
//   int value_type_result, value_type_origin;
//   src1.bits[0] = 0b00000000000000000000000000000011;
//   src1.bits[1] = 0b00000000000000000000000000000000;
//   src1.bits[2] = 0b00000000000000000000000000000000;
//   src1.bits[3] = 0b00000000000000000000000000000000;
//   src2.bits[0] = 0b00000000000000000000000000000010;
//   src2.bits[1] = 0b00000000000000000000000000000000;
//   src2.bits[2] = 0b00000000000000000000000000000000;
//   src2.bits[3] = 0b00000000000000000000000000000000;
//   value_type_result = s21_div(src1, src2, &result);
//   value_type_origin = 0;
//   origin.bits[0] = 0b00000000000000000000000000001111;
//   origin.bits[1] = 0b00000000000000000000000000000000;
//   origin.bits[2] = 0b00000000000000000000000000000000;
//   origin.bits[3] = 0b00000000000000000000000000000000;
//   setScale(&origin, 1);
//   ck_assert_int_eq(origin.bits[3], result.bits[3]);
//   ck_assert_int_eq(origin.bits[2], result.bits[2]);
//   ck_assert_int_eq(origin.bits[1], result.bits[1]);
//   ck_assert_int_eq(origin.bits[0], result.bits[0]);
//   ck_assert_int_eq(value_type_result, value_type_origin);
// }
// END_TEST

// START_TEST(test_s21_div_2) {
//   // 2 / 3
//   s21_decimal src1, src2, origin, result;
//   int value_type_result, value_type_origin;
//   src1.bits[0] = 0b00000000000000000000000000000010;
//   src1.bits[1] = 0b00000000000000000000000000000000;
//   src1.bits[2] = 0b00000000000000000000000000000000;
//   src1.bits[3] = 0b00000000000000000000000000000000;
//   src2.bits[0] = 0b00000000000000000000000000000011;
//   src2.bits[1] = 0b00000000000000000000000000000000;
//   src2.bits[2] = 0b00000000000000000000000000000000;
//   src2.bits[3] = 0b00000000000000000000000000000000;
//   value_type_result = s21_div(src1, src2, &result);
//   value_type_origin = 0;
//   origin.bits[0] = 0b00001010101010101010101010101011;
//   origin.bits[1] = 0b00101001011011100000000110010110;
//   origin.bits[2] = 0b00010101100010101000100110010100;
//   origin.bits[3] = 0b00000000000000000000000000000000;
//   setScale(&origin, 28);
//   ck_assert_int_eq(origin.bits[3], result.bits[3]);
//   ck_assert_int_eq(origin.bits[2], result.bits[2]);
//   ck_assert_int_eq(origin.bits[1], result.bits[1]);
//   ck_assert_int_eq(origin.bits[0], result.bits[0]);
//   ck_assert_int_eq(value_type_result, value_type_origin);
// }
// END_TEST

// START_TEST(test_s21_div_3) {
//   // -3 / 2 = -1.5
//   s21_decimal src1, src2, origin, result;
//   int value_type_result, value_type_origin;
//   src1.bits[0] = 0b00000000000000000000000000000011;
//   src1.bits[1] = 0b00000000000000000000000000000000;
//   src1.bits[2] = 0b00000000000000000000000000000000;
//   src1.bits[3] = 0b10000000000000000000000000000000;
//   src2.bits[0] = 0b00000000000000000000000000000010;
//   src2.bits[1] = 0b00000000000000000000000000000000;
//   src2.bits[2] = 0b00000000000000000000000000000000;
//   src2.bits[3] = 0b00000000000000000000000000000000;
//   value_type_result = s21_div(src1, src2, &result);
//   value_type_origin = 0;
//   origin.bits[0] = 0b00000000000000000000000000001111;
//   origin.bits[1] = 0b00000000000000000000000000000000;
//   origin.bits[2] = 0b00000000000000000000000000000000;
//   origin.bits[3] = 0b10000000000000000000000000000000;
//   setScale(&origin, 1);
//   ck_assert_int_eq(origin.bits[3], result.bits[3]);
//   ck_assert_int_eq(origin.bits[2], result.bits[2]);
//   ck_assert_int_eq(origin.bits[1], result.bits[1]);
//   ck_assert_int_eq(origin.bits[0], result.bits[0]);
//   ck_assert_int_eq(value_type_result, value_type_origin);
// }
// END_TEST

// START_TEST(test_s21_div_4) {
//   // 3 / -2 = -1.5
//   s21_decimal src1, src2, origin, result;
//   int value_type_result, value_type_origin;
//   src1.bits[0] = 0b00000000000000000000000000000011;
//   src1.bits[1] = 0b00000000000000000000000000000000;
//   src1.bits[2] = 0b00000000000000000000000000000000;
//   src1.bits[3] = 0b00000000000000000000000000000000;
//   src2.bits[0] = 0b00000000000000000000000000000010;
//   src2.bits[1] = 0b00000000000000000000000000000000;
//   src2.bits[2] = 0b00000000000000000000000000000000;
//   src2.bits[3] = 0b10000000000000000000000000000000;
//   value_type_result = s21_div(src1, src2, &result);
//   value_type_origin = 0;
//   origin.bits[0] = 0b00000000000000000000000000001111;
//   origin.bits[1] = 0b00000000000000000000000000000000;
//   origin.bits[2] = 0b00000000000000000000000000000000;
//   origin.bits[3] = 0b10000000000000000000000000000000;
//   setScale(&origin, 1);
//   ck_assert_int_eq(origin.bits[3], result.bits[3]);
//   ck_assert_int_eq(origin.bits[2], result.bits[2]);
//   ck_assert_int_eq(origin.bits[1], result.bits[1]);
//   ck_assert_int_eq(origin.bits[0], result.bits[0]);
//   ck_assert_int_eq(value_type_result, value_type_origin);
// }
// END_TEST

// START_TEST(test_s21_div_5) {
//   // -3 / -2 = 1.5
//   s21_decimal src1, src2, origin, result;
//   int value_type_result, value_type_origin;
//   src1.bits[0] = 0b00000000000000000000000000000011;
//   src1.bits[1] = 0b00000000000000000000000000000000;
//   src1.bits[2] = 0b00000000000000000000000000000000;
//   src1.bits[3] = 0b10000000000000000000000000000000;
//   src2.bits[0] = 0b00000000000000000000000000000010;
//   src2.bits[1] = 0b00000000000000000000000000000000;
//   src2.bits[2] = 0b00000000000000000000000000000000;
//   src2.bits[3] = 0b10000000000000000000000000000000;
//   value_type_result = s21_div(src1, src2, &result);
//   value_type_origin = 0;
//   origin.bits[0] = 0b00000000000000000000000000001111;
//   origin.bits[1] = 0b00000000000000000000000000000000;
//   origin.bits[2] = 0b00000000000000000000000000000000;
//   origin.bits[3] = 0b00000000000000000000000000000000;
//   setScale(&origin, 1);
//   ck_assert_int_eq(origin.bits[3], result.bits[3]);
//   ck_assert_int_eq(origin.bits[2], result.bits[2]);
//   ck_assert_int_eq(origin.bits[1], result.bits[1]);
//   ck_assert_int_eq(origin.bits[0], result.bits[0]);
//   ck_assert_int_eq(value_type_result, value_type_origin);
// }
// END_TEST

// START_TEST(test_s21_div_6) {
//   // -3 / 0 = ошибка 3
//   s21_decimal src1, src2, result;
//   int value_type_result, value_type_origin;
//   src1.bits[0] = 0b00000000000000000000000000000011;
//   src1.bits[1] = 0b00000000000000000000000000000000;
//   src1.bits[2] = 0b00000000000000000000000000000000;
//   src1.bits[3] = 0b10000000000000000000000000000000;
//   src2.bits[0] = 0b00000000000000000000000000000000;
//   src2.bits[1] = 0b00000000000000000000000000000000;
//   src2.bits[2] = 0b00000000000000000000000000000000;
//   src2.bits[3] = 0b00000000000000000000000000000000;
//   value_type_result = s21_div(src1, src2, &result);
//   value_type_origin = 3;
//   ck_assert_int_eq(value_type_result, value_type_origin);
// }
// END_TEST

// START_TEST(test_div_manual1) {
//   // 1.2640938749860586450804312205
//   s21_decimal decimal1 = {{0x1888888D, 0xBE250261, 0x28D856E6, 0x1C0000}};
//   // 2
//   s21_decimal decimal2 = {{0x2, 0x0, 0x0, 0x0}};
//   // 0.6320469374930293225402156102
//   s21_decimal check = {{0x8C444446, 0x5F128130, 0x146C2B73, 0x1C0000}};

//   test_div(decimal1, decimal2, check);
// }

// START_TEST(test_div_manual2) {
//   // 2
//   s21_decimal decimal1 = {{0x2, 0x0, 0x0, 0x0}};
//   // 1.2640938749860586450804312205
//   s21_decimal decimal2 = {{0x1888888D, 0xBE250261, 0x28D856E6, 0x1C0000}};
//   // 1.5821609767882606564463392905
//   s21_decimal check = {{0x8B80B889, 0x20B8279E, 0x331F5430, 0x1C0000}};

//   test_div(decimal1, decimal2, check);
// }

// START_TEST(test_div_manual3) {
//   // 1431655765
//   s21_decimal decimal1 = {{0x55555555, 0x0, 0x0, 0x0}};
//   // 12.640938749860586450805
//   s21_decimal decimal2 = {{0x2EB3EF75, 0x4444026F, 0x2AD, 0x150000}};
//   // 113255494.17884722765619814067
//   s21_decimal check = {{0x6D7C5EB3, 0x2C521AF0, 0x24984660, 0x140000}};

//   test_div(decimal1, decimal2, check);
// }

// START_TEST(test_div_manual4) {
//   // 1
//   s21_decimal decimal1 = {{0x1, 0x0, 0x0, 0x0}};
//   // 500
//   s21_decimal decimal2 = {{0x1F4, 0x0, 0x0, 0x0}};
//   // 0.002
//   s21_decimal check = {{0x2, 0x0, 0x0, 0x30000}};

//   test_div(decimal1, decimal2, check);
// }

START_TEST(test_div_manual5) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 7.9228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // 10000000000000000000000000000
  s21_decimal check = {{0x10000000, 0x3E250261, 0x204FCE5E, 0x0}};

  test_div(decimal1, decimal2, check);
}

// START_TEST(test_div_manual6) {
//   // 7.9228162514264337593543950335
//   s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
//   // 79228162514264337593543950335
//   s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
//   // 0.0000000000000000000000000001
//   s21_decimal check = {{0x1, 0x0, 0x0, 0x1C0000}};

//   test_div(decimal1, decimal2, check);
// }

// START_TEST(test_div_manual7) {
//   // 79228162514264337593543950335
//   s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
//   // -26409387504754779197847983445
//   s21_decimal decimal2 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
//   // -3
//   s21_decimal check = {{0x3, 0x0, 0x0, 0x80000000}};

//   test_div(decimal1, decimal2, check);
// }

// START_TEST(test_div_manual8) {
//   // -26409387504754779197847983445
//   s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
//   // 156064767525876035022225408
//   s21_decimal decimal2 = {{0x0, 0x0, 0x811800, 0x0}};
//   // -169.22068909868474138626644073
//   s21_decimal check = {{0x8C085869, 0xC05E68BD, 0x36AD9B79, 0x801A0000}};

//   test_div(decimal1, decimal2, check);
// }

START_TEST(test_div_manual9) {
  // -26409387504754779197847983445
  s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
  // -429496729.5
  s21_decimal decimal2 = {{0xFFFFFFFF, 0x0, 0x0, 0x80010000}};
  // 61489146926681729710
  s21_decimal check = {{0xAAAAAAAE, 0x55555558, 0x3, 0x0}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual10) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // 1.0000000000000000000000000001
  s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // -79228162514264337593543950327
  s21_decimal check = {{0xFFFFFFF7, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual11) {
  // 1.2640938749860586450804312207
  s21_decimal decimal1 = {{0x1888888F, 0xBE250261, 0x28D856E6, 0x1C0000}};
  // 2.0000000000000000000000000000
  s21_decimal decimal2 = {{0x20000000, 0x7C4A04C2, 0x409F9CBC, 0x1C0000}};
  // 0.6320469374930293225402156104
  s21_decimal check = {{0x8C444448, 0x5F128130, 0x146C2B73, 0x1C0000}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual12) {
  // 3.6336660283201536
  s21_decimal decimal1 = {{0x811800, 0x811800, 0x0, 0x100000}};
  // 0.0000000000000000000000000001
  s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x1C0000}};
  // 36336660283201536000000000000
  s21_decimal check = {{0x89800000, 0xFEE8FB1C, 0x7568FB1C, 0x0}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual13) {
  // 7.9228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // 0.0000000000000000000000000001
  s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x1C0000}};
  // 79228162514264337593543950335
  s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual14) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0
  s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x80000000}};
  // division by zero
  int code_check = TEST_ARITHMETIC_ZERO_DIV;

  test_div_fail1(decimal1, decimal2, code_check);
}

START_TEST(test_div_manual18) {
  // 0.0000000074505801528346182749
  s21_decimal decimal1 = {{0xCAD4845D, 0x9F9C7BD, 0x4, 0x1C0000}};
  // 1152921504606846976
  s21_decimal decimal2 = {{0x0, 0x10000000, 0x0, 0x0}};
  // 0.0000000000000000000000000065
  s21_decimal decimal_check = {{0x41, 0x0, 0x0, 0x1C0000}};

  test_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div_manual19) {
  // 0.0000000000000000000542101054
  s21_decimal decimal1 = {{0x204FCE3E, 0x0, 0x0, 0x1C0000}};
  // 8388608
  s21_decimal decimal2 = {{0x800000, 0x0, 0x0, 0x0}};
  // 0.0000000000000000000000000065
  s21_decimal decimal_check = {{0x41, 0x0, 0x0, 0x1C0000}};

  test_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div_manual20) {
  // 8388608
  s21_decimal decimal1 = {{0x800000, 0x0, 0x0, 0x0}};
  // 0.0000000000000000000542101054
  s21_decimal decimal2 = {{0x204FCE3E, 0x0, 0x0, 0x1C0000}};
  // 154742514114351823414827745.38
  s21_decimal decimal_check = {{0x6306800A, 0xE4ABBB05, 0x32000031, 0x20000}};

  test_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div_manual22) {
  // 8388608
  s21_decimal decimal1 = {{0x800000, 0x0, 0x0, 0x0}};
  // 0.0000000000000000000020971522
  s21_decimal decimal2 = {{0x1400002, 0x0, 0x0, 0x1C0000}};
  // 3999999618530309817284601470.5
  s21_decimal decimal_check = {{0x695280F1, 0x2D062C6F, 0x813F38AA, 0x10000}};

  test_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div_manual24) {
  // 79228162514264337593543950320
  s21_decimal decimal1 = {{0xFFFFFFF0, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.9999999999999999999999999998
  s21_decimal decimal2 = {{0xFFFFFFE, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_BIG;

  test_div_fail1(decimal1, decimal2, code_check);
}

START_TEST(test_div_manual25) {
  // -79228162514264337593543950320
  s21_decimal decimal1 = {{0xFFFFFFF0, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // 0.9999999999999999999999999998
  s21_decimal decimal2 = {{0xFFFFFFE, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_SMALL;

  test_div_fail1(decimal1, decimal2, code_check);
}

START_TEST(test_div_manual26) {
  // 79228162514264337593543950319
  s21_decimal decimal1 = {{0xFFFFFFEF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.9999999999999999999999999998
  s21_decimal decimal2 = {{0xFFFFFFE, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // 79228162514264337593543950335
  s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  test_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div_manual27) {
  // 79228162514264337593543950319
  s21_decimal decimal1 = {{0xFFFFFFEF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0.9999999999999999999999999998
  s21_decimal decimal2 = {{0xFFFFFFE, 0x3E250261, 0x204FCE5E, 0x801C0000}};
  // -79228162514264337593543950335
  s21_decimal decimal_check = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  test_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_from_decimal_to_float_fail_manual1) {
  // 792281625.14264337593543950335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
  int code = s21_from_decimal_to_float(decimal, NULL);

  ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_fail_manual2) {
  // степень 154 (показатель степени должен быть от 0 до 28)
  // биты 0-15 не нули
  // биты 24-30 не нули
  s21_decimal decimal = {{0, 0, 0, 1000000000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_fail_manual3) {
  // степень 29 (показатель степени должен быть от 0 до 28)
  s21_decimal decimal = {{-1, 0, 0, 0x1D0000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_fail_manual4) {
  // степень 29 (показатель степени должен быть от 0 до 28)
  s21_decimal decimal = {{0, 0, 0, 0x1D0000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_fail_manual5) {
  // степень 28 (что корректно), но биты 0-15 не нули (младший бит)
  s21_decimal decimal = {{-1, 0, 0, 0x1C0001}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_fail_manual6) {
  // степень 28 (что корректно), но биты 0-15 не нули (старший бит)
  s21_decimal decimal = {{-1, 0, 0, 0x1C8000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_fail_manual7) {
  // степень 28 (что корректно), но биты 24-30 не нули (младший бит)
  s21_decimal decimal = {{-1, 0, 0, 0x11C0000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_fail_manual8) {
  // степень 28 (что корректно), но биты 24-30 не нули (старший бит)
  s21_decimal decimal = {{-1, 0, 0, 0x401C0000}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_float_fail_manual9) {
  // Просто все единицы
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);

  ck_assert_int_eq(code, TEST_CONVERSION_ERROR);
}
END_TEST

Suite *s21_decimal_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_decimal");
  tc_core = tcase_create("Core");
  // s21_is_less
  tcase_add_test(tc_core, test_s21_is_less_1);
  tcase_add_test(tc_core, test_s21_is_less_2);
  tcase_add_test(tc_core, test_s21_is_less_3);
  tcase_add_test(tc_core, test_s21_is_less_4);
  tcase_add_test(tc_core, test_s21_is_less_5);
  tcase_add_test(tc_core, test_s21_is_less_6);
  // s21_is_less_or_equal
  tcase_add_test(tc_core, test_s21_is_less_or_equal_1);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_2);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_3);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_4);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_5);
  // s21_is_greater
  tcase_add_test(tc_core, test_s21_is_greater_1);
  tcase_add_test(tc_core, test_s21_is_greater_2);
  tcase_add_test(tc_core, test_s21_is_greater_3);
  tcase_add_test(tc_core, test_s21_is_greater_4);
  tcase_add_test(tc_core, test_s21_is_greater_5);
  // s21_is_greater_or_equal
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_1);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_2);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_3);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_4);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_5);
  // s21_is_equal
  tcase_add_test(tc_core, test_s21_is_equal_1);
  tcase_add_test(tc_core, test_s21_is_equal_2);
  tcase_add_test(tc_core, test_s21_is_equal_3);
  tcase_add_test(tc_core, test_s21_is_equal_4);
  tcase_add_test(tc_core, test_s21_is_equal_5);
  tcase_add_test(tc_core, test_s21_is_equal_6);
  // s21_is_not_equal
  tcase_add_test(tc_core, test_s21_is_not_equal_1);
  tcase_add_test(tc_core, test_s21_is_not_equal_2);
  tcase_add_test(tc_core, test_s21_is_not_equal_3);
  tcase_add_test(tc_core, test_s21_is_not_equal_4);
  tcase_add_test(tc_core, test_s21_is_not_equal_5);
  // s21_from_int_to_decimal
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_1);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_2);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_3);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_4);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_5);
  // s21_from_float_to_decimal
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_1);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_2);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_3);
  // s21_from_decimal_to_int
  tcase_add_test(tc_core, test_s21_from_decimal_to_int_1);
  // s21_from_decimal_to_float
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_1);
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_2);
  // s21_floor
  tcase_add_test(tc_core, test_s21_floor_1);
  tcase_add_test(tc_core, test_s21_floor_2);
  // s21_round
  tcase_add_test(tc_core, test_s21_round_1);
  // s21_truncate
  tcase_add_test(tc_core, test_s21_truncate_1);
  // s21_negate
  tcase_add_test(tc_core, test_s21_negate_1);
  // s21_add
  tcase_add_test(tc_core, test_s21_add_1);  // +2 + +3
  tcase_add_test(tc_core, test_s21_add_2);  // +2 + -3
  tcase_add_test(tc_core, test_s21_add_3);  // -2 + +3
  tcase_add_test(tc_core, test_s21_add_4);  // -2 + -3
  tcase_add_test(tc_core, test_s21_add_5);  // +3 + -2
  tcase_add_test(tc_core, test_s21_add_6);  // -3 + 2
  tcase_add_test(tc_core, test_s21_add_7);  // ошибка 1
  tcase_add_test(tc_core, test_s21_add_8);  // ошибка 2
  // s21_sub
  tcase_add_test(tc_core, test_s21_sub_1);  // 3 - 2
  tcase_add_test(tc_core, test_s21_sub_2);  // -3 - 2
  tcase_add_test(tc_core, test_s21_sub_3);  // 3 - -2
  tcase_add_test(tc_core, test_s21_sub_4);  // -3 - -2
  tcase_add_test(tc_core, test_s21_sub_5);  // 2 - 3
  tcase_add_test(tc_core, test_s21_sub_6);  // -2 - 3
  tcase_add_test(tc_core, test_s21_sub_7);  // 2 - -3
  tcase_add_test(tc_core, test_s21_sub_8);  // -2 - -3
  tcase_add_test(tc_core, test_s21_sub_9);
  tcase_add_test(tc_core, test_s21_sub_10);
  // s21_mul
  tcase_add_test(tc_core, test_s21_mul_1);  // 3 * 2
  tcase_add_test(tc_core, test_s21_mul_2);  // -3 * 2
  tcase_add_test(tc_core, test_s21_mul_3);  // 3 * -2
  tcase_add_test(tc_core, test_s21_mul_4);  // -3 * -2
  tcase_add_test(tc_core, test_s21_mul_5);  // 3 * 0
  tcase_add_test(tc_core, test_s21_mul_6);
  tcase_add_test(tc_core, test_s21_mul_7);

  // s21_div
  //   tcase_add_test(tc_core, test_s21_div_1);  // 3 / 2
  //   tcase_add_test(tc_core, test_s21_div_2);  // 2 / 3
  //   tcase_add_test(tc_core, test_s21_div_3);  // -3 / 2
  //   tcase_add_test(tc_core, test_s21_div_4);  // 3 / -2
  //   tcase_add_test(tc_core, test_s21_div_5);  // -3 / -2
  //   tcase_add_test(tc_core, test_s21_div_6);  // -3 / 0 = ошибка 3
  //   tcase_add_test(tc_core, test_div_manual1);
  //   tcase_add_test(tc_core, test_div_manual2);
  //   tcase_add_test(tc_core, test_div_manual3);
  //   tcase_add_test(tc_core, test_div_manual4);
  tcase_add_test(tc_core, test_div_manual5);
  //   tcase_add_test(tc_core, test_div_manual6);
  //   tcase_add_test(tc_core, test_div_manual7);
  //   tcase_add_test(tc_core, test_div_manual8);
  tcase_add_test(tc_core, test_div_manual9);
  tcase_add_test(tc_core, test_div_manual10);
  tcase_add_test(tc_core, test_div_manual11);
  tcase_add_test(tc_core, test_div_manual12);
  tcase_add_test(tc_core, test_div_manual13);
  tcase_add_test(tc_core, test_div_manual14);
  tcase_add_test(tc_core, test_div_manual18);
  tcase_add_test(tc_core, test_div_manual19);
  tcase_add_test(tc_core, test_div_manual20);
  tcase_add_test(tc_core, test_div_manual22);
  tcase_add_test(tc_core, test_div_manual24);
  tcase_add_test(tc_core, test_div_manual25);
  tcase_add_test(tc_core, test_div_manual26);
  tcase_add_test(tc_core, test_div_manual27);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail_manual1);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail_manual2);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail_manual3);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail_manual4);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail_manual5);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail_manual6);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail_manual7);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail_manual8);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail_manual9);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = s21_decimal_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
}