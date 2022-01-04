#include "support.h"
#include "provided.h"

bool evaluateBoard(const Scaffold& s, int col, int level, int color, int numWin, bool& full) {

    // if there isn't enough plays to count win, return false
    if (s.numberEmpty() > (s.levels() * s.cols() - 1))
        return false;

    int numHorizontal = 0;
    // horizontally
    // check every column in the most recent play's level
    for (int i = 1; i <= s.cols(); i++) {
        if (s.checkerAt(i, level) == color) {
            numHorizontal++; 
            if ((i + 1) <= s.cols()
                && s.checkerAt(i + 1, level) != color)
                break; // colors need to be connected consecutively
        }
    }
    if (numHorizontal >= numWin) // horizontal win
        return true;

    int numVertical = 0;
    // vertically
    // check every level in the most recent play's column
    for (int i = 1; i <= s.levels(); i++) {
        if (s.checkerAt(col, i) == color) {
            numVertical++;
            if ((i + 1) <= s.cols()
                && s.checkerAt(col, i + 1) != color)
                break; // colors need to be connected consecutively
        }
    }
    if (numVertical >= numWin) // vertical win
        return true;


    int numDiagL = 0;
    int numDiagR = 0;
    // checking ascending and descending diagonals .. 
    // first for loop is to the left side of recent play
    int j = 0;
    int m = 0;
    for (int i = col; i > 0; i--) {
        if (level - j > 0) { // needs to be valid
            if (s.checkerAt(i, level - j) == color) {
                numDiagL++;
                if (level - (j + 1) > 0 && (i - 1) > 0 &&
                    s.checkerAt(i - 1, level - (j + 1)) != color)
                    j = 100; // if the next in the diagonal isn't same color, then end it
            }
        }
        if (level + m <= s.levels()) { // needs to be valid
            if (s.checkerAt(i, level + m) == color) {
                numDiagR++;
                if (level + (m + 1) <= s.levels() && (i - 1) > 0 &&
                    s.checkerAt(i - 1, level + (m + 1)) != color)
                    m = 100; // if the next in the diagonal isn't same color, then end it
            }
        }
        j++; // increment to account for level
        m++;
    }
    int k = 1;
    int l = 1;
    // checking ascending and descending diagonals .. 
    // second for loop is to the right side of recent play
    for (int i = col + 1; i <= s.cols(); i++) {
        if (level + k <= s.levels()) { // needs to be valid
            if (s.checkerAt(i, level + k) == color) {
                numDiagL++;
                if (level + (k + 1) <= s.levels() && (i + 1) <= s.cols() &&
                    s.checkerAt(i + 1, level + (k + 1)) != color)
                    k = 100; // if the next in the diagonal isn't same color, then end it
            }
        }
        if (level - l > 0) { // needs to be valid
            if (s.checkerAt(i, level - l) == color) {
                numDiagR++;
                if (level - (l + 1) > 0 && (i + 1) <= s.cols() &&
                    s.checkerAt(i + 1, level - (l + 1)) != color)
                    l = 100; // if the next in the diagonal isn't same color, then end it
            }
        }
        l++; // increment to account for level
        k++;
    }
    if (numDiagL >= numWin) // left diagonal win
        return true;

    if (numDiagR >= numWin) // right diagonal win
        return true;


    bool allFull = true; // if all positions are occupied by checker, return true
    int i = s.levels();
    for (int j = 1; j <= s.cols(); j++) {
        if (s.checkerAt(j, i) == -1) {
            allFull = false;
            break;
        }
    }
    full = allFull;

    return allFull; // returns false
}
