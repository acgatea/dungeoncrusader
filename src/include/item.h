#ifndef __ITEM_H__
#define __ITEM_H__

#include <iostream>
#include <string>
#include "game.h"

class Player;
class Enemy;
class Dragon;
class DragonHoard;

class Item {
    const char token;

  protected:
    // constructor
    // only to be called by derived subclasses
    Item(const char token);

  public :
    virtual ~Item();
    
    // exception to be thrown when a Dragon Hoard is created
    // in order for floor to create a nearby dragon
    class DHCreated : public GameException {
        DragonHoard &dh;
      public :
        DHCreated (DragonHoard &dh);

        // getter for dh       
        DragonHoard& getDH();
    };

    // getter for token
    char getToken() const;
   
    /*******************************************************************************
     * pickup(pc) has this item be picked up by p
     * effects: changes p according to type of item
     *          stores appropriate comment in p's action
     * exceptions: throws DragonAlive() if item is a dragon
     *             hoard whose dragon is still alive
     * time: O(1)
     * *****************************************************************************/
    virtual void pickUp(Player &p) = 0;
};

/*******************************************************************************
 * createItem(type) generates an item of type type
 * notes: uses prescribed probabilities
 * required: type is 'P' or 'G'
 * time : O(1)
 * *****************************************************************************/
Item * createItem(const char type);

class Gold : public Item {
    const int value;

  protected:
    explicit Gold(const int value);
    virtual ~Gold();

  public:
    // getter for value
    int getValue() const;

    /*******************************************************************************
     * pickUp(p) adds the value of this gold pile to p's gold
     * exceptions: if this is a dragon hoard with its dragon alive,
     *             throws DragonAlive() exception
     * effects: updates p's gold amount
     * time: O(1)
     * *****************************************************************************/
    virtual void pickUp(Player &p);
};

// types of gold piles
class NormalHoard : public Gold {
  public:
    NormalHoard();

    // see Gold::pickUp()
    void pickUp(Player &p);
};

class DragonHoard : public Gold {
  // guardian (NULL if no guardian)
  Dragon * dragon;

  public:
    DragonHoard();
    ~DragonHoard();

    /*******************************************************************************
     * attachDragon(d) adds dragon guardian d to this Dragon Hoard
     * notes: to be called when dragon is created
     * effects: stores d as guardian
     * time : O(1)
     * *****************************************************************************/
    void attachDragon(Dragon &d);

    /*******************************************************************************
     * detachDragon() removes dragon guardian for this Dragon Hoard
     * required: dragon is not NULL
     * notes: to be called when dragon is killed
     * effects: sets dragon to NULL
     * time : O(1)
     * *****************************************************************************/    
    void detachDragon();

    // see Gold::pickUp()
    void pickUp(Player &p);
};

class SmallHoard : public Gold {
  public:
    SmallHoard();
    
    // see Gold::pickUp()
    void pickUp(Player &p);
};

class MerchantHoard: public Gold {
  public:
    MerchantHoard();

    // see Gold::pickUp()
    void pickUp(Player &p);
};


#endif
