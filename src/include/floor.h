#ifndef __FLOOR_H__
#define __FLOOR_H__
#include <vector>
#include <string>
#include "enemy.h"

class Cell;
class PC;

class Floor {
    // grid of cells
    Cell*** floor;

    const int floorNum;

    PC &pc;
    
    // enemies remaining on the floor (including dragons)
    std::vector<Enemy *> enemies;

    // dragons remaining on the floor
    // (to be used only while floor is constructed; this is empty during game play)
    std::vector<Dragon *> dragons;

    /*******************************************************************************
     * randFloorTile() randomly chooses an empty floor tile; the optional parameter
     *    notIn indicates (if not -1) which chamber to avoid
     * required: notIn must be one of -1, 0, ..., NUM_CHAMBERS
     * notes: each chamber other than notIn is equally likely, 
     *        each floor tile in a chamber is equally likely
     * required: srand() must have been called
     * time: nondeterministic, O(1) expected
     * *****************************************************************************/
    Cell& randFloorTile(const int notIn = -1) const;

    /*******************************************************************************
     * populates(pc) randomly populates the floor with pc, enemies, gold, potions
     * required: srand() must have been called
     *           floor must be empty [not checked]
     * time: nondeterministic, O(#enemies + #potions + #gold per floor) expected
     * *****************************************************************************/
    void populate(PC &pc);

    /*******************************************************************************
     * linkDragons() links existing Dragons with Dragon Hoards
     * notes: to be called once, immediately after floor is built from file
     *        from configuration file
     * effects: links Dragons with any nearby Dragon Hoards
     * time : O(#dragons)
     * *****************************************************************************/
    void linkDragons ();

  public:
    /*******************************************************************************
     * floor constructor create a floor from configuration in filename
     * notes: if filename is EMPTY_CONFIG, populates it randomly
     * required: filename is a valid configuration file (not checked)
     * time: nondeterministic, 
     *       O(NUM_ROWS * NUM_COLS) expected
     * *****************************************************************************/
    Floor(const int floorNum, const std::string filename, PC &pc);

    /*******************************************************************************
     * floor destructor frees memory for cells, potions, gold, enemies
     * time: O(NUM_ROWS * NUM_COLS) 
     * *****************************************************************************/   
    ~Floor();

    /*******************************************************************************
     * addEnemy(e) adds e to list of enemies
     * time: O(1) 
     * *****************************************************************************/
    void addEnemy (Enemy &e);

    /*******************************************************************************
     * addDragon(d) adds d to list of dragons
     * time: O(1) 
     * *****************************************************************************/
    void addDragon (Dragon &d);

    /*******************************************************************************
     * removeEnemy(e) removes e from list of enemies
     * time: O(|enemies|) 
     * *****************************************************************************/
    void removeEnemy(const Enemy &e);

    /*******************************************************************************
     * turnEnemies() performs one turn for every enemy on the floor
     * notes: each enemy tries to attack PC if it is in its 1 block radius
     *        if it cannot attack PC, it moves in a random direction
     *        enemies never leave their chamber or pick up items
     * time: nondeterministic, O(#enemies) expected
     * *****************************************************************************/
    void turnEnemies();

    /*******************************************************************************
     * prints current floor configuration, PC stats and buffer of actions
     * effects: clears actions buffer
     * time: O(NUM_ROWS * NUM_COLS + |actions|) 
     * *****************************************************************************/
    friend std::ostream& operator<< (std::ostream& out, const Floor &f);
};

#endif