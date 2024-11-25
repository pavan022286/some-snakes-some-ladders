# Target for the main game executable
playGame: main.c game_setting.c generate_board.c drawSnakesAndLadders.c
	gcc `pkg-config --cflags gtk4` -o snakeLadders main.c game_setting.c generate_board.c drawSnakesAndLadders.c `pkg-config --libs gtk4` -lm

# Clean up build artifacts
clean:
	rm -f playGame
