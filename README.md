Some Snakes Some Ladders

This project is a digital recreation of Snakes and Ladders, designed to enhance my skills in GUI programming and algorithmic problem-solving. 
Players can enjoy the game through an intuitive graphical interface that replicates the board game's visual appeal and mechanics.

Build it using: gcc `pkg-config --cflags gtk4` -o snakeLadders main.c game_setting.c generate_board.c drawSnakesAndLadders.c `pkg-config --libs gtk4` -lm

Run it with: ./snakeLadders
