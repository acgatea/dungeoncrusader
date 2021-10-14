/*******************************************************************************
 * enemy.cc
 * 
 * A module implementing race-specific enemies
 * ****************************************************************************/

#include <cassert>
#include <iostream>
#include "constants.h"
#include "enemy.h"
#include "item.h"
#include "pc.h"
#include "cell.h"

using namespace std;
using namespace constants;

// constructor
Enemy::Enemy(const std::string race, const int hp, const int atk, 
            const int def, const char token, const std::string ability)
                : Player(race,hp,hp,atk,def, token, ability) {}

// see enemy.h for details
void Enemy::modifyHP(const int amount) {
    Player::modifyHP(amount);
    if (hp == 0) {
        throw EnemySlain();
    } // if
} // Enemy::modifyHP()

// see enemy.h for details
void Enemy::dodgesAttack(Player &attacker) {}

// see enemy.h for details
void Enemy::initiateTalks() const {throw NotInterested();}

// see enemy.h for details
void Enemy::makeMove(const string dir) {
    Cell &target = findTargetCellMove(dir);

    // enemies cannot move in a cell that is not a floor tile or contains an item
    if (!target.isFloorTile()) throw UnwalkableCell();
    if (target.containsItem()) throw CellContainsItem();

    // performs the move
    getLocation()->enemyMovesOut();
    target.characterMovesIn(*this);
} // Enemy::makeMove()

// see enemy.h for details
PC& Enemy::enemyAttackPrep(const std::string dir){
    Cell &target = findTargetCellAttack(dir);

    // enemy can only attack PC 
    if (!target.isOccupiedByPC()) throw PCNotInCell();

    PC *pc = target.getPC();
    assert(pc);

    pc->dodgesAttack(*this);
    return *pc;
} // Enemy::enemyAttackPrep()

// see enemy.h for details
void Enemy::attackInDirection(const string dir) {
    // attacks PC
    enemyAttackPrep(dir).struckBy(*this);
} // Enemy::attackInDirection()

// see enemy.h for details
void Enemy::dropGold(PC& pc) const{
    const int i = rand() % DROP_GOLD_PROB_DENOM;
    if (i == 0) (new SmallHoard())->pickUp(pc);
    else (new NormalHoard())->pickUp(pc);
} // Enemy::dropGold(PC&)

/****************************** struckBy Overloaded Methods **************************/
// see enemy.h for details
void Enemy::struckBy(PC& pc){
    applyDamage(pc, pc);
} // Enemy::struckBy(PC&)

// see enemy.h for details
void Enemy::struckBy(Vampire &pc) {
    pc.addAction("Player Character gains " + to_string(VAMP_HP_GAIN) 
                 + " HP from attacking");
    pc.modifyHP(VAMP_HP_GAIN);
    applyDamage(pc, pc);
} // Enemy::struckBy(Vampire&)

// see enemy.h for details
void Dwarf::struckBy(Vampire& pc) {
    pc.addAction("Player Character loses " + to_string(-VAMP_HP_LOSS_DWARF) 
                 + " HP from attacking W");
    pc.modifyHP(VAMP_HP_LOSS_DWARF);
    applyDamage(pc, pc);
} // Dwarf::struckBy(Vampire&)

// see enemy.h for details
void Merchant::struckBy(PC& pc) {
    makeHostile(pc);
    Enemy::struckBy(pc);
} // Merchant::struckBy(PC&)

// see enemy.h for details
void Merchant::struckBy(Vampire& pc) {
    makeHostile(pc);
    Enemy::struckBy(pc);
} // Merchant::struckBy(Vampire&)

/****************************** Enemy Factory Method *********************************/
// see enemy.h for details
Enemy * createEnemy() {
    int random = rand() % ENEMY_DENOMINATOR;
    if (random < PROB_HUMAN) return new Human();
    random -= PROB_HUMAN;
    if (random < PROB_DWARF) return new Dwarf();
    random -= PROB_DWARF;
    if (random < PROB_HALFLING) return new Halfling();
    random -= PROB_HALFLING;
    if (random < PROB_ELF) return new Elf();
    random -= PROB_ELF;
    if (random < PROB_ORC) return new Orc();
    return new Merchant();
} // createEnemy()

/****************************** Enemy Race Constructors **********************************/
Human::Human(): Enemy("Human", HUMAN_HP, HUMAN_ATK, HUMAN_DEF, 'H', HUMAN_ABILITY) {}

Dwarf::Dwarf(): Enemy("Dwarf", DWARF_HP, DWARF_ATK, DWARF_DEF, 'W', DWARF_ABILITY) {}

Elf::Elf(): Enemy("Elf", ELF_HP, ELF_ATK, ELF_DEF, 'E', ELF_ABILITY) {}

Orc::Orc(): Enemy("Orc", ORC_HP, ORC_ATK, ORC_DEF, 'O', ORC_ABILITY) {}

Merchant::Merchant(): Enemy("Merchant", MERCH_HP, MERCH_ATK, MERCH_DEF,'M'), 
                      fee(1 + rand() % (MERCH_MAX_FEE-1)) {
    generatePotions();
}

Dragon::Dragon(): Enemy("Dragon", DRAGON_HP, DRAGON_ATK, DRAGON_DEF, 'D', DRAGON_ABILITY) {}

Halfling::Halfling(): Enemy("Halfling", HALFLING_HP, HALFLING_ATK, HALFLING_DEF, 'L', HALFLING_ABILITY) {}

// Merchant destructor
Merchant::~Merchant(){
    // deletes potions
    for (vector<Potion *>::iterator it = inventory.begin(); it != inventory.end(); ++it) {
        delete (*it);
    } // for
} // ~Merchant()

// Dragon destructor
Dragon::~Dragon(){
    dh->detachDragon();
} // ~Dragon()

/************************** Race-Specific Overloaded Methods ********************************/

// see enemy.h for details
void Human::dropGold(PC& pc) const{
    for (int j = 0; j < HUMAN_GOLD_PILES; ++j) {
        (new NormalHoard())->pickUp(pc);
    } // for   
} // Human::dropGold()

// see enemy.h for details
void Elf::attackInDirection(const string dir) {
    // attacks PC
    enemyAttackPrep(dir).struckBy(*this);
} // Elf::attackInDirection()

// see enemy.h for details
void Orc::attackInDirection(const string dir) {
    // attacks PC
    enemyAttackPrep(dir).struckBy(*this);
} // Orc::attackInDirection()

// merchants are not hostile initially
bool Merchant::isHostile = false;

// see enemy.h for details
void Merchant::generatePotions(){
    assert(inventory.empty());
    const int numPotions = rand() % MERCH_MAX_NUM_POTIONS;
    for (int j = 0; j < numPotions; ++j) {
        inventory.push_back((Potion *) createItem('P'));
    } // for
} // Merchant::generatePotions()

// see enemy.h for details
void Merchant::makeHostile(PC &pc){
    if (!isHostile) {
        isHostile = true;
        pc.addAction("Merchants are now hostile to Player Character");
    } // if
} // makeHostile()

// see enemy.h for details
void Merchant::resetHostile() {
    isHostile = false;
} // resetHostile()

// see enemy.h for details
bool Merchant::areMerchantsHostile() {
    return isHostile;
} // areMerchantsHostile()

// see enemy.h for details
void Merchant::initiateTalks() const {
    if (areMerchantsHostile() || inventory.empty() || rand() % MERCH_PROB_REFUSE == 0) {
        throw NotInterested();
    } // if
} // Merchant::initiateTalks()

// see enemy.h for details
void Merchant::printInventory() const {
    cout << "Merchant's Inventory:" << endl;
    for (vector<Potion *>::const_iterator it = inventory.begin(); it != inventory.end(); ++it) {
        cout << '(' << (char) ('a' + it - inventory.begin()) << ") " << **it << endl;
    } // for
    cout << "Fee: " << fee << endl << endl;
} // printInventory()

// see enemy.h for details
void Merchant::performTransaction(Player &pc, const char choice, const string dir){
    if('a' > choice || choice >= 'a' + inventory.size()) throw InvalidChoice();
    const int index = choice - 'a';

    // checks that PC has enough money
    if (pc.getGold() < fee) {
        cout << "Player Character has insufficient funds." << endl;
        throw NoDeal();
    } // if

    // direction as a two-character string
    string direction = constants::getFullDirection(dir);
    pc.addAction("Player Character trades with merchant " + direction);

    // performs transaction
    pc.modifyGold(-fee);
    pc.addAction("Player Character transfers " + to_string(fee) + " gold to M");

    // determines whether merchant will steal
    if (rand() % MERCH_STEAL_PROB_DENOM) {
        // cannot steal more gold than PC has
        const int steal = min ((rand() % MERCH_MAX_STEAL), pc.getGold());
        if (steal > 0) {
            pc.modifyGold(-steal);
            pc.addAction("M steals an additional " + to_string(steal) + " gold from Player Character");
        } // if
    } // if

    // gives item to PC
    inventory.at(index)->pickUp(pc);

    // remove potion that was bought from merchant's inventory
    delete inventory.at(index);
    inventory.erase(inventory.begin() + index);
} // performTransaction()

// see enemy.h for details
void Merchant::attackInDirection(const string dir) {
    if (!Merchant::isHostile) throw NotHostile();
    // attacks PC
    enemyAttackPrep(dir).struckBy(*this);
} // Merchant::attackInDirection()

// see enemy.h for details
void Merchant::dropGold(PC& pc) const{
    (new MerchantHoard())->pickUp(pc);
} // Merchant::dropGold()

// see enemy.h for details
void Dragon::makeMove(const string dir) {}

// see enemy.h for details
void Dragon::attachDH(DragonHoard &hoard) {
    dh = &hoard;
} // attachDH()

// see enemy.h for details
void Dragon::dropGold(PC& pc) const{}

// see enemy.h for details
void Halfling::dodgesAttack(Player &attacker) {
    if (rand() % HALFLING_MISS_DENOM) {
        attacker.addAction("L beguiles the Player Character and causes them to miss");
        throw AttackDodged();
    } // if
} // Halfling::dodgesAttack()