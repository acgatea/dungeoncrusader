/*******************************************************************************
 * game.cc
 * 
 * A module implementing one round of the game.
 * ****************************************************************************/

#include <iomanip>
#include <cassert>
#include "game.h"
#include "floor.h"
#include "enemy.h"
#include "message.h"
#include "constants.h"

using namespace std;

/*******************************************************************************
 * chooseCharacter() creates a PC of the race specified by the user
 * exceptions: throws PCQuit(), PCRestart() if the user quits or restarts
 * time: O(1) if input is valid
 * *****************************************************************************/
static PC& chooseCharacter(){
    // creates a race for the player based on input
    printRaces();

    char cmd;
    while (1) {
        cin >> cmd;
        if (cmd == 'q') throw PCQuit();
        if (cmd == 'r') throw PCRestart();

        // tries to create corresponding race
        try {
            return *createRace(cmd);
        } catch (PC::NotPCRace &e) {
            cout << "Invalid race. Please try again." << endl;
            printRaces();
        } // catch
    } // while
} // chooseCharacter

/*******************************************************************************
 * helMenu(fl) implements the help menu
 * notes: may print the floor info for fl if user asks for it
 * time : O(|floor area|)
 * *****************************************************************************/
static void helpMenu(Floor &fl) {
    // get help menu command
    while (1) {
        printHelp();
        char cmd;
        cin >> cmd;
        switch (cmd) {
            case 'b' : {
                cout << fl << "Back to the game." << endl;
                return;
            } case 'c' : {
                printCommands();
                break;
            } case 'e' : {
                printEnemies();
                break;
            } case 'f' : {
                cout << fl;
                break;
            } case 'l' : {
                printLegend();
                break;
            } case 'r' : {
                printRules();
                break;                  
            } default : {
                cout << "Invalid help option. Please try again." << endl;
            } // default
        } // switch
    } // while
} // helpMenu()

/*******************************************************************************
 * playFloor(floorNum,pc,filename) plays level floorNum, with configuration 
 *  from filename
 * required: 1 <= floorNum <= NUM_FLOORS
 *           filename is a valid, readable file [not checked]
 * exceptions: may throw PCQuits(), PCRestarts(), PCSlain(), PCWon() as needed
 * effects: updates pc's stats as the level is played
 * time: O(|input| + floor area)
 * *****************************************************************************/
static void playFloor(const int floorNum, PC &pc, const string filename) {
    assert(1 <= floorNum && floorNum <= constants::NUM_FLOORS);

    // creates a new floor and prints it
    Floor fl(floorNum, filename, pc);

    if (floorNum == 1) pc.addAction("Player Character has spawned");
    else pc.addAction("Player Character enters a new floor");

    cout << fl;

    try {
        // gets user commands until PC finishes floor,
        // loses, quits or restarts
        while(1) {
            cout << endl << "Please enter a command." << endl;        
            char cmd;
            cin >> cmd;

            if (cmd == 'q' || cmd == 'r') {
                // quit, restart commands
                delete &pc;
                if (cmd == 'q') throw PCQuit();
                else throw PCRestart();
            } else if (cmd == 'h') {
                helpMenu(fl);
            } else if (cmd == 'n' || cmd == 's' || cmd == 'w' || cmd == 'e' 
                        || cmd == 'a' || cmd == 't') {
                // direction characters
                const char dir1 = (cmd == 'a' || cmd == 't') ? cin.get() : cmd;
                const char dir2 = cin.get();
                // direction string
                string dir = "";
                dir = dir + dir1 + dir2;

                if (cmd == 'a') {
                    // tries to perform PC's attack; if dodged, does nothing
                    try {
                        pc.attackInDirection(dir);
                    } catch (InvalidMove &e) {
                        cout << "Invalid attack. " << e.what() << endl;
                        continue;
                    } catch (Enemy::AttackDodged &e) {}
                } else if (cmd == 't') {
                    // tries to perform trade; if no deal is reached
                    try {
                        pc.trade(dir);
                    } catch (InvalidMove &e) {
                        cout << "Invalid trade. " << e.what() << endl;
                        continue;
                    } catch (Merchant::NoDeal &e) {
                        cout << fl << "Back to the game." << endl;
                        continue;
                    } // catch
                } else {
                    // tries to perform PC move
                    try {
                        pc.makeMove(dir);
                    } catch(ReachedStairs &e) {
                        // reached the stairs
                        pc.addAction("Player Character has reached the stairs at floor " + to_string(floorNum));
                        break;
                    } catch(InvalidMove &e) {
                        cout << "Invalid move. " << e.what() << endl;
                        continue; 
                    } // catch
                } // else

                // enemies' turns
                fl.turnEnemies();

                // does any race-specific end-of-turn updates
                pc.turnFinished();

                // prints new state
                cout << fl;
            } else {
                cout << "Invalid command." << endl;
                printCommands();
            } // else
        } // while
    } catch (PCSlain &e) {
        // prints PC's final state
        cout << "The Player Character has been slain. Final Player Character Statistics: " << endl;
        cout << pc;
        delete &pc;
        throw e;
    } // catch

    // character has reached stairs to the next floor, so removes effects of temp potions
    pc.clearDecorators();
} // playFloor()

// see game.h for details
void playGame(const string filename) {
    // merchants are not hostile initially
    Merchant::resetHostile();

    // choose character from user input
    PC &pc = chooseCharacter();

    // plays each level until player quits, restarts, loses or wins
    for (int floorNum = 1; floorNum <= constants::NUM_FLOORS; ++floorNum) {
        playFloor(floorNum, pc, filename);
    } // for

    // player won
    cout << "Your score is " << (int) (pc.getGold() * pc.scoreMultiplier()) << "." << endl;
    delete &pc;
    throw PCWon();
} // playGame()

/**************************** Exception Messages ***********************************/
const char * UnwalkableCell::what () const noexcept{
        return "ERROR: Tile is not walkable.";
} // UnwalkableCell::what()

const char * UnoccupiedCell::what () const noexcept{
        return "ERROR: Cannot attack unoccupied cell.";
} // UnoccupiedCell::what()

const char * PCNotInCell::what () const noexcept{
        return "ERROR: Enemy cannot attack cell not containing PC.";
} // PCNotInCell::what()

const char * OccupiedCell::what () const noexcept{
        return "ERROR: Cannot move to occupied cell.";
} // OccupiedCell::what()

const char * CellContainsItem::what () const noexcept{
        return "ERROR: Cell contains an item.";
} // CellContainsItem::what()

const char * DragonAlive::what () const noexcept{
        return "ERROR: Cannot pick up Dragon Hoard while Dragon is alive.";
} // DragonAlive::what()

const char * NonMerchant::what () const noexcept{
        return "ERROR: Cell doesn't contain merchant.";
} // NonMerchant::what()

const char * NotInterested::what () const noexcept{
        return "This character is not interested in a trade.";
} // NotInterested::what()

const char * GameError::what () const noexcept{
        return "FATAL ERROR";
} // GameError::what()
