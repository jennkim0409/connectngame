# connectngame
Connect N Game

This is a two-player game, similar to Connect 4, except users are able to choose the column and width lenghts. The goal of the name is for one of the players to get N checkrs in a row, either horizontally, diagonally, or vertically.

My code is designed so that there can either be two human players, two computer players, or a human and computer player. If two human players, they must be playing on the same device. For the computer player, there is both a bad and intelligent option. For the intelligent computer player, the program models what a human does when playing. This means that the program will consider all the possible moves in a given situation and select the best option.

I implemented the minimax algorithm, which is a recursive algorithm that recursively evaluates all possible moves to decide which move will lead to a win or a tie. Therefore, when two computer players play against each other, it should typically always end with a tie. 
