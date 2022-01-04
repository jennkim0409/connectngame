// Scaffold.cpp

#include "provided.h"
#include <iostream>
#include <iterator>
#include <stack>
#include <utility>
#include <vector>
using namespace std;

class ScaffoldImpl
{
  public:
    ScaffoldImpl(int nColumns, int nLevels); 
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const; 
    void display() const; 
    bool makeMove(int column, int color); 
    int undoMove();
private:
    int columnsNum;
    int levelsNum;
    int numEmpty;
    vector<vector<char>> scaffoldBoard;
    stack<pair<int, int>> moves;
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels) : columnsNum(nColumns), levelsNum(nLevels), 
    numEmpty(nColumns*nLevels)
{
        if (nColumns <= 0 || nLevels <= 0) { // invalid input
            cerr << "Please enter a valid number";
            exit(1);
        }
        scaffoldBoard.resize(nLevels);  // resize vector and adjust to col and level
        for (int i = 0; i < nLevels; i++)
            scaffoldBoard[i].resize(nColumns);  

        for (int i = 0; i < nLevels; i++)
            for (int j = 0; j < nColumns; j++)
                scaffoldBoard[i][j] = ' '; // all spaces should be empty when first constructed

}


int ScaffoldImpl::cols() const
{
    return columnsNum;
}

int ScaffoldImpl::levels() const
{
    return levelsNum;
}

int ScaffoldImpl::numberEmpty() const
{
    return numEmpty;
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    if (column > columnsNum)
        return -5; // invalid
    if (level > levelsNum)
        return -5; // invalid
    // since the way my 2d vector works for level is the opposite way (player sees from bottom
    // to top but my scaffold sees from top to bottom), I have to adjust level accordingly 
    int subtractFrom = levelsNum - level; 
    char check = scaffoldBoard[subtractFrom][column-1]; 
    switch (check) { // Return an int based on what move is or isn't on scaffold
    case 'R':
        return RED; // 0
        break;
    case 'B':
        return BLACK; // 1
        break;
    case ' ': // -1
        return VACANT;
        break;
    }

    return -5;
}

void ScaffoldImpl::display() const
{
    // Repeat for every level 
    for (int i = 0; i < levelsNum; i++) {
        cout << '|'; 
        for (int j = 0; j < columnsNum; j++) { // Show appropriate checker
             switch (scaffoldBoard[i][j]) {
            case 'R':
                cout << 'R';
                break;
            case 'B':
                cout << 'B';
                break;
            case ' ':
                cout << ' ';
                break;
            }
            cout << '|'; // Separate checkers and show column
        }
        cout << endl;
    }
    cout << '+'; // Use + and - to indicate where columns are which ones are dividers
    for (int k = 0; k < columnsNum; k++) {
        cout << '-';
        cout << '+';
    }
    cout << endl;
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    if (column < 1 || column > columnsNum) // invalid
        return false;
    if (color != RED && color != BLACK) // invalid
        return false;
    
    // check every level
    for (int i = 1; i <= levelsNum; i++) {
        // start with lowest low in the selected column
        // If spot not availble, then move up a layer
        char current = scaffoldBoard[levelsNum - i][column - 1];
        if (current == ' ') { // empty
            if (color == 0) // RED
                scaffoldBoard[levelsNum - i][column - 1] = 'R';
            else // BLACK
                scaffoldBoard[levelsNum - i][column - 1] = 'B';
            pair<int, int> position;
            position.first = i; 
            position.second = column;
            // store this position in moves.. indicating that this was most recent
            moves.push(position);
            numEmpty--; // one less empty spot
            return true;
        }
    }

    return false;
}

int ScaffoldImpl::undoMove()
{
    pair<int, int> recent;
    recent = moves.top(); // top of stack stores most recent play
    if (recent.first == 0 && recent.second == 0)
        return 0; // invalid
    // update char to indicate it's now empty
    scaffoldBoard[levelsNum-recent.first][recent.second - 1] = ' ';
    moves.pop(); // remove pair from stack
    numEmpty++; // empty space increased
    return recent.second; // column where piece was removed

}



//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.
//  You probably don't want to change any of this code. 

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}
 
Scaffold::~Scaffold()
{
    delete m_impl;
}
 
Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}
 
Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}
 
int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}
 
void Scaffold::display() const
{
    m_impl->display();
}
 
bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}
 
int Scaffold::undoMove()
{
    return m_impl->undoMove();
}

