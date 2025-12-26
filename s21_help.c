#include "s21_decimal.h"

//++ Additional functions

int s21_addBig(s21_decimal_big value_1, s21_decimal_big value_2,
               s21_decimal_big *result) {
  int value_1_sign = getSignBig(value_1), value_2_sign = getSignBig(value_2);
  s21_decimal_big res_big;
  setSignBig(&value_1, 0);
  setSignBig(&value_2, 0);

  if ((value_1_sign ^ value_2_sign) == 0)
    res_big = s21_addPatternBig(value_1, value_2);
  else {
    s21_decimal_big temp_result = getZeroDecimalBig();
    int err = 0;
    if (s21_is_greater_or_equal_big(value_1, value_2))
      err = s21_subBig(value_1, value_2, &temp_result);
    else
      err = s21_subBig(value_2, value_1, &temp_result);

    if (err == 0)
      res_big = temp_result;
    else
      res_big = getZeroDecimalBig();
  }

  *result = res_big;
  CheckPutSign(value_1_sign, value_2_sign, value_1, value_2, result);
  return 0;
}
s21_decimal_big s21_addPatternBig(s21_decimal_big value_1,
                                  s21_decimal_big value_2) {
  int carry = 0;
  s21_decimal_big result = getZeroDecimalBig();

  for (int i = 0; i < MAX_INDEX_BIG_DECIMAL * 32; i++) {
    int bit1 = getBitBig(value_1, i);
    int bit2 = getBitBig(value_2, i);
    int sum = bit1 + bit2 + carry;
    if (sum % 2 == 1)
      setBitBig(&result, i);
    else
      clearBitBig(&result, i);

    carry = sum / 2;
  }

  return result;
}
void addAndWritint(s21_decimal_big add, s21_decimal_big *addWrite) {
  s21_addBig(*addWrite, add, addWrite);
}
//-- Additional functions

//++ Subtraction functions

int s21_subBig(s21_decimal_big value_1, s21_decimal_big value_2,
               s21_decimal_big *result) {
  int err_code = 0;

  s21_decimal dec_value_1 = convertBack(value_1);
  s21_decimal dec_value_2 = convertBack(value_2);

  normalize(&dec_value_1, &dec_value_2);

  value_1 = convertBig(dec_value_1);
  value_2 = convertBig(dec_value_2);

  int sign1 = getSignBig(value_1);
  int sign2 = getSignBig(value_2);

  setSignBig(&value_1, 0);
  setSignBig(&value_2, 0);

  s21_decimal_big temp_result;

  if (sign1 == sign2) {
    if (s21_is_greater_or_equal_big(value_1, value_2)) {
      temp_result = s21_subPatternBig(value_1, value_2);
    } else {
      temp_result = s21_subPatternBig(value_2, value_1);
    }
  } else
    temp_result = s21_addPatternBig(value_1, value_2);

  *result = temp_result;
  CheckPutSign(sign1, sign2, value_1, value_2, result);

  if (overflowCheckBig(*result)) err_code = 1;

  return err_code;
}
s21_decimal_big s21_subPatternBig(s21_decimal_big value_1,
                                  s21_decimal_big value_2) {
  s21_decimal_big diff = getZeroDecimalBig();
  uint64_t borrow = 0;

  for (int i = 0; i < BIG_META_INDEX; ++i) {
    uint64_t minuend = (uint64_t)value_1.bits[i];
    uint64_t subtrahend = (uint64_t)value_2.bits[i] + borrow;
    uint64_t word_result;

    if (minuend < subtrahend) {
      borrow = 1;
      word_result = (minuend + ((uint64_t)1 << 32)) - subtrahend;
    } else {
      borrow = 0;
      word_result = minuend - subtrahend;
    }

    diff.bits[i] = (uint32_t)(word_result & 0xFFFFFFFFu);
  }
  diff.bits[BIG_META_INDEX] = value_1.bits[BIG_META_INDEX];

  return diff;
}
//-- Subtraction functions

//++ Division functions

void s21_divBig(const s21_decimal_big *dividend, const s21_decimal_big *divisor,
                s21_decimal_big *quotient, s21_decimal_big *remainder) {
  InitializationDecimalBig(quotient);
  InitializationDecimalBig(remainder);

  s21_decimal_big zero = getZeroDecimalBig();
  if (comparisonBig(divisor, &zero) == 0) return;

  for (int i = MAX_INDEX_BIG_DECIMAL * 32 - 1; i >= 0; --i) {
    shiftLeftOneBig(remainder);

    if (getBitBig(*dividend, i)) setBitBig(remainder, 0);

    if (comparisonBig(remainder, divisor) >= 0) {
      *remainder =
          s21_subPatternBig(*remainder, *divisor);  // value_1 >= value_2
      setBitBig(quotient, i);
    }
  }
}

void Div10countBig(s21_decimal_big *val, int count) {
  for (int i = 0; i < count; i++) div10(val);
}
void div10(s21_decimal_big *val) {
  s21_decimal_big divisor = getZeroDecimalBig();
  divisor.bits[0] = 10;

  s21_decimal_big quotient = getZeroDecimalBig();
  s21_decimal_big remainder = getZeroDecimalBig();

  s21_divBig(val, &divisor, &quotient, &remainder);

  uint64_t carry = remainder.bits[0];
  *val = quotient;

  if (carry > 5ULL) {
    s21_decimal_big one = getZeroDecimalBig();
    setBitBig(&one, 0);
    addAndWritint(one, val);
  } else if (carry == 5ULL) {
    if (getBitBig(*val, 0) == 1) {
      s21_decimal_big one = getZeroDecimalBig();
      setBitBig(&one, 0);
      addAndWritint(one, val);
    }
  }
}
//-- Division functions

//++ Multiplication functions

s21_decimal_big s21_mulBig(s21_decimal_big big1, s21_decimal_big big2) {
  s21_decimal_big res = getZeroDecimalBig();
  for (int i = 0; i < MAX_INDEX_BIG_DECIMAL * 32; i++) {
    if (getBitBig(big2, i)) {
      s21_decimal_big shifted = shiftLeft(big1, i);
      addAndWritint(shifted, &res);
    }
  }
  return res;
}
int Mul10Big(s21_decimal_big *value) {
  unsigned long long carry = 0;
  for (int i = 0; i < BIG_META_INDEX; i++) {
    unsigned long long temp = (unsigned long long)value->bits[i] * 10 + carry;
    value->bits[i] = (unsigned int)(temp & 0xFFFFFFFF);
    carry = temp >> 32;
  }
  return (carry != 0) ? 1 : 0;
}
//-- Multiplication functions

//++ normalize

int normalize(s21_decimal *value_1, s21_decimal *value_2) {
  int exp1 = getExp(value_1->bits[3]);
  int exp2 = getExp(value_2->bits[3]);
  int sign1 = getSign(*value_1);
  int sign2 = getSign(*value_2);

  if (exp1 > 28 || exp2 > 28) return 1;

  s21_decimal_big big1 = convertBig(*value_1);
  s21_decimal_big big2 = convertBig(*value_2);

  int maxExp = (exp1 > exp2) ? exp1 : exp2;
  maxExp = (maxExp > 28) ? 28 : maxExp;
  while (exp1 < maxExp) {
    if (Mul10Big(&big1) == 1) {
      if (exp2 > exp1) {
        while (exp2 > exp1) {
          if (exp2 <= 0 || bigTrimAndRound(&big2)) return 1;
          exp2--;
          SetScaleBig(&big2, exp2);
          maxExp = (exp2 > maxExp) ? exp2 : maxExp;
        }
      } else {
        if (exp1 <= 0 || bigTrimAndRound(&big1)) return 1;
        exp1--;
        SetScaleBig(&big1, exp1);
        maxExp = (exp1 > exp2) ? exp1 : exp2;
      }
    } else {
      exp1++;
      SetScaleBig(&big1, exp1);
    }
  }

  while (exp2 < maxExp) {
    if (Mul10Big(&big2) == 1) {
      if (exp1 > exp2) {
        while (exp1 > exp2) {
          if (exp1 <= 0 || bigTrimAndRound(&big1)) return 1;
          exp1--;
          SetScaleBig(&big1, exp1);
          maxExp = (exp1 > exp2) ? exp1 : exp2;
        }
      } else {
        if (exp2 <= 0 || bigTrimAndRound(&big2)) return 1;
        exp2--;
        SetScaleBig(&big2, exp2);
        maxExp = (exp1 > exp2) ? exp1 : exp2;
      }
    } else {
      exp2++;
      SetScaleBig(&big2, exp2);
    }
  }

  if (exp1 != exp2) return 1;
  if (overflowCheckBig(big1) || overflowCheckBig(big2)) return 1;
  *value_1 = convertBack(big1);
  *value_2 = convertBack(big2);
  setSign(value_1, sign1);
  setSign(value_2, sign2);

  return 0;
}
//-- normalize

//++Help get_bit

int getBitBig(s21_decimal_big value, int index) {
  return (value.bits[index / 32] >> (index % 32)) & 1;
}
//--Help get_bit

//++Help set_bit
int setBit(int index, s21_decimal *dst) {
  if (index < 0 || index >= 128 || dst == NULL) return 1;
  int bits_index = index / 32;
  int bit_index = index % 32;

  dst->bits[bits_index] |= (1 << bit_index);
  return 0;
}
int setBitBig(s21_decimal_big *dst, int index) {
  if (index < 0 || index >= MAX_INDEX_BIG_DECIMAL * 32 || dst == NULL) return 1;

  int bits_index = index / 32;
  int bit_index = index % 32;

  dst->bits[bits_index] |= (1 << bit_index);

  return 0;
}

int clearBitBig(s21_decimal_big *value, int index) {
  if (index < 0 || index >= MAX_INDEX_BIG_DECIMAL * 32 || value == NULL)
    return 1;

  int word_index = index / 32;
  int bit_pos = index % 32;

  value->bits[word_index] &= ~(1U << bit_pos);

  return 0;
}
//--Help Set_bit

//++Help Scale
int getScale(s21_decimal src) { return (int)((src.bits[3] >> 16) & 0xFFu); }
void setScale(s21_decimal *dst, int scale) {
  dst->bits[3] &= ~(0xFFu << 16);
  dst->bits[3] |= ((unsigned int)scale & 0xFFu) << 16;
}
void SetScaleBig(s21_decimal_big *big, int scale) {
  big->bits[BIG_META_INDEX] &= ~((0xFFu << 16) | (1u << 31));
  big->bits[BIG_META_INDEX] |= ((unsigned int)scale & 0xFFu) << 16;
}
//--Help Scale

//++Help sing
void CheckPutSign(int value_1_sign, int value_2_sign, s21_decimal_big value_1,
                  s21_decimal_big value_2, s21_decimal_big *res_bit) {
  int result_sign = 0;

  if (value_1_sign == value_2_sign) {
    result_sign = value_1_sign;
  } else {
    if (s21_is_greater_or_equal_big(value_1, value_2))
      result_sign = value_1_sign;
    else
      result_sign = value_2_sign;
  }

  setSignBig(res_bit, result_sign);
}

int getSign(s21_decimal value) { return (value.bits[3] >> 31) & 1; }
int getSignBig(const s21_decimal_big big) {
  return (big.bits[BIG_META_INDEX] >> 31) & 1;
}

void setSign(s21_decimal *val, int sign) {
  val->bits[3] &= ~(1u << 31);
  if (sign) val->bits[3] |= (1u << 31);
}
void setSignBig(s21_decimal_big *val, int sign) {
  val->bits[BIG_META_INDEX] &= ~(1u << 31);
  val->bits[BIG_META_INDEX] |= ((unsigned int)sign << 31);
}
//--Help sign

//++Help Shift
s21_decimal_big shiftLeft(s21_decimal_big value, int shift) {
  s21_decimal_big res = value;

  if (shift == 0) return res;

  for (int s = 0; s < shift; s++) {
    unsigned int carry = 0;
    for (int i = 0; i < MAX_INDEX_BIG_DECIMAL; i++) {
      uint64_t current = ((uint64_t)res.bits[i] << 1) | carry;
      res.bits[i] = (unsigned int)(current & 0xFFFFFFFFu);
      carry = (unsigned int)(current >> 32);
    }
  }

  return res;
}
void shiftLeftOneBig(s21_decimal_big *value) { *value = shiftLeft(*value, 1); }
//--Help Shift

//++Help convert
s21_decimal_big convertBig(s21_decimal dst) {
  s21_decimal_big result = getZeroDecimalBig();
  for (int i = 0; i < 3; ++i) {
    result.bits[i] = dst.bits[i];
  }

  return result;
}
s21_decimal convertBack(s21_decimal_big dst) {
  s21_decimal result = getZeroDecimal();

  if (overflowCheckBig(dst)) return result;

  for (int i = 0; i < 4; i++) result.bits[i] = dst.bits[i];

  unsigned int big_flags = dst.bits[BIG_META_INDEX];
  unsigned int scale = (big_flags >> 24) & 0xFF;
  unsigned int sign = big_flags & MINUS;

  if (scale > 28 || (big_flags & 0x7FFFFF00)) return result;

  result.bits[3] = sign | (scale << 16);

  return result;
}
//--Help convert

//++Help init
void InitializationDecimal(s21_decimal *dst) { memset(dst, 0, sizeof(*dst)); }
void InitializationDecimalBig(s21_decimal_big *dst) {
  memset(dst, 0, sizeof(*dst));
}
//--Help init

//++Help overflow
int overflowCheckBig(const s21_decimal_big value) {
  int result = 0;
  for (int i = 3; i < BIG_META_INDEX && !result; i++) {
    if (value.bits[i] != 0) result = 1;
  }
  return result;
}
//--Help Overflow

//++Help Zero
int isZero(s21_decimal value) {
  return (value.bits[0] | value.bits[1] | value.bits[2]) == 0;
}

int isZeroBig(const s21_decimal_big *value) {
  for (int i = 0; i < BIG_META_INDEX; ++i)
    if (value->bits[i]) return 0;
  return 1;
}
s21_decimal getZeroDecimal() {
  s21_decimal result;
  InitializationDecimal(&result);
  return result;
}
s21_decimal_big getZeroDecimalBig() {
  s21_decimal_big result;
  InitializationDecimalBig(&result);
  return result;
}
//--Help Zero

//++Help EXP

int getExp(int bit3) { return (bit3 >> 16) & 0xFF; }
//--Help EXP

//++Help comparisonBig
int s21_is_equal_big(s21_decimal_big value_1, s21_decimal_big value_2) {
  int result = (normalize(&((s21_decimal *)&value_1)[0],
                          &((s21_decimal *)&value_2)[0]) != 0);

  if (!result) {
    int zero_1 = 1, zero_2 = 1;
    for (int i = 0; i < MAX_INDEX_BIG_DECIMAL; i++) {
      if (value_1.bits[i] != 0) zero_1 = 0;
      if (value_2.bits[i] != 0) zero_2 = 0;
    }
    if (zero_1 && zero_2) {
      result = 1;
    }
    if (!result && (getSignBig(value_1) == getSignBig(value_2))) {
      int equal_bits = 1;
      for (int i = 0; i < MAX_INDEX_BIG_DECIMAL; i++) {
        if (value_1.bits[i] != value_2.bits[i]) {
          equal_bits = 0;
          break;
        }
      }
      result = equal_bits;
    }
  }

  return result;
}
int s21_is_greater_big(s21_decimal_big value_1, s21_decimal_big value_2) {
  int result = 0;

  int sign_1 = getSignBig(value_1);
  int sign_2 = getSignBig(value_2);

  if (sign_1 == sign_2) {
    s21_decimal dec_value_1 = convertBack(value_1);
    s21_decimal dec_value_2 = convertBack(value_2);

    normalize(&dec_value_1, &dec_value_2);

    value_1 = convertBig(dec_value_1);
    value_2 = convertBig(dec_value_2);

    int end = 0;
    for (int i = BIG_META_INDEX; i >= 0 && !end; i--) {
      if (value_1.bits[i] > value_2.bits[i]) {
        result = (sign_1 == 0) ? 1 : 0;
        end = 1;
      } else if (value_1.bits[i] < value_2.bits[i]) {
        result = (sign_1 == 0) ? 0 : 1;
        end = 1;
      }
    }
  } else {
    result = (sign_1 == 0) ? 1 : 0;
  }

  return result;
}
int s21_is_greater_or_equal_big(s21_decimal_big value_1,
                                s21_decimal_big value_2) {
  return s21_is_equal_big(value_1, value_2) ||
         s21_is_greater_big(value_1, value_2);
}
int comparisonBig(const s21_decimal_big *value_1,
                  const s21_decimal_big *value_2) {
  int result = 0;
  for (int i = BIG_META_INDEX; i >= 0 && result == 0; --i) {
    if (value_1->bits[i] > value_2->bits[i]) {
      result = 1;
    } else if (value_1->bits[i] < value_2->bits[i]) {
      result = -1;
    }
  }
  return result;
}
//--Help comparisonBig

s21_decimal s21_roundBanking(s21_decimal integral, s21_decimal fractional) {
  s21_decimal zerofive = {{0x00000005, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal result = getZeroDecimal();

  if (s21_is_equal(fractional, zerofive))
    if ((integral.bits[0] & 1) != 1)
      result = integral;
    else
      s21_add(integral, (s21_decimal){{1, 0, 0, 0}}, &result);
  else if (s21_is_greater(fractional, zerofive))
    s21_add(integral, (s21_decimal){{1, 0, 0, 0}}, &result);
  else
    result = integral;
  return result;
}
int bigTrimAndRound(s21_decimal_big *value) {
  uint64_t rem = 0;
  s21_decimal_big quotient = *value;
  for (int i = 6; i >= 0; i--) {
    uint64_t dividend = (rem << 32) | quotient.bits[i];
    quotient.bits[i] = (unsigned int)(dividend / 10);
    rem = dividend % 10;
  }

  int roundUp = 0;
  if (rem > 5)
    roundUp = 1;
  else if (rem == 5) {
    unsigned int lsd = quotient.bits[0] % 10;
    if (lsd % 2 != 0) roundUp = 1;
  }
  if (roundUp) {
    uint64_t carry = 1;
    for (int i = 0; i < BIG_META_INDEX; i++) {
      uint64_t sum = (uint64_t)quotient.bits[i] + carry;
      quotient.bits[i] = (unsigned int)(sum & 0xFFFFFFFF);
      carry = sum >> 32;
      if (!carry) break;
    }
    if (carry) return 1;
  }
  for (int i = 0; i < 7; i++) value->bits[i] = quotient.bits[i];

  return 0;
}
