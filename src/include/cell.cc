/*******************************************************************************
 * cell.cc
 * 
 * A module implementing a cell on the game board.
 * ****************************************************************************/

#include <cassert>
#include "cell.h"
#include "item.h"
#include "enemy.h"
#include "potion.h"

using namespace std;

// see cell.h for details
Cell::Cell (const int row, const int col, const int chamberNum, const char ch, PC &pc, Floor &fl): 
            row(row), col(col), chamberNum(chamberNum), ch(ch), fl(fl), item (0), pc(0), enemy(0) {
    if (ch == '.') type = FloorTile;
    else if (ch == '@') {
        type = FloorTile;
        // changes character for printing purposes
        this->ch = '.';

        characterMovesIn(pc);
    } else if (ch >= '0' && ch <= '9') {
        type = FloorTile;
        // creates corresponding gold / potion item
        Item * i;
        switch (ch) {
            case '0' : {
                i = new RH();
                break;
            } case '1' : {
                i = new BA();
                break;
            } case '2' : {
                i = new BD();
                break;
            } case '3' : {
                i = new PH();
                break;
            } case '4' : {
                i = new WA();
                break;
            } case '5' : {
                i = new WD();
                break;
            } case '6' : {
                i = new NormalHoard();
                break;
            } case '7' : {
                i = new SmallHoard();
                break;
            } case '8' : {
                i = new MerchantHoard();
                break;
            } case '9' : {
                i = new DragonHoard();
                break;
            } // case
            // changes character for printing purposes
            this->ch = (ch <= '5') ? 'P' : 'G';
        } // switch
        depositItem(*i);
    } else if (ch == 'H' || ch == 'W' || ch == 'E' || ch == 'O' 
               || ch == 'M' || ch == 'D' || ch == 'L') {
        type = FloorTile;
        // creates corresponding enemy
        Enemy * e;
        switch (ch) {
            case 'H' : {
                e = new Human();
                break;
            } case 'W' : {
                e = new Dwarf();
                break;
            } case 'E': {
                e = new Elf();
                break;
            } case 'M': {
                e = new Merchant();
                break;
            } case 'O': {
                e = new Orc();
                break;
            } case 'D': {
                Dragon * d = new Dragon();
                fl.addDragon(*d);
                e = d;
                break;
            } case 'L': {
                e = new Halfling();
            } // case
        } // switch
        // changes character for printing purposes
        this->ch = '.';
        characterMovesIn(*e);
        fl.addEnemy(*e);
    } else if (ch == '+') type = Doorway;
    else if (ch == '#') type = Passageway;
    else if (ch == '|' || ch == '-') type = Wall;
    else if (ch == '\\') type = Stairs;
    else if (ch == ' ') type = Empty;
    else {
        cerr << "Invalid board character: " + string(1,ch) << endl;
        throw GameError();
    } // else
} // Cell ctor

// see cell.h for details
Cell::~Cell(){
    delete item;
} // ~Cell()

// see cell.h for details
void Cell::addNeighbor(const string dir, Cell &neighbor) {
    neighbors.insert(pair<const string, Cell &>(dir, neighbor));
} // addNeighbor()

// see cell.h for details
Cell* Cell::getNeighbor(const string dir) const{
    map<string, Cell &>::const_iterator it = neighbors.find(dir);
    return (it == neighbors.end()) ? 0 : &(it->second);
} // getNeighbor()

// see cell.h for details
Dragon& Cell::generateDragon (DragonHoard &dh) const {
    // tries every walkable neighbor
    for (std::map<std::string, Cell &>::const_iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
        Cell &target = (*it).second;
        if (target.isFloorTile() && !target.isStairs()
            && !target.containsItem() && !target.isOccupiedByCharacter()) {
            // if target is empty and is a floor tile, creates a dragon in it
            Dragon *d = new Dragon();
            target.characterMovesIn(*d);

            // adds dragon to list of enemies on the floor
            fl.addEnemy(*d);

            // links dragon and dragon hoard
            dh.attachDragon(*d);
            d->attachDH(dh);
            return *d;
        } // if
    } // for
    throw NoSpaceForDragon();
} // generateDragon()

// see cell.h for details
Enemy* Cell::getEnemy() const {
    return enemy;
} // getEnemy()

// see cell.h for details
PC* Cell::getPC() const {
    return pc;
} // getPC()

// see cell.h for details
Item* Cell::getItem() const {
    return item;
} // getItem()

// see cell.h for details
int Cell::getChamberNum() const {
    return chamberNum;
} //getChamberNum()

// see cell.h for details
void Cell::setStairs(){
    type = Cell::Stairs;
} // setStairs()

// see cell.h for details
void Cell::characterMovesIn(PC &p) {
    if (pc) {
        cerr << "PC cannot move in." << endl;
        throw GameError();
    } // if
    pc = &p;
    p.setLocation(this);
} // characterMovesIn(PC&)

// see cell.h for details
void Cell::characterMovesIn(Enemy &e) {
    if (enemy) {
        cerr << "Enemy cannot move in." << endl;
        throw GameError();
    } // if
    enemy = &e;
    e.setLocation(this);
} // characterMovesIn(Enemy&)

// see cell.h for details
void Cell::pcMovesOut() {
    assert(pc);
    pc->setLocation(NULL);
    pc = NULL;
} // pcMovesOut()

// see cell.h for details
void Cell::enemyMovesOut() {
    assert(enemy);
    enemy->setLocation(NULL);
    enemy = NULL;
} // enemyMovesOut()

// see cell.h for details
void Cell::enemyDies() {
    assert(enemy);
    fl.removeEnemy(*enemy);
    delete enemy;
    enemy = NULL;
} // enemyDies()

// see cell.h for details
bool Cell::isFloorTile() const {
    return type == FloorTile;
} // isFloorTile()

// see cell.h for details
bool Cell::isStairs() const{
    return type == Stairs;
} // isStairs()

// see cell.h for details
bool Cell::isWalkableTile() const{
    return type == FloorTile || type == Doorway || type == Stairs || type == Passageway;
} // isWalkableTile()

// see cell.h for details
bool Cell::isOccupiedByCharacter() const {
    return (pc != NULL || enemy != NULL);
} // isOccupiedByCharacter()

// see cell.h for details
bool Cell::isOccupiedByPC() const {
    return (pc != NULL);
} // isOccupiedByPC()

// see cell.h for details
bool Cell::containsItem() const {
    return (item != NULL);
} // containsItem()

// see cell.h for details
void Cell::depositItem (Item &i) {
    if (pc || enemy || item) {
        cerr << "Cannot store item here." << endl;
        throw GameError();
    } // if
    item = &i;
} // depositItem()

// see cell.h for details
void Cell::pickUpItem(Player &p){
    assert(item);
    item->pickUp(p);

    // removes item
    delete item;
    item = NULL;
    ch = '.';
} // pickUpItem

// see cell.h for details
ostream& operator<< (ostream& out, const Cell &c) {
    if (c.type == Cell::Stairs) out << "\\";
    else if (c.containsItem()) out << c.item->getToken();
    else if (c.isOccupiedByPC()) out << c.pc->getToken();
    else if (c.isOccupiedByCharacter()) out << c.enemy->getToken();
    else out << c.ch;

    return out;
} // operator<< (ostream&, Cell&)
