# Target for the main game executable
playGame: main.c game_setting.c generate_board.c
	gcc `pkg-config --cflags gtk4` -o playGame main.c game_setting.c generate_board.c `pkg-config --libs gtk4`

# Clean up build artifacts
clean:
	rm -f playGame
