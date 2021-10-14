#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <string>
#include "game.h"
#include "potion.h"
#include "pc.h"

class DragonHoard;

class Enemy : public Player {
  protected:
    /*******************************************************************************
     * Enemy constructor
     * notes: an Enemy cannot be constructed directly; must use factory method
     * time: O(1)
     * *****************************************************************************/
    Enemy(const std::string race, const int hp, const int atk, 
          const int def, const char token, const std::string ability = "");

    /*******************************************************************************
     * enemyAttackPrep(dir) prepares an enemy attack on cell in direction dir, 
     *  returns reference to PC
     * notes: the attack is not performed if the corresponding square is 
     *        not occupied by PC
     * requires: dir must be one of no,so,ea,we,ne,nw,se,sw
     * exceptions: may throw InvalidMove() if move is not valid, or AttackDodged()
     *             if attack is dodged
     * effects: may add messages to PC's action
     * time: O(#neighbors of enemy's location)
     * *****************************************************************************/  
    PC& enemyAttackPrep(const std::string dir);

  public:
  
    class EnemySlain : public GameException {};

    // to be thrown if non-hostile character tries to attack PC
    class NotHostile : public GameException {};

    /*******************************************************************************
     * modifyHP(amount) updates HP by amount
     * notes: HP does not go below 0 or above race maximum
     * exceptions: throws EnemySlain() if resulting hp is 0
     * time: O(1)
     * *****************************************************************************/
    void modifyHP(const int amount);

    /*******************************************************************************
     * dodgesAttack(attacker) determines if this player dodges an attack by attacker
     * notes: dodge happens with race-specific probability
     *        by default, enemies do not dodge PC attacks
     * exceptions: if attack is dodged, throws AttackDodged()
     * time: O(1)
     * *****************************************************************************/
    virtual void dodgesAttack(Player &attacker);

    /*******************************************************************************
     * initiateTalks() determines if this enemy is interested in trading with PC
     * notes: non-merchants or hostile merchants are never interested
     * exceptions: if not interested, throws NotInterested()
     * time: O(1)
     * *****************************************************************************/
    virtual void initiateTalks() const;

    /*******************************************************************************
     * makeMove(dir) attempts to move enemy in direction dir
     * notes: the move is not performed if the corresponding square is 
     *        not walkable or is occupied by item/character
     * requires: dir must be one of no,so,ea,we,ne,nw,se,sw
     * effects: changes location of enemy if move is valid
     * exceptions: may throw InvalidMove() if move is not valid
     * time: O(#neighbors of enemy's location)
     * *****************************************************************************/
    virtual void makeMove(const std::string dir);

    /*******************************************************************************
     * attackInDirection(dir) attempts to have enemy attack cell in direction dir
     * notes: the attack is not performed if the corresponding square is 
     *        not occupied by PC
     * requires: dir must be one of no,so,ea,we,ne,nw,se,sw
     * exceptions: may throw InvalidMove() if attack is not valid, or AttackDodged()
     *             if attack is dodged
     * effects: adds messages to PC's action
     *          updates PC's hp
     * time: O(#neighbors of enemy's location)
     * *****************************************************************************/
    virtual void attackInDirection(const std::string dir);

    /*******************************************************************************
     * dropGold(pc) has pc pick up some gold piles
     * notes: to be called only upon death of enemy
     * effects: adds messages to PC's action
     *          updates PC's gold
     * time: O(#gold items dropped)
     * *****************************************************************************/
    virtual void dropGold(PC& pc) const;

    /*******************************************************************************
     * struckBy(e) applies damage to enemy from attack(s) by pc
     * exceptions: throws EnemySlain() if appropriate
     * effects: changes hp of enemy
     *          adds messages to PC's action
     * time: O(#attacks)
     * *****************************************************************************/
    virtual void struckBy(PC& pc);
    virtual void struckBy(Vampire& pc);
};

/*******************************************************************************
 * factory method for Enemy races
 * createEnemy() generates an enemy with prescribed probabilities 
 * time: O(1) 
 * *****************************************************************************/
Enemy * createEnemy();

/*************************** DIFFERENT ENEMY RACES *****************************/
class Human : public Enemy {
  public:
    Human();

    // see Enemy::dropGold()
    void dropGold(PC& pc) const;
};

class Dwarf : public Enemy {
  public:
    Dwarf();

    // see Enemy::struckBy(Vampire&)
    void struckBy(Vampire& v);
};

class Elf : public Enemy {
  public:
    class ElfCannotAttackAgain : public GameException {};

    Elf();

    // see Enemy::attackInDirection()
    void attackInDirection(const std::string dir);
};

class Orc : public Enemy {
  public:
    Orc();
    // see Enemy::attackInDirection()
    void attackInDirection(const std::string dir);
};

class Merchant : public Enemy {
  const int fee;
  static bool isHostile;

  std::vector<Potion *> inventory;

  /*******************************************************************************
   * generatePotions() fills potions with a random potions
   * required: potions must be empty
   * time: nondeterministic 
   * *****************************************************************************/
  void generatePotions();

  public:
    Merchant();
    ~Merchant();

    // thrown when no deal is reached with merchant
    class NoDeal : public GameException {};

    // thrown when PC choose invalid inventory item
    class InvalidChoice : public GameException {};

    /*******************************************************************************
     * makeHostile() makes all merchants hostile to PC
     * effects: adds message to pc's action
     * time: O(1) 
     * *****************************************************************************/
    static void makeHostile(PC &pc);

    // resetter for isHostile
    static void resetHostile();

    // getter for hostile flag
    static bool areMerchantsHostile();

    // see Enemy::initiateTalks()
    // merchants refuse to talk if hostile or with prescribed probability
    void initiateTalks() const;

    /*******************************************************************************
     * printsInventory() prints merchant's potions
     * time: O(|potions|) 
     * *****************************************************************************/    
    void printInventory() const;

    /*******************************************************************************
     * performTransaction(pc, choicem dir) gets fee from pc in exchange for potion
     *  dir indicates direction of merchant
     * required: choice is between 'a' and 'a' + potions.size()
     * effects: updates PC's gold and action
     *          applies item to PC
     * exceptions : may throw NoDeal() or InvalidChoice()
     * time: O(1) 
     * *****************************************************************************/
    void performTransaction(Player &pc, const char choice, const std::string dir);

    // see Enemy::attackInDirection()
    void attackInDirection(const std::string dir);

    // see Enemy::dropGold()
    void dropGold(PC& pc) const;

    // see Enemy::struckBy(PC&)
    void struckBy(PC& pc);

    // see Enemy::struckBy(Vampire&)
    void struckBy(Vampire& pc);
};

class Dragon : public Enemy {
    DragonHoard * dh;
  public:
    Dragon();
    ~Dragon();
    
    // empty method (dragons don't move)
    void makeMove(const std::string dir);

    // setter for dh
    void attachDH(DragonHoard &hoard);

    // see Enemy::dropGold()
    void dropGold(PC& pc) const;
};

class Halfling : public Enemy {
  public:
    Halfling();

    // see Enemy::dodgesAttack()
    void dodgesAttack(Player &attacker);
};

#endif