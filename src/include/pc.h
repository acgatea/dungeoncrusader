#ifndef __PC_H__
#define __PC_H__

#include <exception>
#include "player.h"

class Enemy;
class Orc;
class Elf;

class PC : public Player {
  protected:
    /*******************************************************************************
     * PC constructor
     * notes: a PC cannot be constructed directly; must use factory method
     * time: O(1)
     * *****************************************************************************/
    PC(const std::string race, const int maxHp, const int hp, 
       const int atk, const int def, const std::string ability = "");

    /*******************************************************************************
     * PCAttackPrep(dir) prepares a PC attack on cell in direction dir, returns
     *  reference to enemy to be attacked
     * notes: the attack is not performed if the corresponding square is 
     *        not occupied by enemy
     * requires: dir must be one of no,so,ea,we,ne,nw,se,sw
     * exceptions: may throw InvalidMove() if move is not valid, or AttackDodged()
     *             if attack is dodged
     * effects: adds messages to PC's action
     * time: O(#neighbors of PC's location)
     * *****************************************************************************/  
    Enemy& PCAttackPrep(const std::string dir);

  public:
    // exception to be thrown when selected race is invalid
    class NotPCRace : public GameException {};

    /*******************************************************************************
     * modifyHP(amount) updates HP by amount
     * notes: HP does not go below 0 or above race maximum
     * exceptions: throws PCSlain() if resulting hp is 0
     * time: O(1)
     * *****************************************************************************/
    void modifyHP(const int amount);

    /*******************************************************************************
     * dodgesAttack(attacker) determines if this player dodges an attack by attacker
     * notes: dodge happens with race-specific probability
     * exceptions: if attack is dodged, throws AttackDodged()
     * time: O(1)
     * *****************************************************************************/
    void dodgesAttack(Player &attacker);

    /*******************************************************************************
     * makeMove(dir) attempts to move PC in direction dir
     * notes: the move is not performed if the corresponding square is 
     *        not walkable or is occupied by enemy or contains a guarded dragon hoard
     * requires: dir must be one of no,so,ea,we,ne,nw,se,sw
     * effects: changes location of PC if move is valid
     *          adds messages to PC's action
     * exceptions: may throw InvalidMove() if move is not valid
     * time: O(#neighbors of PC's location)
     * *****************************************************************************/
    void makeMove(const std::string dir);

    /*******************************************************************************
     * attackInDirection(dir) attempts to have PC attack cell in direction dir
     * notes: the attack is not performed if the corresponding square is 
     *        not occupied by enemy
     * requires: dir must be one of no,so,ea,we,ne,nw,se,sw
     * exceptions: may throw InvalidMove() if attack is not valid, or AttackDodged()
     * time: O(#neighbors of PC's location)
     * *****************************************************************************/
    virtual void attackInDirection(const std::string dir);

    /*******************************************************************************
     * trade(dir) attempts to have PC trade with merchant in cell in direction dir
     * notes: the trade is not performed if the corresponding square is 
     *        not occupied by merchant
     * requires: dir must be one of no,so,ea,we,ne,nw,se,sw
     * exceptions: may throw InvalidMove() if trade is not valid, NoDeal()
     *             could not reach a deal
     * time: O(#neighbors of PC's location)
     * *****************************************************************************/
    void trade(const std::string dir);

    /*******************************************************************************
     * vanquishEnemy(enemy, dir) loots and disposes of corpse of enemy in 
     *  direction dir
     * requires: enemy has hp 0 (not checked)
     *           dir must be one of no,so,ea,we,ne,nw,se,sw
     * effects: grabs any gold / items left dropped by enemy
     *          deletes enemy
     * time: O(1)
     * *****************************************************************************/
    void vanquishEnemy(Enemy &enemy, const std::string dir);

    /*******************************************************************************
     * stealGold() determines, based on pc run-time type, if it steals gold from
     *  slain enemy
     * notes: will only be called after an enemy is slain
     *        default PC doesn't steal gold
     * effects: might increase gold of pc
     * *****************************************************************************/
    virtual void stealGold();

    /*******************************************************************************
     * struckBy(e) applies damage to PC from attack(s) by enemy e 
     * exceptions: throws PCSlain() if appropriate
     * effects: changes hp of PC
     *          adds messages to action
     * time: O(#attacks)
     * *****************************************************************************/
    virtual void struckBy(Enemy &e);
    virtual void struckBy(Orc &e);
    virtual void struckBy(Elf &e);
};

/*******************************************************************************
 * factory method for PC races
 * createRace(c) creates a pc with the race indicated by c
 * exception: throws NotPCRace() if c is not one of s, d, v, t, g
 * time: O(1) 
 * *****************************************************************************/
PC *createRace(const char c);

/***************************** DIFFERENT PC RACES ******************************/
class Shade: public PC {
  public:
    Shade();
    
    // see Player::scoreMultiplier()
    float scoreMultiplier() const;
};

class Drow: public PC {  
  public:
    Drow();

    // see Player::potionMultiplier()
    float potionMultiplier() const;

    // see PC::struckBy()
    void struckBy(Elf &e);
};

class Vampire: public PC {
  public:
    Vampire();

    // see PC::attackInDirection()
    void attackInDirection(const std::string dir);
};

class Troll: public PC {
  public:
    Troll();

    // see Player::turnFinished()
    void turnFinished();
};

class Goblin: public PC {
  public:
    Goblin();

    // see PC::stealGold()
    void stealGold();
  
    // see PC::struckBy()
    void struckBy(Orc &e);
};

#endif