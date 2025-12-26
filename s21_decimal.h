#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MINUS 0x80000000
#define PLUS 0xFF0000
#define MAX_DECIMAL powl(2.0, 96)
#define MIN_DECIMAL -1 * powl(2.0, 96)
#define MAX_POW 28
#define FLOAT_ACCURACY 7
#define S21_POSITIVE 0
#define S21_NEGATIVE 1
#define MAX_INDEX_BIG_DECIMAL 8
#define BIG_META_INDEX (MAX_INDEX_BIG_DECIMAL - 1)

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[MAX_INDEX_BIG_DECIMAL];
} s21_decimal_big;

// Arithmetic Operators
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Comparison Operators
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

// Convertors and parsers
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Another functions
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// Helpful functions
int getScale(s21_decimal src);
void setScale(s21_decimal *dst, int scale);

int getExp(int bit3);

int getSign(s21_decimal val);
void setSign(s21_decimal *val, int sign);

int setBit(int index, s21_decimal *dst);

int normalize(s21_decimal *value_1, s21_decimal *value_2);

int isZero(s21_decimal value_1);
void InitializationDecimal(s21_decimal *dst);
s21_decimal getZeroDecimal();

// big decimal functions
s21_decimal_big convertBig(s21_decimal dst);
s21_decimal convertBack(s21_decimal_big dst);

int getSignBig(const s21_decimal_big big);
void setSignBig(s21_decimal_big *val, int sign);

int getBitBig(s21_decimal_big value, int i);
int setBitBig(s21_decimal_big *dst, int index);
int clearBitBig(s21_decimal_big *value, int bit_index);

void SetScaleBig(s21_decimal_big *big, int scale);

int s21_is_equal_big(s21_decimal_big value_1, s21_decimal_big value_2);
int s21_is_greater_big(s21_decimal_big value_1, s21_decimal_big value_2);
int s21_is_greater_or_equal_big(s21_decimal_big value_1,
                                s21_decimal_big value_2);

int overflowCheckBig(const s21_decimal_big value);

void CheckPutSign(int value_1_sign, int value_2_sign, s21_decimal_big value_1,
                  s21_decimal_big value_2, s21_decimal_big *res_bit);
int s21_addBig(s21_decimal_big value_1, s21_decimal_big value_2,
               s21_decimal_big *result);
int s21_subBig(s21_decimal_big value_1, s21_decimal_big value_2,
               s21_decimal_big *result);

int Mul10Big(s21_decimal_big *value);
int bigTrimAndRound(s21_decimal_big *value);

void InitializationDecimalBig(s21_decimal_big *dst);
s21_decimal_big s21_mulBig(s21_decimal_big big1, s21_decimal_big big2);
void Div10countBig(s21_decimal_big *val, int count);
void div10(s21_decimal_big *val);
s21_decimal s21_roundBanking(s21_decimal integral, s21_decimal fractional);
int isZeroBig(const s21_decimal_big *value);
s21_decimal_big getZeroDecimalBig();
int comparisonBig(const s21_decimal_big *value_1,
                  const s21_decimal_big *value_2);
s21_decimal_big s21_addPatternBig(s21_decimal_big value_1,
                                  s21_decimal_big value_2);
void s21_divBig(const s21_decimal_big *dividend, const s21_decimal_big *divisor,
                s21_decimal_big *quotient, s21_decimal_big *remainder);

void addAndWritint(s21_decimal_big add, s21_decimal_big *addWrite);

s21_decimal_big shiftLeft(s21_decimal_big value, int shift);
void shiftLeftOneBig(s21_decimal_big *value);

s21_decimal_big s21_subPatternBig(s21_decimal_big value_1,
                                  s21_decimal_big value_2);

#endif
