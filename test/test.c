#include <check.h>
#include <stdio.h>

#include "../s21_decimal.h"

#define __DEBUG 1

void test_floor(s21_decimal decimal, s21_decimal decimal_check);
void test_negate(s21_decimal decimal, s21_decimal decimal_check);
void test_round(s21_decimal decimal, s21_decimal decimal_check);
void test_result(s21_decimal origin, s21_decimal result, int value_type_result,
                 int value_type_origin);

static void test_div(s21_decimal a, s21_decimal b, s21_decimal expected) {
  s21_decimal res;
  int rc = s21_div(a, b, &res);

  test_result(expected, res, 0, rc);
}

static void test_div_fail1(s21_decimal a, s21_decimal b, int expected_code) {
  s21_decimal dummy;
  int rc = s21_div(a, b, &dummy);
  ck_assert_int_eq(rc, expected_code);
}

s21_decimal case_min = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
s21_decimal case1 = {{0, 0, 0, 0}};
s21_decimal case2 = {{1, 0, 0, 0}};

START_TEST(test_convert_int_1) {
  int int_min = -2147483647;
  int min1 = 0;
  s21_decimal casemin = {{0, 0, 0, 0}};
  s21_from_int_to_decimal(int_min, &casemin);
  s21_from_decimal_to_int(casemin, &min1);
  ck_assert_int_eq(int_min, min1);
}
END_TEST

START_TEST(test_convert_int_2) {
  int int_max = 2147483647;
  int max1 = 0;
  s21_decimal casemin1 = {{0, 0, 0, 0}};
  s21_from_int_to_decimal(int_max, &casemin1);
  s21_from_decimal_to_int(casemin1, &max1);
  ck_assert_int_eq(int_max, max1);
}
END_TEST

START_TEST(test_convert_int_3) {
  int zero = 0;
  int zero1 = 0;
  s21_decimal case_2 = {{0, 0, 0, 0}};
  s21_from_int_to_decimal(zero, &case_2);
  s21_from_decimal_to_int(case_2, &zero1);
  ck_assert_int_eq(zero, zero1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_1) {
  float min = -0.1;
  s21_decimal test = {{1, 0, 0, 0x80010000}};
  s21_from_float_to_decimal(min, &case1);
  int res = s21_is_equal(case1, test);
  ck_assert_int_eq(1, res);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_2) {
  float min1 = -0.5;
  s21_decimal test1 = {{5000000, 0, 0, 0x80070000}};
  s21_from_float_to_decimal(min1, &case1);
  int res1 = s21_is_equal(case1, test1);
  ck_assert_int_eq(1, res1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_3) {
  float min2 = -0.1234567;
  s21_decimal test2 = {{0x12D687, 0, 0, 0x80070000}};
  s21_from_float_to_decimal(min2, &case1);

  ck_assert_int_eq(1, s21_is_equal(case1, test2));
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_4) {
  float min3 = 0.0;
  s21_decimal test3 = {{0, 0, 0, 0}};
  s21_from_float_to_decimal(min3, &case1);
  int res3 = s21_is_equal(case1, test3);
  ck_assert_int_eq(1, res3);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_1) {
  float src = 0.0001;
  float temp = 0;
  s21_decimal value = {{1, 0, 0, 0b00000000000001000000000000000000}};
  s21_from_decimal_to_float(value, &temp);
  ck_assert_float_eq_tol(src, temp, 10e-7);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_2) {
  float src = 0.0000000000000123;
  float temp = 0;
  s21_decimal value = {{123, 0, 0, 0b00000000000100000000000000000000}};
  s21_from_decimal_to_float(value, &temp);
  ck_assert_float_eq_tol(src, temp, 10e-7);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_3) {
  float src = -0.0001;
  float temp = 0;
  s21_decimal value = {{1, 0, 0, 0b10000000000001000000000000000000}};
  s21_from_decimal_to_float(value, &temp);
  ck_assert_float_eq_tol(src, temp, 10e-7);
}
END_TEST

//////// s21_is_equal
START_TEST(test_is_equal_zero_vs_zero) {
  s21_decimal dec1, dec2;
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_equal(dec1, dec2), 1);
}
END_TEST

START_TEST(test_is_equal_plus_zero_vs_minus_zero) {
  s21_decimal plus_zero, minus_zero;
  s21_from_int_to_decimal(0, &plus_zero);
  s21_from_int_to_decimal(0, &minus_zero);
  minus_zero.bits[3] |= 1u << 31;
  ck_assert_int_eq(s21_is_equal(plus_zero, minus_zero), 1);
}
END_TEST

START_TEST(test_is_equal_max_vs_max) {
  s21_decimal max_val_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal max_val_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  ck_assert_int_eq(s21_is_equal(max_val_1, max_val_2), 1);
}
END_TEST

START_TEST(test_is_equal_min_vs_min) {
  s21_decimal min_val_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal min_val_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(min_val_1, min_val_2), 1);
}
END_TEST

START_TEST(test_is_equal_max_vs_min) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(max_val, min_val), 0);
}
END_TEST

START_TEST(test_is_equal_same_value_different_scale) {
  s21_decimal dec_scaled_1, dec_scaled_2;
  s21_from_float_to_decimal(123.45f, &dec_scaled_1);
  s21_from_float_to_decimal(123.45f, &dec_scaled_2);

  // Увеличим scale второго числа на 1
  dec_scaled_2.bits[3] += (1 << 16);

  // Умножим само число на 10
  unsigned long long carry = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long part =
        (unsigned long long)dec_scaled_2.bits[i] * 10ULL + carry;
    dec_scaled_2.bits[i] = (unsigned int)(part & 0xFFFFFFFFu);
    carry = part >> 32;
  }

  ck_assert_int_eq(s21_is_equal(dec_scaled_1, dec_scaled_2), 1);
}
END_TEST

////////s21_is_greater_or_equal
START_TEST(test_s21_is_ge_zero_zero) {
  s21_decimal dec_zero_1, dec_zero_2;
  s21_from_int_to_decimal(0, &dec_zero_1);
  s21_from_int_to_decimal(0, &dec_zero_2);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_zero_1, dec_zero_2), 1);
}
END_TEST

START_TEST(test_s21_is_ge_plus_zero_vs_minus_zero) {
  s21_decimal plus_zero, minus_zero;
  s21_from_int_to_decimal(0, &plus_zero);
  s21_from_int_to_decimal(0, &minus_zero);
  minus_zero.bits[3] |= (1u << 31);
  ck_assert_int_eq(s21_is_greater_or_equal(plus_zero, minus_zero), 1);
}
END_TEST

START_TEST(test_s21_is_ge_pos_vs_pos_true) {
  s21_decimal dec_a, dec_b;
  s21_from_int_to_decimal(5, &dec_a);
  s21_from_int_to_decimal(3, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 1);
}
END_TEST

START_TEST(test_s21_is_ge_pos_vs_pos_false) {
  s21_decimal dec_a, dec_b;
  s21_from_int_to_decimal(3, &dec_a);
  s21_from_int_to_decimal(5, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 0);
}
END_TEST

START_TEST(test_s21_is_ge_equal_pos) {
  s21_decimal dec_a, dec_b;
  s21_from_int_to_decimal(5, &dec_a);
  s21_from_int_to_decimal(5, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 1);
}
END_TEST

START_TEST(test_s21_is_ge_neg_vs_neg_true) {
  s21_decimal dec_a, dec_b;
  s21_from_int_to_decimal(-5, &dec_a);
  s21_from_int_to_decimal(-6, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 1);
}
END_TEST

START_TEST(test_s21_is_ge_neg_vs_neg_false) {
  s21_decimal dec_a, dec_b;
  s21_from_int_to_decimal(-7, &dec_a);
  s21_from_int_to_decimal(-2, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 0);
}
END_TEST

START_TEST(test_s21_is_ge_equal_neg) {
  s21_decimal dec_a, dec_b;
  s21_from_int_to_decimal(-5, &dec_a);
  s21_from_int_to_decimal(-5, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 1);
}
END_TEST

START_TEST(test_s21_is_ge_pos_neg) {
  s21_decimal dec_a, dec_b;
  s21_from_int_to_decimal(10, &dec_a);
  s21_from_int_to_decimal(-10, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 1);
}
END_TEST

START_TEST(test_s21_is_ge_neg_pos) {
  s21_decimal dec_a, dec_b;
  s21_from_int_to_decimal(-10, &dec_a);
  s21_from_int_to_decimal(10, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 0);
}
END_TEST

START_TEST(test_s21_is_ge_same_value_diff_scale) {
  s21_decimal dec_a, dec_b;
  s21_from_float_to_decimal(123.45f, &dec_a);

  float fval = 123.450f;
  s21_from_float_to_decimal(fval, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 1);
}
END_TEST

START_TEST(test_s21_is_ge_small_decimals) {
  s21_decimal dec_a, dec_b;
  s21_from_float_to_decimal(1.000001f, &dec_a);
  s21_from_float_to_decimal(1.00001f, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 0);
}
END_TEST

START_TEST(test_s21_is_ge_small_decimals_reversed) {
  s21_decimal dec_a, dec_b;
  s21_from_float_to_decimal(1.00001f, &dec_a);
  s21_from_float_to_decimal(1.000001f, &dec_b);
  ck_assert_int_eq(s21_is_greater_or_equal(dec_a, dec_b), 1);
}
END_TEST

START_TEST(test_s21_is_ge_max_vs_min) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_greater_or_equal(max_val, min_val), 1);
}
END_TEST

START_TEST(test_s21_is_ge_min_vs_max) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_greater_or_equal(min_val, max_val), 0);
}
END_TEST

////////s21_is_greater
START_TEST(test_s21_is_greater_zero_zero) {
  s21_decimal dec_zero_1, dec_zero_2;
  s21_from_int_to_decimal(0, &dec_zero_1);
  s21_from_int_to_decimal(0, &dec_zero_2);
  ck_assert_int_eq(s21_is_greater(dec_zero_1, dec_zero_2), 0);
}
END_TEST

START_TEST(test_s21_is_greater_plus_zero_vs_minus_zero) {
  s21_decimal plus_zero, minus_zero;
  s21_from_int_to_decimal(0, &plus_zero);
  s21_from_int_to_decimal(0, &minus_zero);
  minus_zero.bits[3] |= 1u << 31;
  ck_assert_int_eq(s21_is_greater(plus_zero, minus_zero), 1);
}
END_TEST

START_TEST(test_s21_is_greater_pos_true) {
  s21_decimal a, b;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(3, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_pos_false) {
  s21_decimal a, b;
  s21_from_int_to_decimal(3, &a);
  s21_from_int_to_decimal(5, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_greater_eq_nums) {
  s21_decimal a, b;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(5, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_greater_neg_true) {
  s21_decimal a, b;
  s21_from_int_to_decimal(-3, &a);
  s21_from_int_to_decimal(-5, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_neg_false) {
  s21_decimal a, b;
  s21_from_int_to_decimal(-5, &a);
  s21_from_int_to_decimal(-3, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_greater_neg_pos) {
  s21_decimal a, b;
  s21_from_int_to_decimal(-5, &a);
  s21_from_int_to_decimal(5, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_greater_pos_neg) {
  s21_decimal a, b;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(-5, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_decimals) {
  s21_decimal a, b;
  s21_from_float_to_decimal(123.45f, &a);
  s21_from_float_to_decimal(123.44f, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_diff_scale_equal_vals) {
  s21_decimal a, b;
  s21_from_float_to_decimal(123.45f, &a);
  s21_from_float_to_decimal(123.450f, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_greater_neg_decimals) {
  s21_decimal a, b;
  s21_from_float_to_decimal(-2.5f, &a);
  s21_from_float_to_decimal(-2.6f, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_s21_is_greater_neg_decimals_false) {
  s21_decimal a, b;
  s21_from_float_to_decimal(-2.6667f, &a);
  s21_from_float_to_decimal(-2.6660f, &b);
  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_s21_is_greater_max_min) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_greater(max_val, min_val), 1);
}
END_TEST

START_TEST(test_s21_is_greater_min_max) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_greater(min_val, max_val), 0);
}
END_TEST

///////s21_is_less_or_equal
START_TEST(test_s21_is_le_zero_zero) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(0, &d1);
  s21_from_int_to_decimal(0, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_le_plus_zero_vs_minus_zero) {
  s21_decimal plus_zero, minus_zero;
  s21_from_int_to_decimal(0, &plus_zero);
  s21_from_int_to_decimal(0, &minus_zero);

  minus_zero.bits[3] |= 1u << 31;
  ck_assert_int_eq(s21_is_less_or_equal(plus_zero, minus_zero), 1);
}
END_TEST

START_TEST(test_s21_is_le_pos_true) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(3, &d1);
  s21_from_int_to_decimal(5, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_le_pos_false) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(5, &d1);
  s21_from_int_to_decimal(3, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_le_eq_val) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(5, &d1);
  s21_from_int_to_decimal(5, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_le_neg_true) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(-5, &d1);
  s21_from_int_to_decimal(-3, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_le_neg_false) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(-3, &d1);
  s21_from_int_to_decimal(-5, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_le_neg_pos) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(-5, &d1);
  s21_from_int_to_decimal(5, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_le_pos_neg) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(5, &d1);
  s21_from_int_to_decimal(-5, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_le_decimals_true) {
  s21_decimal d1, d2;
  s21_from_float_to_decimal(123.44f, &d1);
  s21_from_float_to_decimal(123.45f, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_le_diff_scale_eq_val) {
  s21_decimal d1, d2;
  s21_from_float_to_decimal(123.45f, &d1);
  s21_from_float_to_decimal(123.450f, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_le_neg_decimals_false) {
  s21_decimal d1, d2;
  s21_from_float_to_decimal(-2.5f, &d1);
  s21_from_float_to_decimal(-2.6f, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_le_neg_decimals_true) {
  s21_decimal d1, d2;
  s21_from_float_to_decimal(-2.6f, &d1);
  s21_from_float_to_decimal(-2.5f, &d2);
  ck_assert_int_eq(s21_is_less_or_equal(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_le_max_vs_min) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_less_or_equal(max_val, min_val), 0);
}
END_TEST

START_TEST(test_s21_is_le_min_vs_max) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_less_or_equal(min_val, max_val), 1);
}
END_TEST

////s21_is_less
START_TEST(test_s21_is_less_zero_zero) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(0, &d1);
  s21_from_int_to_decimal(0, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_less_plus_zero_vs_minus_zero) {
  s21_decimal plus_zero, minus_zero;
  s21_from_int_to_decimal(0, &plus_zero);
  s21_from_int_to_decimal(0, &minus_zero);
  minus_zero.bits[3] |= 1u << 31;
  ck_assert_int_eq(s21_is_less(plus_zero, minus_zero), 0);
}
END_TEST

START_TEST(test_s21_is_less_pos_true) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(3, &d1);
  s21_from_int_to_decimal(5, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_less_pos_false) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(5, &d1);
  s21_from_int_to_decimal(3, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_less_eq_vals) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(5, &d1);
  s21_from_int_to_decimal(5, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_less_neg_true) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(-5, &d1);
  s21_from_int_to_decimal(-3, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_less_neg_false) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(-3, &d1);
  s21_from_int_to_decimal(-5, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_less_neg_pos) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(-5, &d1);
  s21_from_int_to_decimal(5, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_less_pos_neg) {
  s21_decimal d1, d2;
  s21_from_int_to_decimal(5, &d1);
  s21_from_int_to_decimal(-5, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_less_decimals_true) {
  s21_decimal d1, d2;
  s21_from_float_to_decimal(123.44f, &d1);
  s21_from_float_to_decimal(123.45f, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_less_diff_scale_eq_val) {
  s21_decimal d1, d2;
  s21_from_float_to_decimal(123.45f, &d1);
  s21_from_float_to_decimal(123.450f, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_less_neg_decimals_false) {
  s21_decimal d1, d2;
  s21_from_float_to_decimal(-2.5f, &d1);
  s21_from_float_to_decimal(-2.6f, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 0);
}
END_TEST

START_TEST(test_s21_is_less_neg_decimals_true) {
  s21_decimal d1, d2;
  s21_from_float_to_decimal(-2.6f, &d1);
  s21_from_float_to_decimal(-2.5f, &d2);
  ck_assert_int_eq(s21_is_less(d1, d2), 1);
}
END_TEST

START_TEST(test_s21_is_less_max_vs_min) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_less(max_val, min_val), 0);
}
END_TEST

START_TEST(test_s21_is_less_min_vs_max) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_less(min_val, max_val), 1);
}
END_TEST

//////is_not_equal

START_TEST(test_is_not_equal_zero_vs_zero) {
  s21_decimal dec1, dec2;
  s21_from_int_to_decimal(0, &dec1);
  s21_from_int_to_decimal(0, &dec2);
  ck_assert_int_eq(s21_is_not_equal(dec1, dec2), 0);
}
END_TEST

START_TEST(test_is_not_equal_plus_zero_vs_minus_zero) {
  s21_decimal plus_zero, minus_zero;
  s21_from_int_to_decimal(0, &plus_zero);
  s21_from_int_to_decimal(0, &minus_zero);
  minus_zero.bits[3] |= (1u << 31);
  ck_assert_int_eq(s21_is_not_equal(plus_zero, minus_zero), 0);
}
END_TEST

START_TEST(test_is_not_equal_max_vs_max) {
  s21_decimal max_val_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal max_val_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  ck_assert_int_eq(s21_is_not_equal(max_val_1, max_val_2), 0);
}
END_TEST

START_TEST(test_is_not_equal_min_vs_min) {
  s21_decimal min_val_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal min_val_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_not_equal(min_val_1, min_val_2), 0);
}
END_TEST

START_TEST(test_is_not_equal_max_vs_min) {
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  s21_decimal min_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  ck_assert_int_eq(s21_is_not_equal(max_val, min_val), 1);
}
END_TEST

START_TEST(test_is_not_equal_same_value_different_scale) {
  s21_decimal dec_scaled_1 = {{0x3039, 0x0, 0x0, 0x80000000}};
  s21_decimal dec_scaled_2 = {{0x3039, 0x0, 0x0, 0x80000000}};
  setScale(&dec_scaled_1, 2);
  setScale(&dec_scaled_2, 2);

  // s21_from_float_to_decimal(123.45f, &dec_scaled_1);
  // s21_from_float_to_decimal(123.45f, &dec_scaled_2);

  dec_scaled_2.bits[3] += (1 << 16);
  unsigned long long carry = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long part =
        (unsigned long long)dec_scaled_2.bits[i] * 10ULL + carry;
    dec_scaled_2.bits[i] = (unsigned int)(part & 0xFFFFFFFFu);
    carry = part >> 32;
  }
  ck_assert_int_eq(s21_is_not_equal(dec_scaled_1, dec_scaled_2), 0);
}
END_TEST

/// multiplication

START_TEST(test_s21_mul_zero_zero) {
  s21_decimal d1, d2, res, expected;
  s21_from_int_to_decimal(0, &d1);
  s21_from_int_to_decimal(0, &d2);
  int code = s21_mul(d1, d2, &res);
  ck_assert_int_eq(code, 0);
  s21_from_int_to_decimal(0, &expected);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_s21_mul_zero_positive) {
  s21_decimal d1, d2, res, expected;
  s21_from_int_to_decimal(0, &d1);
  s21_from_int_to_decimal(12345, &d2);
  int code = s21_mul(d1, d2, &res);
  ck_assert_int_eq(code, 0);
  s21_from_int_to_decimal(0, &expected);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_s21_mul_small_pos) {
  s21_decimal d1, d2, res, expected;
  s21_from_int_to_decimal(12, &d1);
  s21_from_int_to_decimal(3, &d2);
  int code = s21_mul(d1, d2, &res);
  ck_assert_int_eq(code, 0);
  s21_from_int_to_decimal(36, &expected);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_s21_mul_small_neg) {
  s21_decimal d1, d2, res, expected;
  s21_from_int_to_decimal(-5, &d1);
  s21_from_int_to_decimal(4, &d2);
  int code = s21_mul(d1, d2, &res);
  ck_assert_int_eq(code, 0);
  s21_from_int_to_decimal(-20, &expected);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_s21_mul_with_scale_1) {
  s21_decimal d1 = {{0}}, d2 = {{0}}, res = {{0}}, expected = {{0}};

  d1.bits[0] = 0x0000007B;
  d1.bits[3] = 0x00020000;

  d2.bits[0] = 0x00000002;
  d2.bits[3] = 0x00000000;

  int code = s21_mul(d1, d2, &res);
  ck_assert_int_eq(code, 0);

  expected.bits[0] = 0x000000F6;
  expected.bits[3] = 0x00020000;

  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_s21_mul_with_scale_2) {
  s21_decimal d1 = {{0}}, d2 = {{0}}, res = {{0}}, expected = {{0}};

  d1.bits[0] = 0x0000007B;
  d1.bits[3] = 0x00030000;

  d2.bits[0] = 0x00000005;
  d2.bits[3] = 0x00020000;

  int code = s21_mul(d1, d2, &res);
  ck_assert_int_eq(code, 0);

  expected.bits[0] = 0x00000267;
  expected.bits[3] = 0x00050000;

  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(test_s21_mul_pos_overflow) {
  s21_decimal src1, src2, result;
  memset(&src1, 0, sizeof(src1));
  memset(&src2, 0, sizeof(src2));
  memset(&result, 0, sizeof(result));
  src1.bits[0] = 0xFFFFFFFF;
  src1.bits[1] = 0xFFFFFFFF;
  src1.bits[2] = 0xFFFFFFFF;
  src1.bits[3] = 0;
  setScale(&src1, 28);

  src2.bits[0] = 2;
  src2.bits[1] = 0;
  src2.bits[2] = 0;
  src2.bits[3] = 0;

  int code = s21_mul(src1, src2, &result);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(test_s21_mul_neg_overflow) {
  s21_decimal src1, src2, result;
  memset(&src1, 0, sizeof(src1));
  memset(&src2, 0, sizeof(src2));
  memset(&result, 0, sizeof(result));
  src1.bits[0] = 0xFFFFFFFF;
  src1.bits[1] = 0xFFFFFFFF;
  src1.bits[2] = 0xFFFFFFFF;
  src1.bits[3] = 0x80000000;
  setScale(&src1, 28);

  src2.bits[0] = 2;
  src2.bits[1] = 0;
  src2.bits[2] = 0;
  src2.bits[3] = 0;

  int code = s21_mul(src1, src2, &result);
  ck_assert_int_eq(code, 2);
}
END_TEST

START_TEST(test_s21_mul_bank_round) {
  s21_decimal d1, d2, res, expected;
  s21_from_float_to_decimal(9.5f, &d1);
  s21_from_float_to_decimal(2.0f, &d2);
  int code = s21_mul(d1, d2, &res);
  ck_assert_int_eq(code, 0);
  s21_from_float_to_decimal(19.0f, &expected);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
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
// s21_div
START_TEST(test_s21_div_1) {
  // 3 / 2 = 1.5
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
  value_type_result = s21_div(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000001111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  setScale(&origin, 1);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_div_2) {
  // 2 / 3
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
  value_type_result = s21_div(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00001010101010101010101010101011;
  origin.bits[1] = 0b00101001011011100000000110010110;
  origin.bits[2] = 0b00010101100010101000100110010100;
  origin.bits[3] = 0b00000000000000000000000000000000;
  setScale(&origin, 28);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_div_3) {
  // -3 / 2 = -1.5
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

  value_type_result = s21_div(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000001111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  setScale(&origin, 1);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_div_4) {
  // 3 / -2 = -1.5
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
  value_type_result = s21_div(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000001111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  setScale(&origin, 1);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_div_5) {
  // -3 / -2 = 1.5
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
  value_type_result = s21_div(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000001111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  setScale(&origin, 1);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_s21_div_6) {
  // -3 / 0 = ошибка 3
  s21_decimal src1, src2, result;
  int value_type_result, value_type_origin;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_div(src1, src2, &result);
  value_type_origin = 3;
  ck_assert_int_eq(value_type_result, value_type_origin);
}
END_TEST

START_TEST(test_div_manual1) {
  // 1.2640938749860586450804312205
  s21_decimal decimal1 = {{0x1888888D, 0xBE250261, 0x28D856E6, 0x1C0000}};
  // 2
  s21_decimal decimal2 = {{0x2, 0x0, 0x0, 0x0}};
  // 0.6320469374930293225402156102
  s21_decimal check = {{0x8C444446, 0x5F128130, 0x146C2B73, 0x1C0000}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual2) {
  // 2
  s21_decimal decimal1 = {{0x2, 0x0, 0x0, 0x0}};
  // 1.2640938749860586450804312205
  s21_decimal decimal2 = {{0x1888888D, 0xBE250261, 0x28D856E6, 0x1C0000}};
  // 1.5821609767882606564463392905
  s21_decimal check = {{0x8B80B889, 0x20B8279E, 0x331F5430, 0x1C0000}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual3) {
  // 1431655765
  s21_decimal decimal1 = {{0x55555555, 0x0, 0x0, 0x0}};
  // 12.640938749860586450805
  s21_decimal decimal2 = {{0x2EB3EF75, 0x4444026F, 0x2AD, 0x150000}};
  // 113255494.17884722765619814067
  s21_decimal check = {{0x6D7C5EB3, 0x2C521AF0, 0x24984660, 0x140000}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual4) {
  // 1
  s21_decimal decimal1 = {{0x1, 0x0, 0x0, 0x0}};
  // 500
  s21_decimal decimal2 = {{0x1F4, 0x0, 0x0, 0x0}};
  // 0.002
  s21_decimal check = {{0x2, 0x0, 0x0, 0x30000}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual5) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 7.9228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // 10000000000000000000000000000
  s21_decimal check = {{0x10000000, 0x3E250261, 0x204FCE5E, 0x0}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual6) {
  // 7.9228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // 79228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.0000000000000000000000000001
  s21_decimal check = {{0x1, 0x0, 0x0, 0x1C0000}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual7) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -26409387504754779197847983445
  s21_decimal decimal2 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
  // -3
  s21_decimal check = {{0x3, 0x0, 0x0, 0x80000000}};

  test_div(decimal1, decimal2, check);
}

START_TEST(test_div_manual8) {
  // -26409387504754779197847983445
  s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
  // 156064767525876035022225408
  s21_decimal decimal2 = {{0x0, 0x0, 0x811800, 0x0}};
  // -169.22068909868474138626644073
  s21_decimal check = {{0x8C085869, 0xC05E68BD, 0x36AD9B79, 0x801A0000}};

  test_div(decimal1, decimal2, check);
}

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
  int code_check = 3;

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
  int code_check = 1;

  test_div_fail1(decimal1, decimal2, code_check);
}

START_TEST(test_div_manual25) {
  // -79228162514264337593543950320
  s21_decimal decimal1 = {{0xFFFFFFF0, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // 0.9999999999999999999999999998
  s21_decimal decimal2 = {{0xFFFFFFE, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // overflow
  int code_check = 2;

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

/// s21_mul

START_TEST(test_s21_add_positive_numbers) {
  s21_decimal a, b, result, expected;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(10, &b);
  ck_assert_int_eq(s21_add(a, b, &result), 0);

  s21_from_int_to_decimal(15, &expected);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_s21_add_negative_numbers) {
  s21_decimal a, b, result, expected;
  s21_from_int_to_decimal(5, &a);
  s21_from_int_to_decimal(-10, &b);
  ck_assert_int_eq(s21_add(a, b, &result), 0);

  s21_from_int_to_decimal(-5, &expected);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_s21_add_zero) {
  s21_decimal a = {{0}};
  s21_decimal b = {{0}};
  s21_decimal result = {{0}};
  s21_decimal expected = {{0}};

  ck_assert_int_eq(s21_add(a, b, &result), 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_s21_add_overflow) {
  s21_decimal a, b, result;

  a.bits[0] = 0xFFFFFFFF;
  a.bits[1] = 0xFFFFFFFF;
  a.bits[2] = 0xFFFFFFFF;
  a.bits[3] = 0;

  b.bits[0] = 1;
  b.bits[1] = 0;
  b.bits[2] = 0;
  b.bits[3] = 0;

  ck_assert_int_eq(s21_add(a, b, &result), 1);
}
END_TEST

//----sub_div------
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

START_TEST(test_floor_ok1) {
  // 79228162514264337593543950335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 79228162514264337593543950335
  s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_floor_ok2) {
  // -79228162514264337593543950335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // -79228162514264337593543950335
  s21_decimal decimal_check = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_floor_ok3) {
  // 7922816251426433759354395033.5
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
  // 7922816251426433759354395033
  s21_decimal decimal_check = {{0x99999999, 0x99999999, 0x19999999, 0x0}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_floor_ok4) {
  // -7922816251426433759354395033.5
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};
  // -7922816251426433759354395034
  s21_decimal decimal_check = {
      {0x9999999A, 0x99999999, 0x19999999, 0x80000000}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_floor_ok5) {
  // 792281625142643375935439503.35
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x20000}};
  // 792281625142643375935439503
  s21_decimal decimal_check = {{0x28F5C28F, 0xF5C28F5C, 0x28F5C28, 0x0}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_floor_ok6) {
  // -792281625142643375935439503.35
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80020000}};
  // -792281625142643375935439504
  s21_decimal decimal_check = {{0x28F5C290, 0xF5C28F5C, 0x28F5C28, 0x80000000}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_floor_ok7) {
  // 79228162514264337593543950.335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x30000}};
  // 79228162514264337593543950
  s21_decimal decimal_check = {{0x9DB22D0E, 0x4BC6A7EF, 0x418937, 0x0}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_floor_ok8) {
  // -79228162514264337593543950.335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80030000}};
  // -79228162514264337593543951
  s21_decimal decimal_check = {{0x9DB22D0F, 0x4BC6A7EF, 0x418937, 0x80000000}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_floor_ok9) {
  // -792.28162514264337593543950335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x801A0000}};
  // -793
  s21_decimal decimal_check = {{0x319, 0x0, 0x0, 0x80000000}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_floor_ok10) {
  // -7922816251426433759354395.0335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80040000}};
  // -7922816251426433759354396
  s21_decimal decimal_check = {{0x295E9E1C, 0xBAC710CB, 0x68DB8, 0x80000000}};

  test_floor(decimal, decimal_check);
}

START_TEST(test_negate_ok1) {
  // 0
  s21_decimal decimal = {{0x0, 0x0, 0x0, 0x0}};
  // -0
  s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x80000000}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok2) {
  // -0
  s21_decimal decimal_check = {{0x0, 0x0, 0x0, 0x80000000}};
  // 0
  s21_decimal decimal = {{0x0, 0x0, 0x0, 0x0}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok3) {
  // 1
  s21_decimal decimal = {{0x00000001, 0x0, 0x0, 0x0}};
  // -1
  s21_decimal decimal_check = {{0x00000001, 0x0, 0x0, 0x80000000}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok4) {
  // -1
  s21_decimal decimal_check = {{0x00000001, 0x0, 0x0, 0x80000000}};
  // 1
  s21_decimal decimal = {{0x00000001, 0x0, 0x0, 0x0}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok5) {
  // 0x11111111
  s21_decimal decimal = {{0x11111111, 0x0, 0x0, 0x0}};
  // -0x11111111
  s21_decimal decimal_check = {{0x11111111, 0x0, 0x0, 0x80000000}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok6) {
  // -0x11111111
  s21_decimal decimal_check = {{0x11111111, 0x0, 0x0, 0x80000000}};
  // 0x11111111
  s21_decimal decimal = {{0x11111111, 0x0, 0x0, 0x0}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok7) {
  // 0x11111111, 0x00000001
  s21_decimal decimal = {{0x11111111, 0x00000001, 0x0, 0x0}};
  // -0x11111111, 0x00000001
  s21_decimal decimal_check = {{0x11111111, 0x00000001, 0x0, 0x80000000}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok8) {
  // -0x11111111, 0x00000001
  s21_decimal decimal_check = {{0x11111111, 0x00000001, 0x0, 0x80000000}};
  // 0x11111111, 0x00000001
  s21_decimal decimal = {{0x11111111, 0x00000001, 0x0, 0x0}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok9) {
  // 0x11111111, 0x11111111,
  s21_decimal decimal = {{0x11111111, 0x11111111, 0x0, 0x0}};
  // -0x11111111, 0x11111111,
  s21_decimal decimal_check = {{0x11111111, 0x11111111, 0x0, 0x80000000}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_negate_ok10) {
  // -0x11111111, 0x11111111,
  s21_decimal decimal_check = {{0x11111111, 0x11111111, 0x0, 0x80000000}};
  // 0x11111111, 0x11111111,
  s21_decimal decimal = {{0x11111111, 0x11111111, 0x0, 0x0}};

  test_negate(decimal, decimal_check);
}

START_TEST(test_round_ok1) {
  // 79228162514264337593543950335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 79228162514264337593543950335
  s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  test_round(decimal, decimal_check);
}

START_TEST(test_round_ok2) {
  // -79228162514264337593543950335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // -79228162514264337593543950335
  s21_decimal decimal_check = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  test_round(decimal, decimal_check);
}

START_TEST(test_round_ok3) {
  // 7922816251426433759354395033.5
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
  // 7922816251426433759354395034
  s21_decimal decimal_check = {{0x9999999A, 0x99999999, 0x19999999, 0x0}};

  test_round(decimal, decimal_check);
}

START_TEST(test_round_ok4) {
  // -7922816251426433759354395033.5
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};
  // -7922816251426433759354395034
  s21_decimal decimal_check = {
      {0x9999999A, 0x99999999, 0x19999999, 0x80000000}};

  test_round(decimal, decimal_check);
}

START_TEST(test_round_ok5) {
  // 792281625142643375935439503.35
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x20000}};
  // 792281625142643375935439503
  s21_decimal decimal_check = {{0x28F5C28F, 0xF5C28F5C, 0x28F5C28, 0x0}};

  test_round(decimal, decimal_check);
}

START_TEST(test_round_ok6) {
  // -792281625142643375935439503.35
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80020000}};
  // -792281625142643375935439503
  s21_decimal decimal_check = {{0x28F5C28F, 0xF5C28F5C, 0x28F5C28, 0x80000000}};

  test_round(decimal, decimal_check);
}

START_TEST(test_round_ok7) {
  // 79228162514264337593543950.335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x30000}};
  // 79228162514264337593543950
  s21_decimal decimal_check = {{0x9DB22D0E, 0x4BC6A7EF, 0x418937, 0x0}};

  test_round(decimal, decimal_check);
}

START_TEST(test_round_ok8) {
  // -79228162514264337593543950.335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80030000}};
  // -79228162514264337593543950
  s21_decimal decimal_check = {{0x9DB22D0E, 0x4BC6A7EF, 0x418937, 0x80000000}};

  test_round(decimal, decimal_check);
}

START_TEST(test_round_ok9) {
  // 7922816251426433759354395.0335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x40000}};
  // 7922816251426433759354395
  s21_decimal decimal_check = {{0x295E9E1B, 0xBAC710CB, 0x68DB8, 0x0}};

  test_round(decimal, decimal_check);
}

START_TEST(test_round_ok10) {
  // -7922816251426433759354395.0335
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80040000}};
  // -7922816251426433759354395
  s21_decimal decimal_check = {{0x295E9E1B, 0xBAC710CB, 0x68DB8, 0x80000000}};

  test_round(decimal, decimal_check);
}

Suite *s21_decimal_suite(void) {
  Suite *s = suite_create("S21_Decimal");

  TCase *tc_core = tcase_create("Core");
  TCase *tc_fixed1 = tcase_create("fixed_cases_1");
  TCase *tc_fixed2 = tcase_create("fixed_cases_2");
  TCase *tc_fixed3 = tcase_create("fixed_cases_3");
  TCase *tc_fixed4 = tcase_create("fixed_cases_4");
  TCase *tc_fixed5 = tcase_create("fixed_cases_5");
  TCase *tc_fixed6 = tcase_create("fixed_cases_6");
  TCase *tc_mul = tcase_create("mul_tests");
  TCase *tc_trunc = tcase_create("trunc");
  TCase *tc_div = tcase_create("div_tests");
  TCase *tc_add = tcase_create("add_tests");
  TCase *tc_sub = tcase_create("sub_tests");
  TCase *tc_floor = tcase_create("floor_tests");
  TCase *tc_negate = tcase_create("negate_tests");
  TCase *tc_round = tcase_create("round_tests");

  // ---- Core tests ----
  tcase_add_test(tc_core, test_convert_int_1);
  tcase_add_test(tc_core, test_convert_int_2);
  tcase_add_test(tc_core, test_convert_int_3);

  tcase_add_test(tc_core, test_s21_from_float_to_decimal_1);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_2);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_3);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_4);

  tcase_add_test(tc_core, test_s21_from_decimal_to_float_1);
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_2);
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_3);

  // ---- fixed_cases_1 ----
  tcase_add_test(tc_fixed1, test_is_equal_zero_vs_zero);
  tcase_add_test(tc_fixed1, test_is_equal_plus_zero_vs_minus_zero);
  tcase_add_test(tc_fixed1, test_is_equal_max_vs_max);
  tcase_add_test(tc_fixed1, test_is_equal_min_vs_min);
  tcase_add_test(tc_fixed1, test_is_equal_max_vs_min);
  tcase_add_test(tc_fixed1, test_is_equal_same_value_different_scale);

  // ---- fixed_cases_2 ----
  tcase_add_test(tc_fixed2, test_s21_is_ge_zero_zero);
  tcase_add_test(tc_fixed2, test_s21_is_ge_plus_zero_vs_minus_zero);
  tcase_add_test(tc_fixed2, test_s21_is_ge_pos_vs_pos_true);
  tcase_add_test(tc_fixed2, test_s21_is_ge_pos_vs_pos_false);
  tcase_add_test(tc_fixed2, test_s21_is_ge_equal_pos);
  tcase_add_test(tc_fixed2, test_s21_is_ge_neg_vs_neg_true);
  tcase_add_test(tc_fixed2, test_s21_is_ge_neg_vs_neg_false);
  tcase_add_test(tc_fixed2, test_s21_is_ge_equal_neg);
  tcase_add_test(tc_fixed2, test_s21_is_ge_pos_neg);
  tcase_add_test(tc_fixed2, test_s21_is_ge_neg_pos);
  tcase_add_test(tc_fixed2, test_s21_is_ge_same_value_diff_scale);
  tcase_add_test(tc_fixed2, test_s21_is_ge_small_decimals);
  tcase_add_test(tc_fixed2, test_s21_is_ge_small_decimals_reversed);
  tcase_add_test(tc_fixed2, test_s21_is_ge_max_vs_min);
  tcase_add_test(tc_fixed2, test_s21_is_ge_min_vs_max);

  // ---- fixed_cases_3 ----
  tcase_add_test(tc_fixed3, test_s21_is_greater_zero_zero);
  tcase_add_test(tc_fixed3, test_s21_is_greater_plus_zero_vs_minus_zero);
  tcase_add_test(tc_fixed3, test_s21_is_greater_pos_true);
  tcase_add_test(tc_fixed3, test_s21_is_greater_pos_false);
  tcase_add_test(tc_fixed3, test_s21_is_greater_eq_nums);
  tcase_add_test(tc_fixed3, test_s21_is_greater_neg_true);
  tcase_add_test(tc_fixed3, test_s21_is_greater_neg_false);
  tcase_add_test(tc_fixed3, test_s21_is_greater_neg_pos);
  tcase_add_test(tc_fixed3, test_s21_is_greater_pos_neg);
  tcase_add_test(tc_fixed3, test_s21_is_greater_decimals);
  tcase_add_test(tc_fixed3, test_s21_is_greater_diff_scale_equal_vals);
  tcase_add_test(tc_fixed3, test_s21_is_greater_neg_decimals);
  tcase_add_test(tc_fixed3, test_s21_is_greater_neg_decimals_false);
  tcase_add_test(tc_fixed3, test_s21_is_greater_max_min);
  tcase_add_test(tc_fixed3, test_s21_is_greater_min_max);

  // ---- fixed_cases_4 ----
  tcase_add_test(tc_fixed4, test_s21_is_le_zero_zero);
  tcase_add_test(tc_fixed4, test_s21_is_le_plus_zero_vs_minus_zero);
  tcase_add_test(tc_fixed4, test_s21_is_le_pos_true);
  tcase_add_test(tc_fixed4, test_s21_is_le_pos_false);
  tcase_add_test(tc_fixed4, test_s21_is_le_eq_val);
  tcase_add_test(tc_fixed4, test_s21_is_le_neg_true);
  tcase_add_test(tc_fixed4, test_s21_is_le_neg_false);
  tcase_add_test(tc_fixed4, test_s21_is_le_neg_pos);
  tcase_add_test(tc_fixed4, test_s21_is_le_pos_neg);
  tcase_add_test(tc_fixed4, test_s21_is_le_decimals_true);
  tcase_add_test(tc_fixed4, test_s21_is_le_diff_scale_eq_val);
  tcase_add_test(tc_fixed4, test_s21_is_le_neg_decimals_false);
  tcase_add_test(tc_fixed4, test_s21_is_le_neg_decimals_true);
  tcase_add_test(tc_fixed4, test_s21_is_le_max_vs_min);
  tcase_add_test(tc_fixed4, test_s21_is_le_min_vs_max);

  // ---- fixed_cases_5 ----
  tcase_add_test(tc_fixed5, test_s21_is_less_zero_zero);
  tcase_add_test(tc_fixed5, test_s21_is_less_plus_zero_vs_minus_zero);
  tcase_add_test(tc_fixed5, test_s21_is_less_pos_true);
  tcase_add_test(tc_fixed5, test_s21_is_less_pos_false);
  tcase_add_test(tc_fixed5, test_s21_is_less_eq_vals);
  tcase_add_test(tc_fixed5, test_s21_is_less_neg_true);
  tcase_add_test(tc_fixed5, test_s21_is_less_neg_false);
  tcase_add_test(tc_fixed5, test_s21_is_less_neg_pos);
  tcase_add_test(tc_fixed5, test_s21_is_less_pos_neg);
  tcase_add_test(tc_fixed5, test_s21_is_less_decimals_true);
  tcase_add_test(tc_fixed5, test_s21_is_less_diff_scale_eq_val);
  tcase_add_test(tc_fixed5, test_s21_is_less_neg_decimals_false);
  tcase_add_test(tc_fixed5, test_s21_is_less_neg_decimals_true);
  tcase_add_test(tc_fixed5, test_s21_is_less_max_vs_min);
  tcase_add_test(tc_fixed5, test_s21_is_less_min_vs_max);

  // ---- fixed_cases_6 ----
  tcase_add_test(tc_fixed6, test_is_not_equal_zero_vs_zero);
  tcase_add_test(tc_fixed6, test_is_not_equal_plus_zero_vs_minus_zero);
  tcase_add_test(tc_fixed6, test_is_not_equal_max_vs_max);
  tcase_add_test(tc_fixed6, test_is_not_equal_min_vs_min);
  tcase_add_test(tc_fixed6, test_is_not_equal_max_vs_min);
  tcase_add_test(tc_fixed6, test_is_not_equal_same_value_different_scale);

  // ---- mul_tests ----
  tcase_add_test(tc_mul, test_s21_mul_zero_positive);
  tcase_add_test(tc_mul, test_s21_mul_small_pos);
  tcase_add_test(tc_mul, test_s21_mul_small_neg);
  tcase_add_test(tc_mul, test_s21_mul_with_scale_1);
  tcase_add_test(tc_mul, test_s21_mul_with_scale_2);
  tcase_add_test(tc_mul, test_s21_mul_pos_overflow);
  tcase_add_test(tc_mul, test_s21_mul_neg_overflow);
  tcase_add_test(tc_mul, test_s21_mul_bank_round);
  tcase_add_test(tc_mul, test_s21_mul_zero_zero);

  // ---- mul_tests ----

  tcase_add_test(tc_trunc, test_s21_truncate_1);

  // ---- div_tests ----
  tcase_add_test(tc_div, test_s21_div_1);
  tcase_add_test(tc_div, test_s21_div_2);
  tcase_add_test(tc_div, test_s21_div_3);
  tcase_add_test(tc_div, test_s21_div_4);
  tcase_add_test(tc_div, test_s21_div_5);
  tcase_add_test(tc_div, test_s21_div_6);

  tcase_add_test(tc_div, test_div_manual1);
  tcase_add_test(tc_div, test_div_manual2);
  tcase_add_test(tc_div, test_div_manual3);
  tcase_add_test(tc_div, test_div_manual4);
  tcase_add_test(tc_div, test_div_manual5);
  tcase_add_test(tc_div, test_div_manual6);
  tcase_add_test(tc_div, test_div_manual7);
  tcase_add_test(tc_div, test_div_manual8);
  tcase_add_test(tc_div, test_div_manual9);
  tcase_add_test(tc_div, test_div_manual10);
  tcase_add_test(tc_div, test_div_manual11);
  tcase_add_test(tc_div, test_div_manual12);
  tcase_add_test(tc_div, test_div_manual13);
  tcase_add_test(tc_div, test_div_manual14);
  tcase_add_test(tc_div, test_div_manual18);
  tcase_add_test(tc_div, test_div_manual19);
  tcase_add_test(tc_div, test_div_manual20);
  tcase_add_test(tc_div, test_div_manual22);
  tcase_add_test(tc_div, test_div_manual24);
  tcase_add_test(tc_div, test_div_manual25);
  tcase_add_test(tc_div, test_div_manual26);
  tcase_add_test(tc_div, test_div_manual27);

  // ---- add_tests ----
  tcase_add_test(tc_add, test_s21_add_positive_numbers);
  tcase_add_test(tc_add, test_s21_add_negative_numbers);
  tcase_add_test(tc_add, test_s21_add_zero);
  tcase_add_test(tc_add, test_s21_add_overflow);

  // ------sub_test---------

  tcase_add_test(tc_sub, test_s21_sub_1);
  tcase_add_test(tc_sub, test_s21_sub_2);
  tcase_add_test(tc_sub, test_s21_sub_3);
  tcase_add_test(tc_sub, test_s21_sub_4);
  tcase_add_test(tc_sub, test_s21_sub_5);
  tcase_add_test(tc_sub, test_s21_sub_6);
  tcase_add_test(tc_sub, test_s21_sub_7);
  tcase_add_test(tc_sub, test_s21_sub_8);
  tcase_add_test(tc_sub, test_s21_sub_9);
  tcase_add_test(tc_sub, test_s21_sub_10);

  // ------floor_test---------
  tcase_add_test(tc_floor, test_floor_ok1);
  tcase_add_test(tc_floor, test_floor_ok2);
  tcase_add_test(tc_floor, test_floor_ok3);
  tcase_add_test(tc_floor, test_floor_ok4);
  tcase_add_test(tc_floor, test_floor_ok5);
  tcase_add_test(tc_floor, test_floor_ok6);
  tcase_add_test(tc_floor, test_floor_ok7);
  tcase_add_test(tc_floor, test_floor_ok8);
  tcase_add_test(tc_floor, test_floor_ok9);
  tcase_add_test(tc_floor, test_floor_ok10);

  // ------negate_test---------
  tcase_add_test(tc_negate, test_negate_ok1);
  tcase_add_test(tc_negate, test_negate_ok2);
  tcase_add_test(tc_negate, test_negate_ok3);
  tcase_add_test(tc_negate, test_negate_ok4);
  tcase_add_test(tc_negate, test_negate_ok5);
  tcase_add_test(tc_negate, test_negate_ok6);
  tcase_add_test(tc_negate, test_negate_ok7);
  tcase_add_test(tc_negate, test_negate_ok8);
  tcase_add_test(tc_negate, test_negate_ok9);
  tcase_add_test(tc_negate, test_negate_ok10);

  // ------round_test---------
  tcase_add_test(tc_round, test_round_ok1);
  tcase_add_test(tc_round, test_round_ok2);
  tcase_add_test(tc_round, test_round_ok3);
  tcase_add_test(tc_round, test_round_ok4);
  tcase_add_test(tc_round, test_round_ok5);
  tcase_add_test(tc_round, test_round_ok6);
  tcase_add_test(tc_round, test_round_ok7);
  tcase_add_test(tc_round, test_round_ok8);
  tcase_add_test(tc_round, test_round_ok9);
  tcase_add_test(tc_round, test_round_ok10);

  suite_add_tcase(s, tc_core);
  suite_add_tcase(s, tc_fixed1);
  suite_add_tcase(s, tc_fixed2);
  suite_add_tcase(s, tc_fixed3);
  suite_add_tcase(s, tc_fixed4);
  suite_add_tcase(s, tc_fixed5);
  suite_add_tcase(s, tc_fixed6);
  suite_add_tcase(s, tc_mul);
  suite_add_tcase(s, tc_trunc);
  suite_add_tcase(s, tc_div);
  suite_add_tcase(s, tc_add);
  suite_add_tcase(s, tc_sub);
  suite_add_tcase(s, tc_floor);
  suite_add_tcase(s, tc_negate);
  suite_add_tcase(s, tc_round);

  return s;
}

int main() {
  Suite *s;
  SRunner *sr;

  s = s21_decimal_suite();
  sr = srunner_create(s);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return number_failed == 0 ? 0 : 1;
}

void test_floor(s21_decimal decimal, s21_decimal decimal_check) {
  s21_decimal result;

  int err = s21_floor(decimal, &result);
  test_result(decimal_check, result, err, 0);
}

void test_negate(s21_decimal decimal, s21_decimal decimal_check) {
  s21_decimal result;

  int err = s21_negate(decimal, &result);
  test_result(decimal_check, result, err, 0);
}

void test_round(s21_decimal decimal, s21_decimal decimal_check) {
  s21_decimal result;

  int err = s21_round(decimal, &result);
  test_result(decimal_check, result, err, 0);
}

void test_result(s21_decimal origin, s21_decimal result, int value_type_result,
                 int value_type_origin) {
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);

  int scale_origin = getScale(origin);
  int scale_result = getScale(result);
  ck_assert_int_eq(scale_origin, scale_result);

  int sign_origin = getSign(origin);
  int sign_result = getSign(result);
  ck_assert_int_eq(sign_origin, sign_result);

  ck_assert_int_eq(value_type_result, value_type_origin);
}
