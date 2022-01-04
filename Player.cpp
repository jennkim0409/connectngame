// Player.cpp

#include "provided.h"
#include "support.h"
#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <cstdlib>
using namespace std;

class HumanPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    int playerMove = -1; 
    // ask user for column move .. needs to be valid or will keep looping
    while (playerMove < 0 || playerMove > s.cols()) {
        cout << "It is your turn. If you wish to quit, enter -1. Choose a column number: ";
        cin >> playerMove;
        // user asks to exit game
        if (playerMove == -1)
            exit(0);
        // can't choose column that's full
        if (s.checkerAt(playerMove, s.levels()) != -1)
            playerMove = -5;
    }
    return playerMove;
}

//////////////////////////////////////
class BadPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    // chooses the first column that has lowest level vacant
    for (int i = 1; i <= s.levels(); i++) {
        for (int j = 1; j <= s.cols(); j++) {
            if (s.checkerAt(j, i) == VACANT) {
                return j;
            }
        }
    }
    return -1;
}

/////////////////////////////////////

// non member functions that are helper functions for smart player choose move
pair<int, int> bestMove(AlarmClock &ac, Scaffold &s, const int &N, const int &color, const int &depth);
int rating(const Scaffold &s, const int &N, const int &col, const int &color, const int &depth);

class SmartPlayerImpl
{
public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    AlarmClock ac(600); // alarm clock times out after 6 seconds  
    pair <int, int> selectedMove;
    Scaffold copyBoard(s); // create copy
    selectedMove = bestMove(ac, copyBoard, N, color, 1); // get result of best move
    return selectedMove.second; // return the column selected
}

pair<int, int> bestMove(AlarmClock &ac, Scaffold &s, const int &N, const int &color, const int &depth) {

    vector<pair<int, int>> moves; // stores the score and associated column to it

    // for every single column, check if there's vacant spot
    for (int i = 1; i <= s.cols(); i++) {
        if (s.checkerAt(i, s.levels()) == -1) {
            s.makeMove(i, color); // make move and place color checker
            //s.display();
            // get its rating .. win? tie? not done?
            int result = rating(s, N, i, color, depth);

            if (result != -2) { // game is over
                pair <int, int> current;
                current.first = result;
                current.second = i;
                moves.push_back(current); // store score and column
                s.undoMove(); // undo this move
            }
            else { // game not yet finished
                if (ac.timedOut()) {} // don't call recursive function if out of time
                else {
                    // call itself but with opposite color and increase in depth
                    pair<int, int> current = bestMove(ac, s, N, abs(color - 1), depth + 1);
                    current.second = i; // update column so that it's not the child node's
                    moves.push_back(current); // push into moves
                    s.undoMove(); // undo move
                }
            }
        }
    }


    if (depth % 2 == 0) { // minimizing player
        int best = 99999999;
        int bestPosition = 0;
        for (size_t j = 0; j < moves.size(); j++) {
            if (moves[j].first < best) {
                // finds the smallest score in moves
                best = moves[j].first;
                bestPosition = moves[j].second;
            }
        }
        pair<int, int> result;
        result.first = best;
        result.second = bestPosition;
        // returns the selected score
        return result;
    }

    else { // maximizing player (depth % 2 != 0)
        int best = -99999999;
        int bestPosition = 0;
        for (size_t j = 0; j < moves.size(); j++) {
            // finds largest score in move
            if (moves[j].first > best) {
                
                best = moves[j].first;
                bestPosition = moves[j].second;
            }
        }
        pair<int, int> result;
        result.first = best;
        result.second = bestPosition;
        // returns the selected score
        return result;
    }

}


int rating(const Scaffold &s, const int &N, const int &col, const int &color, const int &depth) {
    // find out what level the most recent play was in
    int level = 0;
    for (int i = 1; i <= s.levels(); i++) {
        if (s.checkerAt(col, i) != -1) {
            if (i == s.levels())
                level = i;
            else {}
        }
        else {
            level = (i - 1);
            break;
        }
    }

    // call evaluate board to see win or tie or incomplete
    bool allFull = false;
    bool done = evaluateBoard(s, col, level, color, N, allFull);

    // not tie and player won
    if (allFull != true && done == true) {
        if (depth % 2 != 0) { // maximizing
            return (12345678 - depth); // return big number that accounts for depth
        }
        else if (depth % 2 == 0) { // minimizing
            return (-12345678 + depth); // return small number that accounts for depth
        }  
    }
    // tie
    if (allFull == true && done == true) {
        return 0;
    }
    
    // not complete
    return -2;
}


//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.
//  You probably don't want to change any of this code. 

HumanPlayer::HumanPlayer(string nm)
    : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}

HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
    : Player(nm)
{
    m_impl = new BadPlayerImpl;
}

BadPlayer::~BadPlayer()
{
    delete m_impl;
}

int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
    : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}

SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}

int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
