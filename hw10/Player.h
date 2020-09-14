//
// Player.h
//vmathu02 purpose: player declarations
//

#ifndef HW10_PLAYER_H
#define HW10_PLAYER_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "Card.h"
#include <vector>

class Player {
public:
    //constructor
    Player();
    //getters
    int totalPrestige();
    int getDiscount(int index);
    int getGemCount(int index);
    Card* getReserve(int index);
    //setters
    void setDiscount(int pos); //white red pink blue green
    void setGemCount(int add_gems[6]);
    void setReserveCards(Card* reserve_card);
    void setPrestige( int add_points);
    void removeReserve(int index);

    Card* reserveCards[3];

private:


    //variables which are contained in each player
    int prestige;
    int discounts[5]; //array containing discounts
    int gems[6]; //array containing gem_counts
    //Card* reserveCards[3]; //an array of card* to the reserved cards
};


#endif //HW10_PLAYER_H
