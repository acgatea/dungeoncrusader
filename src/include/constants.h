
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <vector>
#include <string>
#include <map>

namespace constants {
    // default configuration file
    const std::string EMPTY_CONFIG = ".\\src\\include\\emptyconfig.txt";

    /************************ CONFIGURATION CONSTANTS *******************************/

    const int NUM_ROWS = 25, NUM_COLS = 79;
    const int NUM_CHAMBERS = 5;
    const int NUM_FLOORS = 5;

    // chamber 0
    const int lRow0 = 3, rRow0 = 6, lCol0 = 3, rCol0 = 28;
    // chamber 1
    const int lRow1_0 = 3, rRow1_0 = 4, lCol1_0 = 39, rCol1_0 = 61;
    const int lRow1_1 = 5, rRow1_1 = 5, lCol1_1 = 39, rCol1_1 = 69;
    const int lRow1_2 = 6, rRow1_2 = 6, lCol1_2 = 39, rCol1_2 = 72;
    const int lRow1_3 = 7, rRow1_3 = 12, lCol1_3 = 61, rCol1_3 = 75;
    // chamber 2
    const int lRow2_0 = 16, rRow2_0 = 18, lCol2_0 = 65, rCol2_0 = 75;
    const int lRow2_1 = 19, rRow2_1 = 21, lCol2_1 = 37, rCol2_1 = 75;
    // chamber 3
    const int lRow3 = 15, rRow3 = 21, lCol3 = 4, rCol3 = 24;
    // chamber 4
    const int lRow4 = 10, rRow4 = 12, lCol4 = 38, rCol4 = 49;

    // maximum number of things allowed per chamber
    const int MAX_CHAMBER_LOAD = 30;

    /*******************************************************************************
     * findChamberNum(row, col) determines what chamber Cell (row, col) is in
     * notes: if not a floor tile, returns -1
     * time: O(1)
     * *****************************************************************************/
    int findChamberNum(const int row, const int col);

    /*************************** DIRECTION CONSTANTS *******************************/

    const int NUM_DIRECTIONS = 8;
    
    // available directions
    const std::vector<std::string> DIRECTIONS =
        {"no", "nw", "ne", "so", "sw", "se", "ea", "we"};

    // available directions mapped to corresponding (vShift, hShift)
    // the 1 block radius neighbors of a cell will be stored in this order
    const std::map<const std::string, std::pair<const int, const int> > DIRECTION_TO_OFFSET = {
        {"no", std::make_pair(-1,0)},
        {"nw", std::make_pair(-1,-1)},
        {"ne", std::make_pair(-1,1)},
        {"so", std::make_pair(1,0)},
        {"sw", std::make_pair(1,-1)},
        {"se", std::make_pair(1,1)},
        {"ea", std::make_pair(0,1)},
        {"we", std::make_pair(0,-1)}
    };

    /*******************************************************************************
     * getFullDirection(dir) translates short-hand directions to their full name
     * required: dir is one of no, so, sw, se, nw, ne, ea, we
     * *****************************************************************************/
    std::string getFullDirection (const std::string dir);
    
    /*******************************************************************************
     * computeDamage(atk, def) computes the damage from an 
     * attack by character with atk on character with def
     * *****************************************************************************/
    int computeDamage(const int atk, const int def);

    /*************************** ENEMY CONSTANTS *******************************/

    const int NUM_ENEMIES_PER_FLOOR = 20;
    
    // spawn probabilities
    const int ENEMY_DENOMINATOR = 18;
    const int PROB_HUMAN = 4;
    const int PROB_DWARF = 3;
    const int PROB_HALFLING = 5;
    const int PROB_ELF = 2;
    const int PROB_ORC = 2;
    const int PROB_MERCHANT = 2;

    // race-specific attributes
    const int HUMAN_HP = 140;
    const int HUMAN_ATK = 20;
    const int HUMAN_DEF = 20;
    const int HUMAN_GOLD_PILES = 2;
    const std::string HUMAN_ABILITY = "drops " + std::to_string(HUMAN_GOLD_PILES) 
                                      + " normal piles of gold";

    const int DWARF_HP = 100;
    const int DWARF_ATK = 20;
    const int DWARF_DEF = 30;
    const int DWARF_VAMPIRE_HP_LOSS = -5;
    const std::string DWARF_ABILITY = "Vampires are allergic to dwarves and lose" 
                                      + std::to_string(-DWARF_VAMPIRE_HP_LOSS) 
                                      + " HP rather than gain";

    const int ELF_HP = 140;
    const int ELF_ATK = 30;
    const int ELF_DEF = 10;
    const int ELF_NUM_ATTACKS = 2;
    const std::string ELF_ABILITY = "gets " + std::to_string(ELF_NUM_ATTACKS)  
                                    + " attacks against every race except Drow";

    const int ORC_HP = 180;
    const int ORC_ATK = 30;
    const int ORC_DEF = 25;
    const float ORC_ON_GOBLIN_DAMAGE_MULTIPLIER = 1.5;
    const std::string ORC_ABILITY = "does 50% more damage to goblins";

    const int MERCH_HP = 30;
    const int MERCH_ATK = 70;
    const int MERCH_DEF = 5;
    const std::string MERCH_ABILITY = "trades potions";
    const int MERCH_MAX_NUM_POTIONS = 6;
    const int MERCH_MAX_FEE = 3;
    const int MERCH_MAX_STEAL = 2;
    const int MERCH_STEAL_PROB_DENOM = 2;
    const int MERCH_PROB_REFUSE = 5;

    const int DRAGON_HP = 150;
    const int DRAGON_ATK = 20;
    const int DRAGON_DEF = 20;
    const std::string DRAGON_ABILITY = "always guards a treasure hoard";

    const int HALFLING_HP = 100;
    const int HALFLING_ATK = 15;
    const int HALFLING_DEF = 20;
    const int HALFLING_MISS_DENOM = 2;
    const std::string HALFLING_ABILITY = "has a 50% to beguile the player character and cause them to miss";

    /***************************** PC CONSTANTS ***********************************/

    // race-specific attributes
    const int SHADE_HP = 125;
    const int SHADE_ATK = 25;
    const int SHADE_DEF = 25;
    const float SHADE_SCORE_MULTIPLIER = 1.5;
    const std::string SHADE_ABILITY = "final score magnified by 1.5";

    const int DROW_HP = 150;
    const int DROW_ATK = 25;
    const int DROW_DEF = 15;
    const float DROW_POTION_MULTIPLIER = 1.5;
    const std::string DROW_ABILITY = "all potions have their effect magnified by 1.5";

    const int VAMP_HP = 50;
    const int VAMP_ATK = 25;
    const int VAMP_DEF = 25;
    const int VAMP_HP_GAIN = 5;
    const int VAMP_HP_LOSS_DWARF = -5;
    const std::string VAMP_ABILITY = "gains " + std::to_string(VAMP_HP_GAIN) 
                                     + " HP every successful attack and has no maximum HP";
    const int TROLL_HP = 120;
    const int TROLL_ATK = 25;
    const int TROLL_DEF = 15;
    const int TROLL_HP_REJUVENATION = 5;
    const std::string TROLL_ABILITY = "regains " + std::to_string(TROLL_HP_REJUVENATION) 
                                      + " HP every turn (HP capped at " 
                                      + std::to_string(TROLL_HP) + ")";
    const int GOBLIN_HP = 110;
    const int GOBLIN_ATK = 15;
    const int GOBLIN_DEF = 20; 
    const int GOBLIN_GOLD_STEAL = 5; 
    const std::string GOBLIN_ABILITY = "steals " + std::to_string(GOBLIN_GOLD_STEAL) 
                                       + " gold from every slain enemy";

    // dodge probability denominator for enemy attacks
    const int PC_DODGE_DENOM = 2;

    /******************************** ITEM CONSTANTS *******************************/

    // gold
    const int GOLD_DENOMINATOR = 8;
    const int PROB_NORMAL_GOLD = 5;
    const int PROB_DRAGON_GOLD = 1;
    const int PROB_SMALL_GOLD = 2;
    const int NUM_GOLD_PER_FLOOR = 10;
    const int DROP_GOLD_PROB_DENOM = 2;

    const int SMALL_HOARD_VALUE = 1;
    const int NORMAL_HOARD_VALUE = 2;
    const int MERCHANT_HOARD_VALUE = 4;
    const int DRAGON_HOARD_VALUE = 6;

    // potions
    const int POTION_DENOMINATOR = 6;
    const int NUM_POTION_PER_FLOOR = 10;
    const int RH_HP_VALUE = 10;
    const int PH_HP_VALUE = -10;
    const int BA_ATK_VALUE = 5;
    const int BD_DEF_VALUE = 5;
    const int WA_ATK_VALUE = -5;
    const int WD_DEF_VALUE = -5;

} // namespace constants

#endif