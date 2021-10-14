/*******************************************************************************
 * dungeoncrusader.cc
 * 
 * A program implementing the DungeonCrusader game.
 * ****************************************************************************/

#include <sstream>
#include <ctime>
#include "game.h"
#include "pc.h"
#include "constants.h"

using namespace std;

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // gets filename for floor configurations (if any)
    string filename = constants::EMPTY_CONFIG;
    if (argc == 2) {
        istringstream is (argv[1]);
        is >> filename;
    } // if

    // plays game until player wins, loses, or quits
    try {  
        while(1) {
            try {
                playGame(filename);
            } catch (PCRestart &e) {
                cout << "The game restarts." << endl;
            } // catch
        } // while
    } catch (PCSlain &e) {
        cout << "Game over." << endl;
    } catch (PCQuit &e) {
        cout << "You quit." << endl;
    } catch (PCWon &e) {
        cout << "Congratulations! You WIN!!!!" << endl;        
    } // catch
} // main()