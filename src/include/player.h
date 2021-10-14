#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include "potionDecorator.h"
#include "game.h"

class Cell;
class PC;
class Goblin;
class Merchant;

class Player {
  protected:
    const std::string race;
    const int maxHp;
    int hp;
    int atk;
    int def;
    int gold;
    const char token;
    const std::string ability;

    Cell *location;

    // queue of actions to be printed
    std::vector<std::string> actions;

    PotionDecorator *decorators;

    /*******************************************************************************
     * Player constructor
     * notes: a Player cannot be constructed directly; must use factory method
     * time: O(1)
     * *****************************************************************************/
    Player(const std::string race, const int maxHp, const int hp, const int atk, 
            const int def, const char token, const std::string ability = "");
    
    /*******************************************************************************
     * Player destructor frees potion decorators
     * notes: a Player cannot be destroyed directly
     * time: O(#potion decorators)
     * *****************************************************************************/    
    virtual ~Player();
           
  public: 

    /*******************************************************************************
     * addAction(action) adds action to end of queue of actions
     * time : O(1)
     * *****************************************************************************/
    void addAction (const std::string action); 

    /*******************************************************************************
     * attachDecorator(pd) adds pd to decorators
     * effects: make pd the front of linked list of decorators
     * time : O(1)
     * *****************************************************************************/
    void attachDecorator(PotionDecorator &pd);

    /*******************************************************************************
     * clearDecorators() deletes decorators
     * effects: frees memory, sets decorators to NULL
     * time : O(#decorators)
     * *****************************************************************************/
    void clearDecorators();

    // getter for decorators
    PotionDecorator *getDecorators() const;

    // getter for token
    char getToken() const;

    // setter for location
    void setLocation(Cell *c);

    // getter for location
    Cell* getLocation() const;

    /*******************************************************************************
     * modifyGold(sum) updates gold by sum
     * required: resulting amount must be >= 0 (checked)
     * time: O(1)
     * *****************************************************************************/
    void modifyGold(const int sum);

    // getter for gold
    int getGold() const;

    /*******************************************************************************
     * getAtk() gets Atk value 
     * notes: applies temporary potion decorators to the base Atk value
     * time: O(#potion decorators)
     * *****************************************************************************/
    int getAtk() const;

    /*******************************************************************************
     * getDef() gets Def value 
     * notes: applies temporary potion decorators to the base Def value
     * time: O(#potion decorators)
     * *****************************************************************************/
    int getDef() const;

    /*******************************************************************************
     * scoreMultiplier() gets race-specific score multiplier
     * notes: default value is 1
     * time: O(1)
     * *****************************************************************************/
    virtual float scoreMultiplier() const;

    /*******************************************************************************
     * potionMultiplier() gets race-specific potion effect multiplier
     * notes: default value is 1
     * time: O(1)
     * *****************************************************************************/
    virtual float potionMultiplier() const;

    /*******************************************************************************
     * turnFinished() makes any race-specific changes at the end of the turn
     * notes: default behavior is to do nothing
     * time : O(1)
     * *****************************************************************************/
    void turnFinished();

    /*******************************************************************************
     * modifyHP(amount) updates HP by amount
     * notes: HP does not go below 0 or above race maximum
     * time: O(1)
     * *****************************************************************************/
    virtual void modifyHP(const int amount);

    /*******************************************************************************
     * applyDamage(attacker, pc, attackMultiplier) applies attack by attacker to this
     * notes: attackMultiplier is 1 by default
     * effects: updates hp by appropriate amount
     *          adds appropriate msg to pc's action
     * time: O(1)
     * *****************************************************************************/
    void applyDamage(Player &attacker, PC &pc, const float attackMultiplier = 1);

    // exception to be thrown to indicate an attack was dodged
    class AttackDodged : public GameException {};

    /*******************************************************************************
     * dodgesAttack(attacker) determines if this player dodges an attack by attacker
     * notes: dodge happens with race-specific probability
     * exceptions: if attack is dodged, throws AttackDodged()
     * time: O(1)
     * *****************************************************************************/
    virtual void dodgesAttack(Player &attacker) = 0;

    /*******************************************************************************
     * findTargetCellMove(dir) tries to get Cell for moving player in
     *  direction dir
     * exceptions: may throw UnwalkableCell() or OccupiedCell() as appropriate
     * time: O(#neighbors of this player's cell)
     * *****************************************************************************/
    Cell& findTargetCellMove(const std::string dir);
    
    /*******************************************************************************
     * findTargetCellAttack(dir) tries to get Cell for attacking player in
     *  direction dir
     * exceptions: may throw UnwalkableCell() or UnoccupiedCell() as appropriate
     * time: O(#neighbors of this player's cell)
     * *****************************************************************************/
    Cell& findTargetCellAttack(const std::string dir);

    /*******************************************************************************
     * findTargetMerchantTrade(dir) tries to get Merchant for trading player in
     *  direction dir
     * exceptions: may throw NonMerchant() or HostileTrader() as appropriate
     * time: O(#neighbors of this player's cell)
     * *****************************************************************************/
    Merchant& findTargetMerchantTrade(const std::string dir);

    /*******************************************************************************
     * operator<< prints hp, atk, def, gold stats and buffer of actions
     * time : O(#actions)
     * *****************************************************************************/
    friend std::ostream& operator<< (std::ostream& out, Player &p);
};



#endif