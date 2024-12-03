# Target for the main game executable
snakeLadders: main.c game_setting.c generate_board.c drawSnakesAndLadders.c
	gcc `pkg-config --cflags gtk4` -o snakeLadders main.c game_setting.c generate_board.c drawSnakesAndLadders.c `pkg-config --libs gtk4` -lm


# Run gcov
testGame: testApp.c game_setting.c generate_board.c drawSnakesAndLadders.c
	gcc --coverage `pkg-config --cflags gtk4` -o testGame testApp.c game_setting.c generate_board.c drawSnakesAndLadders.c `pkg-config --libs gtk4` -lm

# Clean build artifacts
clean:
	rm -f testGame *.o *.gcda *.gcno *.gcov
