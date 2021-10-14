/*******************************************************************************
 * potionDecorator.cc
 * 
 * Module implementing different types of potion decorators.
 * *****************************************************************************/

#include "potionDecorator.h"
#include "constants.h"

// constructors
PotionDecorator::PotionDecorator(PotionDecorator *decorators, const float potionMultiplier): 
    decorators(decorators), potionMultiplier(potionMultiplier) {}
BoostAttack::BoostAttack(PotionDecorator *decorators, const float potionMultiplier): 
    PotionDecorator(decorators, potionMultiplier) {}
BoostDef::BoostDef(PotionDecorator *decorators, const float potionMultiplier): 
    PotionDecorator(decorators, potionMultiplier) {}
WoundAttack::WoundAttack(PotionDecorator *decorators, const float potionMultiplier): 
    PotionDecorator(decorators, potionMultiplier) {}
WoundDef::WoundDef(PotionDecorator *decorators, const float potionMultiplier): 
    PotionDecorator(decorators, potionMultiplier) {}

// destructor
PotionDecorator::~PotionDecorator(){
    delete decorators;
} // ~PotionDecorator()

// see potionDecorator.h
void PotionDecorator::getAtk(float& atk) const {
    if (decorators) decorators->getAtk(atk);
} // PotionDecorator::getAtk()

// see potionDecorator.h
void PotionDecorator::getDef(float& def) const {
    if (decorators) decorators->getDef(def);
} // PotionDecorator::getDef()

// see potionDecorator.h
void BoostAttack::getAtk(float &atk) const{
    atk += constants::BA_ATK_VALUE * potionMultiplier;
    if (decorators) decorators->getAtk(atk);
} // BoostAttack::getAtk()

// see potionDecorator.h
void BoostDef::getDef(float &def) const{
    def += constants::BD_DEF_VALUE * potionMultiplier;
    if (decorators) decorators->getDef(def);
} //  BoostDef::getDef()

// see potionDecorator.h
void WoundAttack::getAtk(float &atk) const{
    atk += constants::WA_ATK_VALUE * potionMultiplier;
    if (decorators) decorators->getAtk(atk);
} // WoundAttack::getAtk()

// see potionDecorator.h
void WoundDef::getDef(float &def) const{
    def += constants::WD_DEF_VALUE * potionMultiplier;
    if (decorators) decorators->getDef(def);
} // WoundDef::getDef()
