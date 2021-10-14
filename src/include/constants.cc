/*******************************************************************************
 * constants.cc
 * 
 * Module for implementing methods related to constants.
 * ****************************************************************************/

#include <cmath>
#include <cassert>
#include "constants.h"

using namespace std;
using namespace constants;

// see constants.h for details
int constants::findChamberNum(const int row, const int col){
    if (col >= lCol0 && col <= rCol0 && row >= lRow0 && row <= rRow0) return 0;
    else if (col >= lCol1_0 && col <= rCol1_0 && row >= lRow1_0 && row <= rRow1_0) return 1;
    else if (col >= lCol1_1 && col <= rCol1_1 && row >= lRow1_1 && row <= rRow1_1) return 1;
    else if (col >= lCol1_2 && col <= rCol1_2 && row >= lRow1_2 && row <= rRow1_2) return 1;
    else if (col >= lCol1_3 && col <= rCol1_3 && row >= lRow1_3 && row <= rRow1_3) return 1;            
    else if (col >= lCol2_0 && col <= rCol2_0 && row >= lRow2_0 && row <= rRow2_0) return 2;
    else if (col >= lCol2_1 && col <= rCol2_1 && row >= lRow2_1 && row <= rRow2_1) return 2;
    else if (col >= lCol3 && col <= rCol3 && row >= lRow3 && row <= rRow3) return 3;
    else if (col >= lCol4 && col <= rCol4 && row >= lRow4 && row <= rRow4) return 4;
    return -1;
} // findChamberNum()

// see constants.h for details
string constants::getFullDirection (const string dir) {
    if (dir == "no") return "north";
    if (dir == "so") return "south";
    if (dir == "ea") return "east";
    if (dir == "we") return "west";
    if (dir == "ne") return "north-east";
    if (dir == "nw") return "north-west";
    if (dir == "se") return "south-east";
    assert (dir == "sw");
    return "south-west";
} // getFullDirection()

// see constants.h for details
int constants::computeDamage(const int atk, const int def){
    return ceil(atk * 100 / (100 + def));
} // computeDamage()