//written by vmathu02
//purpose: card declarations for class
#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Card {
public:
    //constructor
    Card();
    //Getters
    string getrow();
    int getPrestige();
    string getGemColor();
    int getPrice(int index);

    //Setters
    void setrow(string row);
    void setprestige(int prestige);
    void setdiscount(string discount);
    void setprice(int arr_price[5]);

private:
    string row;
    int prestige;
    string discount; //gem color
    int price[5];
};

#endif
