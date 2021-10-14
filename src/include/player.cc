/*******************************************************************************
 * player.cc
 * 
 * A module implementing generic player functionality
 * ****************************************************************************/

#include <cassert>
#include <iostream>
#include <iomanip>
#include "constants.h"
#include "cell.h"
#include "player.h"
#include "enemy.h"
#include "pc.h"

using namespace std; 

// see player.h for details
Player::Player(const string race, const int maxHp, const int hp, const int atk, 
               const int def, const char token, const std::string ability)
                : race(race), maxHp(maxHp), hp(hp), atk(atk), def(def), gold(0), 
                    token(token), ability(ability), location(0), decorators(0) {}

// see player.h for details
Player::~Player(){
    delete decorators;
}

// see player.h for details
void Player::addAction (const string action) {
    actions.push_back(action);
} // addAction()

// see player.h for details
void Player::attachDecorator(PotionDecorator &pd){
    decorators = &pd;
} // attachDecorator()

// see player.h for details
void Player::clearDecorators(){
    addAction("Effects of Player Characters' temporary potions are gone");
    delete decorators;
    decorators = NULL;
} // clearDecorators()

// see player.h for details
PotionDecorator* Player::getDecorators() const{
    return decorators;
} // getDecorators()

// see player.h for details
char Player::getToken() const {
    return token;
} // getToken()
    
// see player.h for details
void Player::setLocation(Cell *c){
    location = c;
} // setLocation()

// see player.h for details
Cell* Player::getLocation() const{
    return location;
} // getLocation()

// see player.h for details
void Player::modifyGold(const int sum){
    gold += sum;
    assert(gold >= 0);
} // modifyGold()

// see player.h for details
int Player::getGold() const{
    return gold;
} // getGold()

// see player.h for details
int Player::getAtk() const{
    if (!decorators) return atk;
    float newAtk = atk;
    decorators->getAtk(newAtk);
    return max((int) newAtk,0);
} // getAtk()

// see player.h for details
int Player::getDef() const {
    if (!decorators) return def;
    float newDef = def;
    decorators->getDef(newDef);
    return max((int) newDef,0);
} // getDef()

// see player.h for details
float Player::scoreMultiplier() const{
    return 1;
} // scoreMultiplier()

// see player.h for details
float Player::potionMultiplier() const{
    return 1;
} // potionMultiplier()

// see player.h for details
void Player::turnFinished(){}

// see player.h for details
void Player::modifyHP(const int amount){
    if (amount < 0) {
        hp = max(hp + amount, 0);
    } else {
        hp = min(hp + amount, maxHp);
    } // if
} // modifyHP()

// see player.h for details
void Player::applyDamage(Player& attacker, PC& pc, const float attackMultiplier){
    const int damage = constants::computeDamage(attacker.getAtk(), def) * attackMultiplier;
    pc.addAction(string(1, attacker.getToken()) + " does " + to_string(damage) + " damage to "
                        + token + " (" + to_string(max(hp-damage,0)) + " HP)");
    modifyHP(-damage);
} // applyDamage()

// see player.h for details
Cell& Player::findTargetCellMove(const string dir) {
    // get corresponding cell from list of neighbors
    Cell *target = location->getNeighbor(dir);

    // player cannot move to a cell that is not walkable or is occupied by a character
    if (!target) throw UnwalkableCell();
    if (target->isOccupiedByCharacter()) throw OccupiedCell();

    return *target;
} // findTargetCell()

// see player.h for details
Cell& Player::findTargetCellAttack(const string dir) {
    // get corresponding cell from list of neighbors
    Cell *target = location->getNeighbor(dir);

    // cannot attack a cell that is not walkable or is not occupied by a character
    if (!target) throw UnwalkableCell();
    if (!target->isOccupiedByCharacter()) throw UnoccupiedCell();

    return *target;
} // Player::attackInDirection()

// see player.h for details
Merchant& Player::findTargetMerchantTrade(const string dir) {
    // get corresponding cell from list of neighbors
    Cell *target = location->getNeighbor(dir);

    // cannot trade with a cell that is not walkable or is not occupied 
    // by character
    if (!target || !target->isOccupiedByCharacter()) throw NonMerchant();

    // see if character is interested in trading
    target->getEnemy()->initiateTalks();

    return *((Merchant *) target->getEnemy());
} // Player::attackInDirection()

// see player.h for details
ostream& operator<< (ostream& out, Player &p){
    // prints stats
    out << left << setw(9) << "Gold: " << p.gold << "\t"
        << setw(9) << "Race: " << p.race;
    if (p.ability != "") out << " (" << p.ability << ")";
    out << endl;
    out << left << setw(9) << "HP: " << p.hp << "\t"
        << setw(9) << "Atk: " << p.getAtk() << "\t"
        << setw(9) << "Def: " << p.getDef() << endl;

    // print action if any
    if (!p.actions.empty()) {
        out << left << setw(9) << "Action: ";
        for (vector<string>::iterator it = p.actions.begin(); it != p.actions.end(); ++it) {
            if (it != p.actions.begin()) out << "\t ";
            out << (*it) << "." << endl;
        } // for
        p.actions.clear();
    } // if
    return out;
} // operator<<