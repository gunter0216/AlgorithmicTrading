CC=g++
STD=-std=c++17
WWW=
WWW=-Wall -Wextra -Werror
FILES = model/gauss.cc model/cubic_splines.cc model/newton.cc model/least_squares.cc
TESTFLAGS= -lgtest -pthread -lstdc++ -lgtest_main

all: test install gcov_report dvi

tests: test

test: clean
	@$(CC) $(STD) $(WWW) $(FILES) ./tests/test.cc $(TESTFLAGS) -o test
	./test

install: build

build: clean
	@rm -rf ../build
	@-mkdir  ../build
	@cmake -B../build
	@make -C ../build
	@cd ../build &&  rm -rf ./*[^trading\.app] CMakeCache.txt trading_autogen

uninstall:
	@cd ../build &&  rm -rf ./*

lint:
	@clang-format --style=Google -i ./*/*h ./*/*cc
	@clang-format --style=Google -n ./*/*h ./*/*cc

cppcheck:
	@cppcheck --enable=all --suppress=missingInclude  --suppress=unusedFunction --std=c++17 --language=c++  ./*/*.cc ./*/*.h

dvi:
	@open ./doc/dvi.html

dist:
	@-rm -rf DIST
	@-mkdir DIST
	@cp -R ../build/* DIST
	@tar -czvf AlgorithmTrading.tar DIST
	@-rm -rf DIST

lcov: gcov_report
gcov_report: clean
	@$(CC) $(STD) $(WWW) $(FILES) --coverage ./tests/test.cc $(TESTFLAGS) -o test
	@./test
	@lcov -t "test" --no-external -o test.info -c -d .
	@genhtml -o report test.info
	@open ./report/index.html

clean:
	@rm -rf *.a *.o *.gcda *.gcno *.info *.out report ../build test

