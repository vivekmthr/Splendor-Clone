//written by vmathu02,
//purpose:card definitions
#include "Card.h"
/* Card constructor
* Input: Takes in no arguments as already has access to those defined in card.h
* Description: initializes the prestige to 0 and everything in the price array to 0
* and the discount and the row to empty strings
* Output: returns an int containing that value
*/
Card::Card() {
    setprestige(0);
    setdiscount("");
    setrow("");
    for (int i = 0; i <5 ; ++i) {
        price[i]= 0;
    }
}

/* SetRow
* Input: Takes in the string name of the row
* Description: Sets the value of row to the inputted string
* Output: void function as simply modifying the row defined in card.h
*/
void Card::setrow(string row){
this->row = row;
}

/* GetRow
* Input: no input
* Description: gives back the string contained in the row variable
* Output: returns a string containing the value of row
*/
string Card::getrow(){
    return this->row;
}

/* setdiscount
* Input: takes in a string
* Description: sets the discount to the value of the string
* Output: void function, thus no return
*/
void Card::setdiscount(string discount) {
    this->discount = discount;
}

/* getGemColor
* Input: no input
* Description: returns the value stored in the discount variable
* Output: returns the value stored in the discount variable
*/
string Card::getGemColor(){
    return this->discount;
}

/* setprestige
* Input: an int to which the prestige of the card becomes
* Description: sets the prestige to the int value given
* Output: no output as void function
*/
void Card::setprestige(int prestige){
    this->prestige = prestige;
}

/* getPrestige
* Input: no input
* Description: returns the value of prestige stored in the prestige variable
* Output: returns an integer containing the prestige value
*/
int Card::getPrestige(){
    return this->prestige;
}

/* setprice
* Input: takes in an int array of prices to set the prices too
* Description: sets every value in the price array to the one given as an argument
* Output: no output as modifying array
*/
void Card::setprice(int arr_price[5]) {
    for (int i = 0; i <5 ; ++i) {
        price[i]= arr_price[i];
    }
}

/* getprice
* Input: Takes in the number to return the price for
* Description: returns the price at the index inputted
* Output: returns an int containing the price at that index
*/
int Card::getPrice(int number){
    if(number == 0){
        return price[0];
    }
    if(number == 1){
        return price[1];
    }
    if(number == 2){
        return price[2];
    }
    if(number== 3){
        return price[3];
    }
    if(number == 4){
        return price[4];
    }
    return 0;
}
