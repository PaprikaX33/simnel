CC = clang
COMPILE_FLAGS = -Werror -g -Og -Wall -Wextra -Wshadow -Wcast-align -Wunused -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2 

LINK_FLAGS = -lpthread
OUT_NAME = dsimnel


all : $(OUT_NAME)

$(OUT_NAME) : main.o
	$(CC) -o $@ $^ $(COMPILE_FLAGS) $(LINK_FLAGS)

main.o : main.c
	$(CC) -c -o $@ $< $(COMPILE_FLAGS)

.PHONY : clean

clean :
	rm -f $(OUT_NAME) main.o
