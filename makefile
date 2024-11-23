snakesNladders: generate_board.c
	gcc `pkg-config --cflags gtk4` -o snakesNladders generate_board.c `pkg-config --libs gtk4`
