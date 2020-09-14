//
// Created by vmathu02
// purpose player definitions
#include "Player.h"
/* Player() //constructor
* Input: Takes in no arguments other than those already predefined in the class.
* Description: Initializes all the parts of the player to 0 or nullptr on setup
* Output: is a constructor, so no return.
*/
Player::Player() {
    prestige = 0;
    for (int i = 0; i <5 ; ++i) {
        discounts[i]= 0;
    }
    for (int i = 0; i <6 ; ++i) {
        gems[i]= 0;
    }
    for (int i = 0; i <3 ; ++i) {
        reserveCards[i] = nullptr;
    }
}
/* Set_prestige
* Input: Takes in an integer of the points to be added to a players prestige
* Description: Adds the number of points to the prestige
* Output: void, simply modifies the value of prestige
*/
void Player::setPrestige(int add_points){
    prestige+= add_points;
}

/* Set_discount
* Input: Takes in an integer of the position of which the discount is to be set
* Description: Adds one to the discount of a player at some position
* Output: void, simply modifies the value of an int at the discount position
*/
void Player::setDiscount(int pos){//white red pink blue green
    discounts[pos] = discounts[pos] +1;
}

/* set_gemCount
* Input: Takes in an array of the gems to be added
* Description: Adds the number of gems specified in the array add_gems to the gem array
* Output: void, simply modifies them gem array
*/
void Player::setGemCount(int add_gems[6]){
    for (int i = 0; i <6 ; ++i) {
        gems[i]= gems[i]+ add_gems[i];
    }
}

/* set_ReserveCards
* Input: takes in a pointer to the card that needs to be reserved
* Description: Adds that card to the reserve cards array if it is not full already
* Output: void, simply modifies the reservecards array
*/
void Player::setReserveCards(Card *reserve_card) {
    for (int i = 0; i <3 ; ++i) {
        if(reserveCards[i] == nullptr){
            reserveCards[i] = reserve_card;
            break;
        }
    }
}

void Player::removeReserve(int index){
    reserveCards[index] = nullptr;
}

/* TotalPrestige
* Input: None
* Description: Returns the total prestige that a player has as an int
* Output: an int containing total prestige
*/
int Player::totalPrestige() {
    return prestige;
}
/* Getdiscount
* Input: an index which specifies the color discount a player has at some index
* Description: Returns the discount at the poistion
* Output: an int containing the total discount
*/
int Player::getDiscount(int index) {
    return discounts[index];
}
/* GetGemCount
* Input: an index which specifies how many gems of some color a player has
* Description: finds the no of gems of some color
* Output: returns an int containing that value
*/
int Player::getGemCount(int index){
    return gems[index];
}

/* GetGemCount
* Input: An index showing which reserve card to return
* Description: Gets the reserved card at some index
* Output: a pointer to the reserved card
*/
Card* Player::getReserve(int index){
    return reserveCards[index];
}
