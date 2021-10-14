#ifndef __CELL_H__
#define __CELL_H__

#include <map>
#include <iostream>
#include "floor.h"
#include "game.h"
#include "pc.h"

class Enemy;
class Item;
class DragonHoard;

class Cell {
    // coordinates
    const int row;
    const int col;

    // chamber number if this is a floor tile; -1 for non-floor tiles
    const int chamberNum;
    
    // character representation
    char ch;

    enum TileType {FloorTile, Doorway, Passageway, Wall, Stairs, Empty, NUM_TILE_TYPES};
    TileType type;

    // (direction, Cell) pairs for cells in 1 block radius
    std::map<std::string, Cell &> neighbors;

    // floor containing the cell
    Floor &fl;

    // possible occupant of the cell
    Item *item;
    PC *pc;
    Enemy *enemy;

  public:   

    /*******************************************************************************
     * Cell constructor
     * required: ch is one of '.','\','|','-','+','#','0'-'9'(gold/potion),
     *           '@' (PC), 'H' 'D' 'E' 'O' 'W' 'L' 'M' (enemy)
     * effects: creates enemies/items as needed
     *          enemies must be freed by caller; items will be freed by cell dtor
     *          if enemy, stores in fl.enemies
     *          if dragon, stores in fl.dragons
     *          if '@', moves pc to this cell 
     * time: O(1)
     * *****************************************************************************/
    Cell (const int row, const int col, const int chamberNum, const char ch, PC& pc, Floor &fl);

    /*******************************************************************************
     * Cell destructor
     * effects: frees memory for any item it contains
     * time : O(1)
     * *****************************************************************************/
    ~Cell();

    /*******************************************************************************
     * addNeighbor(dir, neighbor) adds pair (dir, neighbor) to list of neighbors 
     * *****************************************************************************/
    void addNeighbor(const std::string dir, Cell &neighbor);

    /*******************************************************************************
     * getter for walkable neighbor in direction dir
     * notes: returns NULL if no walkable neighbor in that direction
     * time : O(#neighbors)
     * *****************************************************************************/
    Cell* getNeighbor(std::string dir) const;

    /*******************************************************************************
     * generateDragon (dh) creates a dragon in 1 block radius of dh
     * exceptions: if no space for dragon, throws NoSpaceForDragon
     * time : O(#neighbors)
     * *****************************************************************************/
    Dragon& generateDragon (DragonHoard &dh) const;

    // exception to be thrown if a DragonHoard (DH) was created
    // but there is no space for a Dragon in 1 block radius
    class NoSpaceForDragon : public GameException {};

    // getter for enemy in this cell
    Enemy* getEnemy() const;

    // getter for pc in this cell
    PC* getPC() const;

    // getter for item in this cell
    Item* getItem() const;

    // getter for chamberNum
    int getChamberNum() const;

    // sets this cell as stairs
    void setStairs();

    /*******************************************************************************
     * characterMovesIn(p) moves PC p to this cell
     * required: cell must be currently unoccupied by characters
     * effects: stores p in this cell and updates location of p
     * *****************************************************************************/
    void characterMovesIn(PC &p);

    /*******************************************************************************
     * characterMovesIn(e) moves Enemy e to this cell
     * required: cell must be currently unoccupied by characters
     * effects: stores e in this cell and updates location of e
     * time: O(1)
     * *****************************************************************************/
    void characterMovesIn(Enemy &e);

    /*******************************************************************************
     * pcMovesOut() kicks out PC out of this cell
     * required: PC must be currently in this cell
     * effects: updates location of pc to NULL, sets pc to NULL
     * time : O(1)
     * *****************************************************************************/
    void pcMovesOut();

    /*******************************************************************************
     * enemyMovesOut() kicks out Enemy out of this cell
     * required: an enemy must be currently in this cell
     * effects: updates location of enemy to NULL, sets enemy to NULL
     * time : O(1)
     * *****************************************************************************/    
    void enemyMovesOut();

    /*******************************************************************************
     * enemyDies() removes enemy from floor
     * required: an enemy must be currently in this cell
     * effects: updates location of enemy to NULL, sets enemy to NULL
     * time : O(#enemies on floor)
     * *****************************************************************************/
    void enemyDies();

    // predicate for floor tile
    bool isFloorTile() const;

    // predicate for stairs
    bool isStairs() const;

    // predicate for walkable tile (floor, doorway, passageway, stairs)
    bool isWalkableTile() const;

    // predicate testing if this tile is occupied by a character (PC or enemy)
    bool isOccupiedByCharacter() const;

    // predicate testing if this tile is occupied by PC
    bool isOccupiedByPC() const;

    // predicate testing if this tile contains an Item
    bool containsItem() const;

    /*******************************************************************************
     * depositItem(i) stores i at this cell
     * required: this cell not previously contain an item or character
     * *****************************************************************************/
    void depositItem (Item &i);

    /*******************************************************************************
     * pickUpItem(p) has p pick up the item in this cell
     * required: this cell contains an item
     * effects: modifies p according to the item's type
     *          deletes the item in this cell
     * exceptions: throws DragonAlive() if item is a dragon
     *             hoard whose dragon is still alive
     * time: O(1)
     * *****************************************************************************/
    void pickUpItem(Player &p);

    /*******************************************************************************
     * << overload
     * effects: prints token for this cell based on its type and 
     *          its occupant (PC/enemy/item)
     * time : O(1)
     * *****************************************************************************/
    friend std::ostream& operator<< (std::ostream& out, const Cell &c);
};

#endif