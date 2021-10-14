#ifndef __POTION_H__
#define __POTION_H__

#include <iostream>
#include "item.h"

class Potion : public Item {
  protected:
    const std::string effect;

    Potion(std::string effect);

  public:
    /*******************************************************************************
     * pickUp(p) applies effects of this potion to p
     * effects: if potion effects are permanent, applies them directly to p
     *          otherwise adds corresponding potion decorator to p
     * notes: to be overloaded by each potion type
     * time: O(1)
     * *****************************************************************************/
    virtual void pickUp(Player &p) = 0;

    virtual ~Potion();

    // print potion effects
    friend std::ostream& operator<<(std::ostream& out, const Potion& p);
};

// potion types
class RH: public Potion {
  public:
    RH();
    // see Potion::pickUp()
    void pickUp(Player &p);
};

class BA: public Potion {
  public:
    BA();
    // see Potion::pickUp()
    void pickUp(Player &p);
};

class BD: public Potion {
  public:
    BD();
    // see Potion::pickUp()
    void pickUp(Player &p);
};

class PH: public Potion {
  public:
    PH();
    // see Potion::pickUp()
    void pickUp(Player &p);
};

class WA: public Potion {
  public:
    WA();
    // see Potion::pickUp()
    void pickUp(Player &p);
};

class WD: public Potion {
  public:
    WD();
    // see Potion::pickUp()
    void pickUp(Player &p);
};

#endif