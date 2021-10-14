/*******************************************************************************
 * floor.cc
 * 
 * A module implementing a floor (i.e. level).
 * ****************************************************************************/

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cassert>
#include "floor.h"
#include "game.h"
#include "pc.h"
#include "cell.h"
#include "item.h"
#include "constants.h"

using namespace std;
using namespace constants;

// see floor.h for details
Cell& Floor::randFloorTile(const int notIn) const{
    assert(-1 <= notIn && notIn <= NUM_CHAMBERS);

    // gets random chamber number, avoiding notIn
    int chamberNum;
    do {
        chamberNum = rand() % NUM_CHAMBERS;
    } while (chamberNum == notIn);

    // number of failed attempts to find empty tile
    int failedAttempts = 0;
    // gets random empty floor tile in this chamber
    // after MAX_CHAMBER_LOAD attempts, tries another chamber
    while (1) {
        const int r = rand() % NUM_ROWS;
        const int c = rand() % NUM_COLS;
        if (floor[r][c]->getChamberNum() == chamberNum
            && floor[r][c]->isFloorTile() && !floor[r][c]->isOccupiedByCharacter()
            && !floor[r][c]->containsItem()) {
                return *(floor[r][c]);
        } // if
        ++failedAttempts;
        if (failedAttempts == MAX_CHAMBER_LOAD) {
            // this chamber is quite full so tries the next one
            chamberNum = (chamberNum + 1) % NUM_CHAMBERS;
        } // if
    } // while
} // randFloorTile()

// see floor.h for details
void Floor::populate(PC &pc) {
    // randomly places player and stairs in different chambers
    Cell& pcLocation = randFloorTile();
    pcLocation.characterMovesIn(pc);
    randFloorTile(pcLocation.getChamberNum()).setStairs();

    // randomly places potions
    for (int i =0; i < NUM_POTION_PER_FLOOR; ++i) {
        randFloorTile().depositItem(*createItem('P'));
    } // for

    // randomly places gold piles
    for (int i =0; i < NUM_GOLD_PER_FLOOR; ++i) {
        Cell & destination = randFloorTile();

        Item * item = NULL;
        while (!item) {
            try {
                item = createItem('G');
            } catch (Item::DHCreated &e) {
                try {
                    // if Dragon Hoard was created, tries to create a dragon in
                    // the vicinity of the Dragon Hoard
                    Dragon * d = &(destination.generateDragon(e.getDH()));
                    item = &(e.getDH());
                } catch (Cell::NoSpaceForDragon &e) {}
            } // catch
        } // while
        // places item at location
        destination.depositItem(*item);
    } // for

    // randomly places enemies; # enemies increases per floor
    const int numEnemiesThisFloor = NUM_ENEMIES_PER_FLOOR + floorNum * 2;
    for (int i =0; i < numEnemiesThisFloor; ++i) {
        enemies.push_back(createEnemy());
        randFloorTile().characterMovesIn(*enemies.back());
    } // for
} // populate()

// see floor.h for details
void Floor::linkDragons (){
    // links every dragon
    for (vector<Dragon *>::iterator it = dragons.begin(); it != dragons.end(); ++it) {
        // tries to find dragon hoard in 1 block radius
        for (vector<string>::const_iterator iter = DIRECTIONS.begin(); iter != DIRECTIONS.end(); ++iter) {
            Cell * target = (*it)->getLocation()->getNeighbor(*iter);

            // if no walkable tile in this direction, try next direction
            if (!target) continue;

            // if there is a dragon hoard in this cell, search is over
            DragonHoard * dh = (DragonHoard *) target->getItem();
            if (dh) {                
                // links dragon and dragon hoard
                (*it)->attachDH(*dh);
                dh->attachDragon(**it);
                break;
            } // if
            if (iter + 1 == DIRECTIONS.end()) {
                // the search must be successful
                cerr << "Configuration file contains Dragon not beside a Dragon Hoard." << endl;
                throw GameError();
            } // if
        } // for
    } // for
    // clears list of dragons to be linked
    dragons.clear();
} // linkDragons()

// see floor.h for details
Floor::Floor(const int floorNum, const string filename, PC &pc) : floorNum(floorNum), pc(pc) {
    ifstream in (filename.c_str());
    if (!in.good()) {
        cout << "Invalid file. Will use random configuration." << endl;
        in.close();
        in = ifstream (EMPTY_CONFIG.c_str());
    } // if      

    // reads config char-by-char
    floor = new Cell ** [NUM_ROWS];
    for (int row = 0; row < NUM_ROWS; ++row) {
        floor[row] = new Cell * [NUM_COLS];
        char ch;
        for (int col = 0; col < NUM_COLS; ++col) {
            ch = in.get(); 

            // determines chamberNum
            const int chamberNum = findChamberNum(row, col); 
            floor[row][col] = new Cell (row, col, chamberNum, ch, pc, *this);
        } // for
        ch = in.get(); // skip new line
    } // for
    in.close();

    // stores neighbors for walkable tiles
    for (int row = 0; row < NUM_ROWS; ++row) {
        for (int col = 0; col < NUM_COLS; ++col) {
            if (floor[row][col]->isWalkableTile()) {
                // look at 8 possible directions
                for (map<const string, pair<const int, const int> >::const_iterator 
                        it = DIRECTION_TO_OFFSET.begin(); it != DIRECTION_TO_OFFSET.end(); ++it) {
                    const int neighbRow = row + it->second.first;
                    const int neighbCol = col + it->second.second;
                    // if the neighbor's coordinates are within bounds and is walkable, adds the neighbor 
                    if (0 <= neighbRow && 0 <= neighbCol && neighbRow < NUM_ROWS && neighbCol < NUM_COLS
                        && floor[neighbRow][neighbCol]->isWalkableTile()) {
                        floor[row][col]->addNeighbor(it->first, *(floor[neighbRow][neighbCol]));
                    } // if
                } // for
            } // if
        } // for
    } // for

    // links dragons with dragon hoards
    linkDragons();

    // randomly populates the floor if the initial configuration was empty
    if (filename == EMPTY_CONFIG) populate(pc);
} // Floor ctor

// see floor.h for details
Floor::~Floor() {
    // delete enemies
    for(vector<Enemy *>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
        delete (*it);
    } // for

    // delete cells
    for (int row = 0; row < NUM_ROWS; ++row) {
        for (int col = 0; col < NUM_COLS; ++col) {
            delete floor[row][col];
        } // for
        delete [] floor[row];
    } // for
    delete [] floor;
} // Floor dtor

// see floor.h for details
void Floor::addEnemy (Enemy &e){
    enemies.push_back(&e);
} // addEnemy()

// see floor.h for details
void Floor::addDragon (Dragon &d){
    dragons.push_back(&d);
} // addDragon()

// see floor.h for details
void Floor::removeEnemy(const Enemy &e){
    enemies.erase(remove(enemies.begin(), enemies.end(), &e), enemies.end());
} // removeEnemies()


// see floor.h for details
void Floor::turnEnemies(){
    // each enemy gets a turn
    for (vector<Enemy *>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
        // has this enemy attacked the PC this turn?
        bool enemyAttacked = false;

        // enemy tries to attack PC in any direction
        for (vector<string>::const_iterator iter = DIRECTIONS.begin(); iter != DIRECTIONS.end(); ++iter) {
            try {
                (*it)->attackInDirection(*iter);
                enemyAttacked = true;
                break;  
            } catch (UnwalkableCell &e) {
            } catch (UnoccupiedCell &e) {
            } catch (PCNotInCell &e) {
            } catch (Enemy::NotHostile &e) {
            } catch (Player::AttackDodged &e) {
                enemyAttacked = true;
                break;
            } // catch
        } // for

        // if attacked succeeded or was dodged, enemy's turn is over; otherwise, enemy moves
        if (enemyAttacked) continue;

        // keep trying random moves until one is valid or list of available moves is exhausted
        vector<string> options = DIRECTIONS;
        do {
            // choose random direction
            const int index = rand() % options.size();
            try {
                (*it)->makeMove(options.at(index));
                // if move was successful, then moving turn is done
                break;
            } catch (InvalidMove &e) {
                // if move is invalid, remove from list and try again
                options.erase(options.begin() + index);
            } // catch
        } while (!options.empty());
    } // for
} // turnEnemies()

// see floor.h for details
ostream& operator<< (ostream& out, const Floor &f) {
    // prints floor config
    for (int row = 0; row < constants::NUM_ROWS; ++row) {
        for (int col = 0; col < constants::NUM_COLS; ++col) {
            out << * (f.floor[row][col]);
        } // for
        out << endl;
    } // for
    out << right << setw(constants::NUM_COLS/2) << "Floor " << f.floorNum << endl;

    // prints PC stats and action
    out << f.pc;

    return out;
} // operator<< (ostream&, Floor&)