#ifndef SUPPORT_H
#define SUPPORT_H

#include "provided.h"

// will evaluate the board.. returns true if there is a win or tie .. else returns false
// to know if board is full or not, bool full is taken as reference parameter
bool evaluateBoard(const Scaffold &s, int col, int level, int color, int numWin, bool& full);


#endif 


