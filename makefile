snakesNladders: generate_board.c
	gcc `pkg-config --cflags gtk4` -o snakesNladders generate_board.c `pkg-config --libs gtk4`

gameSetting: game_setting.c
	gcc -o game_setting game_setting.c `pkg-config --cflags --libs gtk4`
