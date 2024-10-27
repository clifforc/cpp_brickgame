CC = g++
FLAGS = -Wall -Wextra -Werror -std=c++17
DEBUG_FLAGS = -std=c++17 -g
P = -pedantic

PROJECT_NAME = brickGame
LIB_TETRIS = tetris
LIB_TETRIS_SRC = $(wildcard brick_game/tetris/*.cpp)
LIB_SNAKE = snake
LIB_SNAKE_SRC = $(wildcard brick_game/snake/*/*.cpp)
GUI_SRC = $(wildcard gui/cli/*.cpp)
GUI_QT_SRC = $(wildcard gui/desktop/*.cpp)

SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, $(OBJDIR)%.o, $(SOURCES))
TEST = s21_test
TEST_DIR = tests/
RM_EXTS := o a out gcno gcda gcov info html css gz

CPP_DIRS := brick_game/snake/ gui/ tests/
CPP_FILES := main.cpp main_cls.cpp

OS := $(shell uname)
MAC_X86 := $(shell uname -a | grep -o _X86_64)
MAC_ARM := $(shell uname -a | grep -o _ARM64)

GTEST_INCLUDE = $(shell brew --prefix googletest)/include
GTEST_LIB = $(shell brew --prefix googletest)/lib
GTEST_FLAGS = -DGTEST_HAS_PTHREAD=1 -I$(GTEST_INCLUDE) -L$(GTEST_LIB) -lgtest -lgmock

ifeq ($(OS), Linux)
	TEST_LIBS = -lcheck -pthread -lrt -lm -lsubunit
	OPEN = xdg-open
endif

ifeq ($(MAC_ARM), _ARM64)
	TEST_LIBS := $(GTEST_FLAGS)
	OPEN = open
endif
ifeq ($(MAC_X86), _X86_64)
	TEST_LIBS = $(GTEST_FLAGS) -lpthread
	OPEN = open
endif

all: run

run: install install_gtk
	build/$(PROJECT_NAME)

build: install install_gtk

repo:
	mkdir -p $(OBJ)
$(OBJDIR)%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o obj/$@


install: tetris.a snake.a gui.o main_cls.o
	mkdir -p build/
	$(CC) *.o -lncurses -o build/$(PROJECT_NAME)
	rm -rf *.o

install_gtk: tetris.a snake.a
	mkdir -p build/
	cd build && cmake .. && cmake . && make
	rm -rf *.o

uninstall: clean
	rm -rf build/$(PROJECT_NAME)

tetris.a: $(LIB_TETRIS).o
	ar rcs $(LIB_TETRIS).a *.o
	ranlib $(LIB_TETRIS).a

snake.a: $(LIB_SNAKE).o
	ar rcs $(LIB_SNAKE).a *.o
	ranlib $(LIB_SNAKE).a

$(LIB_TETRIS).o:
	$(CC) $(FLAGS) -c $(LIB_TETRIS_SRC) $(DEBUG_FLAGS)

$(LIB_SNAKE).o:
	$(CC) $(FLAGS) -c $(LIB_SNAKE_SRC) $(DEBUG_FLAGS)

gui.o:
	$(CC) $(FLAGS) -c $(GUI_SRC)

main_cls.o:
	$(CC) $(FLAGS) -c main_cls.cpp

dvi:
	doxygen Doxyfile
	open html/index.html

dist: clean uninstall
	tar -czf brickgame.install.tar.gz ./*

test: tetris.a snake.a
	$(CC) $(FLAGS)  tests/*.cpp $(TEST_LIBS) tetris.a snake.a -o $(TEST)
	./$(TEST)

ifeq ($(OS),Linux)
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(TEST)
else
	leaks --atExit -- ./$(TEST)
endif

gcov_report: clean tetris.a snake.a
	g++ $(FLAGS) -fprofile-arcs --coverage $(LIB_TETRIS_SRC) $(LIB_SNAKE_SRC) tests/tests.cpp tetris.a snake.a $(TEST_LIBS) -o report.out
	./report.out
	gcovr --html-details -o report.html --exclude tests/*.cpp
	rm -rf *.gcno *.gcda *.gcov *.info
	open report.html

cppcheck: reset
	@cppcheck --enable=all \
		--check-level=exhaustive \
		--suppress=missingIncludeSystem \
		--std=c++17 \
		--language=c++ \
		--suppress=unusedStructMember \
		--suppress=unusedFunction \
		--suppress=noExplicitConstructor \
		--suppress=unmatchedSuppression \
		--suppress=checkersReport \
		--suppress=uninitvar \
		--suppress=syntaxError \
		--suppress=useStlAlgorithm \
		$(CPP_DIRS) $(CPP_FILES)
.PHONY: cppcheck

add_db_flag:
	$(eval FLAGS += $(DB))

mem: clean add_db_flag test
ifeq ($(OS),Linux)
	valgrind -s --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(TEST)
else
	leaks --atExit -- ./$(TEST)
endif

rebuild: clean build

clean:
	rm -rf $(TEST) *.o *.a *.gcno *.gcda *.gcov *.info report a.out *.dSYM obj
	@for ext in $(RM_EXTS); do \
	find ./ -type f -name "*.$$ext" -exec rm {} \;; \
	done
	@rm -rf ./report
	@rm -rf ./obj
	@rm -rf dist/
	@rm -rf ./build
	@rm -rf ./latex
	@rm -rf html/
	@rm -rf highscore.txt
	@rm -rf max_score.txt
.PHONY: clean
	reset

reset:
	reset | bash

format-check :
	@cp ../materials/linters/.clang-format ./
	@find ./ -type f -name "*.[cpp]" -exec clang-format -n {} \;
	@find ./ -type f -name "*.[h]" -exec clang-format -n {} \;
	@rm ./.clang-format
.PHONY: format-check

format :
	@cp ../materials/linters/.clang-format ./
	@find ./ -type f -name "*.[cpp]" -exec clang-format -i {} \;
	@find ./ -type f -name "*.[h]" -exec clang-format -i {} \;
	@rm ./.clang-format
.PHONY: format
