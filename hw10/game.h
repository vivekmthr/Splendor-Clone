/*
 * game.h
 * COMP11 Splendor
 *vmathu02, purpose: game declarations
 */

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "termfuncs.h"
#include "Card.h"
#include "Player.h"
#include <vector>

using namespace std;

class Game {
public:
    //constructor
    Game(string filename, string play_nobles);

    //main gameplay
    void playGame();


private:

    //drawing functions
    void drawBoard();

    void drawNoble(int vpos, int hpos, Card *c);

    void drawPlayer(int vpos, int hpos, Player *p);

    void drawDeck(int vpos, int hpos, string type);

    void drawCard(int vpos, int hpos, Card *c);

    void drawGem(int vpos, int hpos, string color);

    void clearBoard();

    //drawing variables
    static const int BOARD_HEIGHT = 41;
    static const int BOARD_WIDTH = 120;
    static const int Card_WIDTH = 13;
    static const int Card_HEIGHT = 8;
    static const int Card_ROWS = 3;
    static const int Card_COLS = 4;
    static const int NUM_NOBLES = 3;
    static const int GEMS = 6;

    //Constructor functions
    void initialize_decks(vector<Card *> *point_to_deck, int num_Cards);

    void populate_vectors
    (vector<Card *> *point_to_deck, int num_Cards, string filename, int skip);

    Card *grid[Card_ROWS][Card_COLS];

    void populate_grid();

    bool play_nobles;

    //constructor vars
    int mines_count = 3;
    int transport_count = 3;
    int vendors_count = 3;
    int gem_bank[6] = {4, 4, 4, 4, 4, 5};
    vector<Card *> mines;
    vector<Card *> vendors;
    vector<Card *> transport;
    vector<Card *> nobles;
    Player player1;
    Player player2;

    //Main game loop functions
    void p2(int num);

    void Return_gems(int num);

    void br(int num);

    void buy(int num);

    void p3(int *num);

    void r(int num);

    string valid_move(int num);

    void mod_deck(int num);

    void update_player(int num, int remove_gems[5], int gold);

    void update_grid(int num, int remove_gems[5], int gold);

    void add_nobles();

    void reserve_error(int num);

    void over_10();

    void get_command();

    void Player_Wins();

    void implement_nobles(int num, int noble_num);

    //main game loop vars
    bool game_over = false;
    bool player1_turn = true;
    int num;
    int total_gems = 0;
    string first = "";
    string second = "";
    string third = "";
    string fourth = "";
    string result;
    int intsecond = 0;
    int intthird = 0;



    // Specific action functions
    string indexToColor(int number);

    int ColorToIndex(string color);

    int get_row();

    void remove_gems(int gems[6]);

    Player *get_player(int num);

    void clean_board();

    void deck_check();

    //display
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    string board_color[BOARD_HEIGHT][BOARD_WIDTH];

    //clearing memory
    void clear_memory();
};

#endif
