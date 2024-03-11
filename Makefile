all: cminesweeper

cminesweeper: src/main.c
	gcc -o cminesweeper src/main.c
