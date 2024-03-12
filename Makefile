DEBUG_FLAGS = -Wall -Wextra -pedantic
TARGET = cminesweeper
MODE_FLAGS = $(DEBUG_FLAGS)

all: $(TARGET)

$(TARGET): object
	gcc -o $@ ./*.o

object:
	gcc $(MODE_FLAGS) -I include/ -c src/*.c

