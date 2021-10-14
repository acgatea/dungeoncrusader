/*******************************************************************************
 * message.cc
 * 
 * Module implementing help messages for the game.
 * ****************************************************************************/

#include <iostream>
#include <iomanip>
#include "message.h"
#include "constants.h"

using namespace std;
using namespace constants;

//see message.h for details
void printRaces(){
    cout.precision(2);
    cout << "Please enter a race or quit. Available races:" << endl;
    cout    << "s (Shade):   " + to_string(SHADE_HP) + " HP, "
            << to_string(SHADE_ATK) + " Atk, "
            << to_string(SHADE_DEF) + " Def, " << SHADE_ABILITY << endl
            << "d (Drow):    " + to_string(DROW_HP) + " HP, "
            << to_string(DROW_ATK) + " Atk, "
            << to_string(DROW_DEF) + " Def, " << DROW_ABILITY << endl
            << "v (Vampire):  " + to_string(VAMP_HP) + " HP, "
            << to_string(VAMP_ATK) + " Atk, "
            << to_string(VAMP_DEF) + " Def, " << VAMP_ABILITY << endl
            << "t (Troll):   " + to_string(TROLL_HP) + " HP, "
            << to_string(TROLL_ATK) + " Atk, "
            << to_string(TROLL_DEF) + " Def, " << TROLL_ABILITY << endl
            << "g (Goblin):  " + to_string(GOBLIN_HP) + " HP, "
            << to_string(GOBLIN_ATK) + " Atk, "
            << to_string(GOBLIN_DEF) + " Def, " << GOBLIN_ABILITY << endl;
} // printRaces()

//see message.h for details
void printEnemies(){
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
    cout.precision(2);
    cout << right << setw(43) << "ENEMIES" << endl;
    cout    << "H (Human):    " + to_string(HUMAN_HP) + " HP, "
            << to_string(HUMAN_ATK) + " Atk, "
            << to_string(HUMAN_DEF) + " Def, " << HUMAN_ABILITY << endl
            << "W (Dwarf):    " + to_string(DWARF_HP) + " HP, "
            << to_string(DWARF_ATK) + " Atk, "
            << to_string(DWARF_DEF) + " Def, " << endl 
            << "              " << DWARF_ABILITY << endl
            << "L (Halfling): " + to_string(HALFLING_HP) + " HP, "
            << to_string(HALFLING_ATK) + " Atk, "
            << to_string(HALFLING_DEF) + " Def, " << endl 
            << "              " << HALFLING_ABILITY << endl
            << "E (Elf):      " + to_string(ELF_HP) + " HP, "
            << to_string(ELF_ATK) + " Atk, "
            << to_string(ELF_DEF) + " Def, " << ELF_ABILITY << endl
            << "O (Orc):      " + to_string(ORC_HP) + " HP, "
            << to_string(ORC_ATK) + " Atk, "
            << to_string(ORC_DEF) + " Def, " << ORC_ABILITY << endl
            << "D (Dragon):   " + to_string(DRAGON_HP) + " HP, "
            << to_string(DRAGON_ATK) + " Atk, "
            << to_string(DRAGON_DEF) + " Def, " << DRAGON_ABILITY << endl
            << "M (Merchant): " + to_string(MERCH_HP) + " HP, "
            << to_string(MERCH_ATK) + " Atk, "
            << to_string(MERCH_DEF) + " Def, " << MERCH_ABILITY << endl;
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
} // printEnemies()

//see message.h for details
void printCommands(){
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
    cout << "Valid in-game commands: " << endl;
    cout << " no,so,ea,we,ne,nw,se,sw -- moves player in this direction" << endl;
    cout << " a <direction> -- attacks enemy indicated by direction" << endl;
    cout << " t <direction> -- trades with merchant indicated by direction" << endl;
    cout << " h -- help" << endl;
    cout << " q -- quit" << endl;
    cout << " r -- restart" << endl;
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
} // printCommands()

//see message.h for details
void printHelp(){
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
    cout << "Please choose a help option: " << endl;
    cout << " b -- back to game" << endl;
    cout << " c -- available commands" << endl;
    cout << " e -- enemy info" << endl;
    cout << " f -- floor info" << endl;
    cout << " l -- legend" << endl;
    cout << " r -- rules" << endl;
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
} // printHelp()

//see message.h for details
void printLegend(){
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
    cout << right << setw(43) << "LEGEND" << endl;
    cout << left << setw(40) << " -,|: walls";
    cout << left << setw(85) << " #: passage" << endl;
    cout << left << setw(40) << " +: doorway";
    cout << left << setw(85) << " \\: stairway to next floor" << endl;
    cout << left << setw(40) << " G: gold pile (type hidden)";
    cout << setw(85) << " P: potion (type hidden)" << endl;
    cout << left << setw(40) << " @: you";
    cout << setw(85) << " H: human (enemy)" << endl;
    cout << left << setw(40) << " W: dwarf (enemy)";
    cout << setw(85) << " L: halfling (enemy)" << endl;
    cout << left << setw(40) << " E: elf (enemy)";
    cout << setw(85) << " O: orc (enemy)" << endl;
    cout << left << setw(40) << " M: merchant (enemy)";
    cout << setw(85) << " D: dragon (enemy)" << endl;
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
} // printLegend()

//see message.h for details
void printRules(){
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
    cout << right << setw(50) << "DUNGEON CRUSADER GAME RULES" << endl;
    cout << "The object of the game is to move through the Player Character (@)" << endl
         << "through various floors (levels) items." << endl << endl
         << "To move to the next floor, PC must reach the stairs (\\)." << endl
         << "The final score is the amount of gold collected times a race-specific" << endl
         << "multiplier. " << endl << endl
         << "Items come in two forms: gold (G) and potions (P)." << endl
         << "Potions may have positive or negative effects, which may" << endl
         << "be temporary (only on the current floor) or permanent. " << endl
         << "Gold comes in various pile types. A dragon hoarde cannot be picked up" << endl 
         << "until the guarding Dragon is slain." << endl 
         << "To collect an item, the PC must move on the tile containing the item." << endl << endl
         << "PC might encounter enemies. An enemy will always attack the PC if it" << endl
         << "it is within a 1 block radius of that enemy. Enemies do not move out" << endl
         << "of their chamber." << endl << endl
         << "Merchants are neutral unless one is killed by the PC, at which point"  << endl
         << "they become hostile like all other enemies. PC may barter with non-hostile" << endl 
         << "merchants for potions. Merchants may steal gold during transactions."<< endl << endl
         << "The PC may choose to attack an enemy in a 1 block radius. Defeated " << endl
         << "enemies drop piles of gold. If the PC is slain, the game is over." << endl << endl
         << "The user may quit or restart the game any time." << endl << endl;
    cout << setfill('*') << setw(80) << "" << setfill(' ') << endl;
} // printRules