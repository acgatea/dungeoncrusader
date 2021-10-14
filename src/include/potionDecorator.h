#ifndef __POTION_DECORATOR_H__
#define __POTION_DECORATOR_H__

class PotionDecorator {
  protected:
    // linked list of decorators
    PotionDecorator * decorators;

    // the player's potion multiplier
    const float potionMultiplier;
  public:
    // constructor (shallow copy)
    // time: O(1)
    PotionDecorator(PotionDecorator *decorators, const float potionMultiplier);

    /*******************************************************************************
     * getAtk(atk) updates atk based on decorators
     * notes: atk is an accumulator
     *        method to be overloaded by PotionDecorator types
     * effects: updates atk according to decorators
     * time: O(#decorators)
     * *****************************************************************************/    
    virtual void getAtk(float& atk) const;

    /*******************************************************************************
     * getDef(def) updates def based on decorators
     * notes: def is an accumulator
     *        method to be overloaded by PotionDecorator types
     * effects: updates def according to decorators
     * time: O(#decorators)
     * *****************************************************************************/
    virtual void getDef(float& def) const;

    virtual ~PotionDecorator();
};

// types of PotionDecorator (temporary potions)
class BoostAttack : public PotionDecorator {
  public:
    BoostAttack(PotionDecorator *decorators, const float potionMultiplier);

    // see PotionDecorator::getAtk()
    void getAtk(float& atk) const;
};


class BoostDef : public PotionDecorator {
  public:
    BoostDef(PotionDecorator *decorators, const float potionMultiplier);

    // see PotionDecorator::getDef()
    void getDef(float& def) const;
};

class WoundAttack : public PotionDecorator {
  public:
    WoundAttack(PotionDecorator *decorators, const float potionMultiplier);

    // see PotionDecorator::getAtk()
    void getAtk(float& atk) const;
};

class WoundDef : public PotionDecorator {
  public:
    WoundDef(PotionDecorator *decorators, const float potionMultiplier);

    // see PotionDecorator::getDef()
    void getDef(float& def) const;
};

#endif