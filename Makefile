DEBUG_FLAGS = -Wall -Wextra -pedantic
TARGET = cminesweeper
COMPILER_FLAGS = $(DEBUG_FLAGS)
LINKER_FLAGS = -l ncurses

all: $(TARGET)

$(TARGET): object
	gcc -o $@ ./*.o $(LINKER_FLAGS)

object:
	gcc $(COMPILER_FLAGS) -I include/ -c src/*.c

