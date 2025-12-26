CC = gcc
CC_FLAGS = -Wall -Werror -Wextra -std=c11 -lm -g -O0
C_FILES = $(wildcard */s21_*.c s21_*.c)

TEST_DIR = test
TEST_FILES = $(wildcard $(TEST_DIR)/test.c)
O_FILES = $(C_FILES:.c=.o)
EXE_FILE = s21_decimal.out

all: clean test

test: s21_decimal.a
	$(CC) $(CC_FLAGS) $(TEST_FILES) s21_decimal.a -lcheck -lsubunit -lm -lrt -lpthread -o $(EXE_FILE)
	./$(EXE_FILE)

s21_decimal.a: $(O_FILES)
	ar rcs s21_decimal.a $(O_FILES)
	ranlib s21_decimal.a

%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

gcov_report: clean $(TEST_DIR)/test.o
	$(CC) $(CC_FLAGS) --coverage $(TEST_DIR)/test.o $(C_FILES) -lcheck -lsubunit -lm -lrt -lpthread -o repor
	./repor || true
	lcov -t "Report" -o $(TEST_DIR)/coverage.info -c -d .
	genhtml $(TEST_DIR)/coverage.info --output-directory $(TEST_DIR)/report/
	open $(TEST_DIR)/report/index.html

$(TEST_DIR)/test.o:
	$(CC) -c $(CC_FLAGS) --coverage $(TEST_FILES) -o $(TEST_DIR)/test.o

clean:
	rm -f *.o convertors/*.o *.a *.gcno *.gcda *.info *.out *.gcov
	rm -f *.o arithmetic/*.o *.a *.gcno *.gcda *.info *.out *.gcov
	rm -f *.o comparison/*.o *.a *.gcno *.gcda *.info *.out *.gcov
	rm -f *.o other_func/*.o *.a *.gcno *.gcda *.info *.out *.gcov
	rm -rf $(TEST_DIR)/report
	rm -rf $(TEST_DIR)/*.o
	rm -rf $(TEST_DIR)/*.gcda
	rm -rf $(TEST_DIR)/*.info
	rm -rf $(TEST_DIR)/*.gcno
	rm -f repor
	rm -f debug_runner

.PHONY: gdb
gdb: clean s21_decimal.a
	$(CC) $(CC_FLAGS) $(TEST_FILES) s21_decimal.a \
	      -lcheck -lsubunit -lm -lrt -lpthread -o debug_runner
	@echo "Запускаю GDB (fork отключён)…"
	CK_FORK=no gdb ./debug_runner