/*******************************************************************************
 * item.cc
 * 
 * Module implementing an abstract item as well as gold pile items.
 * ****************************************************************************/

#include <cassert>
#include "item.h"
#include "player.h"
#include "enemy.h"
#include "potion.h"
#include "constants.h"

using namespace std;
using namespace constants;

// constructor
Item::Item(const char token) : token(token) {}

// destructor
Item::~Item() {}

// constructors for gold hoard types
Gold::Gold(const int value) : Item('G'), value(value) {}
SmallHoard::SmallHoard() : Gold(constants::SMALL_HOARD_VALUE) {}
NormalHoard::NormalHoard() : Gold(constants::NORMAL_HOARD_VALUE) {}
MerchantHoard::MerchantHoard() : Gold(constants::MERCHANT_HOARD_VALUE) {}
DragonHoard::DragonHoard() : Gold(constants::DRAGON_HOARD_VALUE), dragon(0) {}

// gold destructors
Gold::~Gold() {}

DragonHoard::~DragonHoard() {
    delete dragon;
} // dtor

// DHCreated() constructor
Item::DHCreated::DHCreated(DragonHoard &dh) : dh(dh) {}

// see item.h for details
DragonHoard& Item::DHCreated::getDH() {
    return dh;
} // Item::DHCreated::getDH()

// see item.h for details
void DragonHoard::attachDragon(Dragon &d){
    assert(!dragon);
    dragon = &d;
} // attachDragon()

// see item.h for details
void DragonHoard::detachDragon(){
    assert(dragon);
    dragon = NULL;
} // detachDragon()

// see item.h for details
char Item::getToken() const {
    return token;
} // Item::getToken()

// see item.h for details
void Gold::pickUp(Player &p) {
    p.modifyGold(value);
} // Gold::pickUp()

// see item.h for details
void DragonHoard::pickUp(Player &p) {
    // not allowed to be picked up if dragon is alive
    if (dragon) throw(DragonAlive());
    p.addAction("Player Character picks up a Dragon Hoard (value " + 
                to_string(constants::DRAGON_HOARD_VALUE) + ")");
    Gold::pickUp(p);
} // DragonHoard::pickUp()

// see item.h for details
void SmallHoard::pickUp(Player &p) {
    p.addAction("Player Character picks up a Small Hoard (value " + 
                to_string(constants::SMALL_HOARD_VALUE) + ")");
    Gold::pickUp(p);
} // SmallHoard::pickUp()

// see item.h for details
void NormalHoard::pickUp(Player &p) {
    p.addAction("Player Character picks up a Normal Hoard (value " + 
                to_string(constants::NORMAL_HOARD_VALUE) + ")");
    Gold::pickUp(p);
} // NormalHoard::pickUp()

// see item.h for details
void MerchantHoard::pickUp(Player &p) {
    p.addAction("Player Character picks up a Merchant Hoard (value " + 
                to_string(constants::MERCHANT_HOARD_VALUE) + ")");
    Gold::pickUp(p);
} // MerchantHoard::pickUp()

// see item.h for details
Item * createItem(const char type) {
    assert (type == 'P' || type == 'G');
    if (type == 'P') {
        // create random potion
        const int random = rand() % POTION_DENOMINATOR;
        switch (random) {
            case (0) : return new RH();
            case (1) : return new BA();
            case (2) : return new BD();
            case (3) : return new PH();
            case (4) : return new WA();     
            default  : return new WD();   
        } // switch
    } else {
        // create random treasure
        int random = rand() % GOLD_DENOMINATOR;
        if (random <= PROB_NORMAL_GOLD) return new NormalHoard();
        random -= PROB_NORMAL_GOLD;
        if (random <= PROB_DRAGON_GOLD) {
            throw Item::DHCreated(*(new DragonHoard));
        } // if
        return new SmallHoard();
    } // else
} // createItem(type)

