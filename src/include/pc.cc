/*******************************************************************************
 * pc.cc
 * 
 * A module implementing race-specific functionality for player.
 * ****************************************************************************/

#include <cassert>
#include <iostream>
#include "pc.h"
#include "game.h"
#include "item.h"
#include "constants.h"
#include "enemy.h"
#include "cell.h"

using namespace std;
using namespace constants;

// constructor
PC::PC(const string race, const int maxHp, const int hp, const int atk, 
       const int def, const string ability): Player(race, maxHp, hp,atk,def, '@',ability) {}

// see pc.h for details
void PC::modifyHP(const int amount) {
    Player::modifyHP(amount);
    if (hp == 0) {
        throw PCSlain();
    } // if
} // PC::modifyHP()

// see pc.h for details
void PC::dodgesAttack(Player &attacker) {
    if (rand() % PC_DODGE_DENOM) {
        addAction("Player character dodges attack");
        throw AttackDodged();
    } // if
} // PC::dodgesAttack()

// see pc.h for details
void PC::makeMove(const string dir) {
    Cell &target = findTargetCellMove(dir);
    
    // direction as a two-character string
    string direction = getFullDirection(dir);
    addAction("Player Character moves " + direction);

    // picks up any item in target cell
    if (target.containsItem()) {
        try {
            target.pickUpItem(*this);
        } catch (DragonAlive &e) {
            // cannot pick up dragon hoard until dragon is dead
            // move is invalidated
            actions.pop_back();
            throw e;
        } // catch
    } // else
    
    // if PC reached stairs, end the level
    if (target.isStairs()) throw(ReachedStairs());
    
    // performs the move
    getLocation()->pcMovesOut();
    target.characterMovesIn(*this);
} // PC::makeMove()

// see pc.h for details
Enemy& PC::PCAttackPrep(const std::string dir) {
    Cell &target = findTargetCellAttack(dir);

    // direction as a two-character string
    string direction = constants::getFullDirection(dir);
    addAction("Player Character attacks " + direction);

    Enemy *defender = target.getEnemy();
    assert(defender);

    defender->dodgesAttack(*this);
    return *defender;
} // PC::PCAttackPrep

// see pc.h for details
void PC::attackInDirection(const string dir) {
    // prepares attack
    Enemy &defender = PCAttackPrep(dir);

    // attacks enemy
    try {
        defender.struckBy(*this);
    } catch (Enemy::EnemySlain &e) {
        // grabs items from enemy, removes it from board
        vanquishEnemy(defender, dir);
    } // catch
} // PC::attackInDirection

// see pc.h for details
void PC::trade(const string dir) {
    Merchant &merchant = findTargetMerchantTrade(dir);

    char cmd;
    do {
        merchant.printInventory();

        // gets user command
        cout << "Enter your choice or r to return to the game." << endl;
        cin >> cmd;

        if (cmd == 'r') throw Merchant::NoDeal();

        // try to perform trade
        try {
            merchant.performTransaction(*this, cmd, dir);
            // if successful, finish the trade
            return;
        } catch (Merchant::InvalidChoice &e) {
            cout << "Invalid choice. Please try again." << endl;
        } // catch
    } while (1);
} // trade()

// see pc.h for details
void PC::vanquishEnemy(Enemy &enemy, const string dir) {
    addAction(string(1, enemy.getToken()) + " is slain");

    enemy.dropGold(*this);
    stealGold();

    enemy.getLocation()->enemyDies();
} // vanquishEnemy()

// see pc.h for details
void PC::stealGold(){}

/****************************** struckBy Overloaded Methods **************************/
// see pc.h for details
void PC::struckBy(Enemy &e) {
    applyDamage(e, *this);
} // PC::struckBy(Enemy&)

// see pc.h for details
void PC::struckBy(Elf &e) {
    // elf has multiple attacks
    for (int j = 0; j < ELF_NUM_ATTACKS; ++j) {
        try {
            dodgesAttack(e);
        } catch (AttackDodged &e) {
            continue;
        } // catch
        applyDamage(e, *this);
    } // for
} // PC::struckBy(Elf&)

// see pc.h for details
void Drow::struckBy(Elf &e) {
    dodgesAttack(e);
    applyDamage(e, *this);
} // Drow::struckBy(Enemy&)

// see pc.h for details
void PC::struckBy(Orc &e) {
    applyDamage(e, *this);
} // PC::struckBy(Orc&)

// see pc.h for details
void Goblin::struckBy(Orc &e) {
    applyDamage(e, *this, ORC_ON_GOBLIN_DAMAGE_MULTIPLIER);
} // Goblin::struckBy(Orc&)

/******************************* PC Factory Method ***********************************/
// see player.h for details
PC *createRace(const char c) {
    switch(c) {
        case 's': {
            return new Shade();
        } case 'd' : {
            return new Drow();
        } case 'v' : {
            return new Vampire();
        } case 't' : {
            return new Troll();
        } case 'g' : {
            return new Goblin();
        } default : {
            throw PC::NotPCRace();
        } // default
    } // switch
} // createRace()

/******************************* PC Race Constructors ************************************/

Shade::Shade(): PC("Shade", SHADE_HP, SHADE_HP, SHADE_ATK, SHADE_DEF, SHADE_ABILITY) {}

Drow::Drow(): PC("Drow", DROW_HP, DROW_HP, DROW_ATK, DROW_DEF, DROW_ABILITY) {}

Vampire::Vampire(): PC("Vampire", INT32_MAX, VAMP_HP, VAMP_ATK, VAMP_DEF, VAMP_ABILITY) {}

Troll::Troll(): PC("Troll", TROLL_HP, TROLL_HP, TROLL_ATK, TROLL_DEF, TROLL_ABILITY) {}

Goblin::Goblin(): PC("Goblin", GOBLIN_HP, GOBLIN_HP, GOBLIN_ATK, GOBLIN_DEF, GOBLIN_ABILITY) {}

/************************** Race-Specific Overloaded Methods ********************************/
// see pc.h for details
float Shade::scoreMultiplier() const{
    return SHADE_SCORE_MULTIPLIER;
} // Shade::scoreMultiplier()

// see pc.h for details
float Drow::potionMultiplier() const {
    return DROW_POTION_MULTIPLIER;
} // Drow::potionMultiplier()

// see pc.h for details
void Vampire::attackInDirection(const string dir) {
    // prepares attack
    Enemy &defender = PCAttackPrep(dir);

    // attacks enemy
    try {
        defender.struckBy(*this);
    } catch (Enemy::EnemySlain &e) {
        // grabs items from enemy, removes it from board
        vanquishEnemy(defender, dir);
    } // catch
} // Vampire::attackInDirection

// see pc.h for details
void Troll::turnFinished(){
    modifyHP(TROLL_HP_REJUVENATION);
    addAction("HP rejuvenated by up to " + to_string(TROLL_HP_REJUVENATION));
} // Troll::turnFinished()

// see pc.h for details
void Goblin::stealGold(){
    addAction("Player Character steals " + to_string(GOBLIN_GOLD_STEAL) 
                + " gold from slain enemy");
    modifyGold(GOBLIN_GOLD_STEAL);    
} // Goblin::stealGold()