/*******************************************************************************
 * potion.cc
 * 
 * Module implementing potion types.
* *****************************************************************************/

#include "potion.h"
#include "player.h"
#include "constants.h"

using namespace std;
using namespace constants;

// constructors for potion types
Potion::Potion(string effect) : Item('P'), effect(effect) {}
RH::RH(): Potion ("restore up to " + to_string(RH_HP_VALUE) + " HP") {}
BA::BA(): Potion ("increase ATK by " + to_string(BA_ATK_VALUE)) {}
BD::BD(): Potion ("increase Def by " + to_string(BD_DEF_VALUE)) {}
PH::PH(): Potion ("lose up to " + to_string(-PH_HP_VALUE) + " HP") {}
WA::WA(): Potion ("decrease Atk by " + to_string(-WA_ATK_VALUE)) {}
WD::WD(): Potion ("decrease Def by " + to_string(-WD_DEF_VALUE)) {}

// destructor
Potion::~Potion() {}

// see potion.h for details
void RH::pickUp(Player &p) {
    const int hpChange = p.potionMultiplier() * RH_HP_VALUE;
    p.addAction("Player Character picks up a Potion that Restores up to " + 
                to_string(hpChange) + " HP");
    p.modifyHP(hpChange);
} // RH::pickup()

// see potion.h for details
void PH::pickUp(Player &p) {
    const int hpChange = p.potionMultiplier() * PH_HP_VALUE;
    p.addAction("Player Character picks up a Potion that Decreases HP by " + 
                to_string(-hpChange));
    p.modifyHP(hpChange);
} // PH::pickUp()

// see potion.h for details
void BA::pickUp(Player &p) {
    const int atkChange = p.potionMultiplier() * BA_ATK_VALUE;
    p.addAction("Player Character picks up a Potion that temporarily boosts Attack by " + 
                to_string(atkChange));
    p.attachDecorator(*(new BoostAttack(p.getDecorators(),p.potionMultiplier())));
} // BA::pickUp

// see potion.h for details
void BD::pickUp(Player &p) {
    const int defChange = p.potionMultiplier() * BD_DEF_VALUE;
    p.addAction("Player Character picks up a Potion that temporarily boosts Defense by " + 
                to_string(defChange));
    p.attachDecorator(*(new BoostDef(p.getDecorators(),p.potionMultiplier())));
} // BD::pickUp()

// see potion.h for details
void WA::pickUp(Player &p) {
    const int atkChange = p.potionMultiplier() * WA_ATK_VALUE;
    p.addAction("Player Character picks up a Potion that temporarily decreases Attack by (at most) " + 
                to_string(-atkChange));
    p.attachDecorator(*(new WoundAttack(p.getDecorators(),p.potionMultiplier())));
} // WA::pickUp()

// see potion.h for details
void WD::pickUp(Player &p) {
    const int defChange = p.potionMultiplier() * WD_DEF_VALUE;
    p.addAction("Player Character picks up a Potion that temporarily decreases Defense by (at most) " + 
                to_string(-defChange));
    p.attachDecorator(*(new WoundDef(p.getDecorators(),p.potionMultiplier())));
} // WD::pickUp()

// see potion.h for details
ostream& operator<<(ostream& out, const Potion& p) {
    out << p.effect << endl;
    return out;
} // operator<<