// Game.cpp

#include "provided.h"
#include "support.h"
#include <iostream>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black); 
    bool completed(int& winner) const; 
    bool takeTurn(); 
    void play(); 
    int checkerAt(int column, int level) const;

private:
    Scaffold gameBoard;
    int numToWin;
    Player* redPlayer;
    Player* blackPlayer;
    int playerTurn;
    pair<int, int> mostRecent;
    int checkPlayer;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black) : gameBoard(nColumns, nLevels), 
numToWin(N), redPlayer(red), blackPlayer(black), playerTurn(RED), checkPlayer(RED) {
    mostRecent.first = 1;
    mostRecent.second = 1;
}

bool GameImpl::completed(int& winner) const
{

    bool allFull = false;
    // evaluate the board 
    bool done = evaluateBoard(gameBoard, mostRecent.second, mostRecent.first, checkPlayer, numToWin, allFull);

    // has winner
    if (allFull == false && done == true) {
        winner = checkPlayer;
        return true;
    }
    // ended in tie
    if (allFull == true && done == true) {
        winner = TIE_GAME;
        return true;
    }
    // not yet complete
    return false;

}

bool GameImpl::takeTurn()
{
    int checkGame;
    bool status = completed(checkGame); // check status
    if (status != false) // if completed, don't take turn
        return false;
    else {
        if (playerTurn == 0) { // red
            // get column selected by redplayer
            int redColumn = redPlayer->chooseMove(gameBoard, numToWin, RED);
            gameBoard.makeMove(redColumn, RED); // make move
            
            // find out level so we can store as most recent play
            for (int i = 1; i <= gameBoard.levels(); i++) {
                if (gameBoard.checkerAt(redColumn, i) != -1) {
                    if (i == gameBoard.levels())
                        mostRecent.first = i;
                    else {}
                }
                else {
                    mostRecent.first = (i - 1);
                    break;
                }
            }
            mostRecent.second = redColumn;

            // we have to now check status for red
            checkPlayer = RED;
            // next player turn is black
            playerTurn = BLACK;
            return true;
        }

        else { // playerTurn == BLACK 
            //get column selected by black player
            int blackColumn = blackPlayer->chooseMove(gameBoard, numToWin, BLACK);
            gameBoard.makeMove(blackColumn, BLACK); // make move
           
            // find out level so we can store as msot recent play
            for (int i = 1; i <= gameBoard.levels(); i++) {
                if (gameBoard.checkerAt(blackColumn, i) != -1) {
                    if (i == gameBoard.levels())
                        mostRecent.first = i;
                    else {}
                }
                else {
                    mostRecent.first = (i - 1);
                    break;
                }
            }
            mostRecent.second = blackColumn;

            // we have to now check status for black
            checkPlayer = BLACK;
            // next player turn is red
            playerTurn = RED;
            return true;
        }
    }
}

void GameImpl::play()
{
    // welcome player
    int winningPlayer;
    cout << "Welcome to Connect " << numToWin << endl;
    cout << "By default, player " << redPlayer->name() << " will be Red (R)." << endl;
    cout << "By default, player " << blackPlayer->name() << " will be Black (B)." << endl;
    gameBoard.display(); // display empty board

    do { // inform player turn
        if (playerTurn == RED) {
            cout << "It is " << redPlayer->name() << "'s turn!" << endl;
        }
        else {
            cout << "It is " << blackPlayer->name() << "'s turn!" << endl;
        }

        takeTurn(); // call take turn
        gameBoard.display(); // display updated board

        // if both players not interactive, let watcher keep pace 
        if ((redPlayer->isInteractive() == false) && (blackPlayer->isInteractive() == false)) {
            cout << "Press enter to continue match. ";
            cin.ignore();
        }
    } while (completed(winningPlayer) == false); // loop continues until game completed

    // announce winner or tie
    string winnerName = "";
    if (winningPlayer == 0) {
        winnerName = redPlayer->name();
        cout << "The game has ended. The winner is: " << winnerName << endl;
        cout << "Hope you had fun!" << endl;
    }
    else if (winningPlayer == 1) {
        winnerName = blackPlayer->name();
        cout << "The game has ended. The winner is: " << winnerName << endl;
        cout << "Hope you had fun!" << endl;
    }
    else {
        cout << "The game ended as a tie. Hope you had fun!" << endl;
    }

}

int GameImpl::checkerAt(int column, int level) const
{
    if (column < 0 || column > gameBoard.cols() || level < 0 || level > gameBoard.levels())
        return -1; // invalid

    int check = gameBoard.checkerAt(column, level); 
    switch (check) { // check gameboard and what is currently stored in position
    case 0:
        return RED;
        break;
    case 1:
        return BLACK;
        break;
    case -1:
        return VACANT;
        break;
    }
    return -5; // invalid
} 


//******************** Game functions *******************************

//  These functions simply delegate to GameImpl's functions.
//  You probably don't want to change any of this code. 

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
