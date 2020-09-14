/*
 * game.cpp
 * COMP11 Splendor
 *written by vmathu02
 */

#include "game.h"
#include "Card.h"

/* Game
* Input: string filename, and a bool indication of whether to play with nobles
* Description: creates the decks of card, decides whether to play with nobles
and initilizes the grid
* Output: No output as void function.
*/
Game::Game(string filename, string play_nobles) {
    //include nobles or not
    if (play_nobles == "true") {
        this->play_nobles = true;
    } else {
        this->play_nobles = false;
    }
    //initialize the board
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
    }
    //creates empty vectors of the required size for all four decks of cards
    initialize_decks(&mines, 40);
    initialize_decks(&transport, 30);
    initialize_decks(&vendors, 20);
    initialize_decks(&nobles, 10);
    //populates the vectors with the necessary cards from the data file
    populate_vectors(&mines, 40, filename, 1);
    populate_vectors(&transport, 30, filename, 41);
    populate_vectors(&vendors, 20, filename, 71);
    populate_vectors(&nobles, 10, filename, 91);
    populate_grid();
}


/* playGame
* Input: As the function is part of the game class it has access to all
variables and functions defined in game.
* Description: plays the main game loop, prompts the user to play actions,
calls the valid move function and ends the game when a player achieves 15 points.
* Output: No output as void function.
*/
void Game::playGame() {
    //resets screen and plays game until the game is over
    while (!game_over) {
        clearBoard();
        screen_clear();
        screen_home();
        drawBoard();
        if (player1_turn == true) {
            cout << "PLAYER 1, enter your next move: ";
            num = 1;
        } else {
            cout << "PLAYER 2, enter your next move: ";
            num = 2;
        }
        //prompts a move
        result = valid_move(num);
        //returns gems if gem count is over 10
        over_10();
        //adds the nobles functionality
        add_nobles();
        //implements the player wins functionality
        Player_Wins();

        if (result == "quit" ) {
            clear_memory();
            return;
        }
        //change turns
        player1_turn = !player1_turn;
    }
}



/* Player_wins
* Input: As the function is part of the game class,
it has access to all variables and functions defined in game.
* Description: when a player has 15 prestige points, if it's p1's turn,
p2 gets one more turn, and then declares a winner.
* Output: No output as void function.
*/
void Game::Player_Wins(){
    //if player has more than 15 prestige and it is p1 gives p2 one more turn
    if (get_player(num)->totalPrestige() == 15) {
        if (num == 1) {
            num = 2;
            clean_board();
            cout << "PLAYER 2, enter your next move: ";
            valid_move(num);
            clean_board();
            if(player2.totalPrestige() != 15){
                cout << "Congratulations Player 1: YOU WIN!!!!"<< endl;
                result = "quit";
                return;
            }
            else{
                cout << "You losers tied - BORING!!!!" << endl;
                result = "quit";
                return;
            }
        }
        else{
            clean_board();
            cout << "Congratulations Player 2: YOU WIN!!!!" << endl;
            result = "quit";
            return;
        }
    }
}

void Game::clean_board(){
    clearBoard();
    screen_clear();
    screen_home();
    drawBoard();
}

/* add_nobles
* Input: As the function is part of the game class it
has access to all variables and functions defined in game.
* Description: adds the nobles functionality to the game
* Output: No output as void function.
*/
void Game::add_nobles(){
    implement_nobles(num, 0);
    implement_nobles(num, 1);
    implement_nobles(num, 2);
}

/* over_10
* Input: As the function is part of the game class
it has access to all variables and functions defined in game.
* Description: if a player has over 10 gems prompts them to return down to 10
* Output: No output as void function.
*/
void Game::over_10() {
    //updates the number of total gems
    for (int i = 0; i < 6; ++i) {
        total_gems += get_player(num)->getGemCount(i);
    }
    //clears the board so you can see it everytime
    if (total_gems > 10) {
        clearBoard();
        screen_clear();
        screen_home();
        drawBoard();
        //calls the return gems function
        Return_gems(num);
        if (total_gems > 10) {
            clearBoard();
            screen_clear();
            screen_home();
            drawBoard();
            //calls the return gems function until total_gems less than 10
            Return_gems(num);
        }
    }
    //resets it to 0
    total_gems = 0;
}

/* valid_move
* Input: All funcs and vars in game, and an int specifying players turn
* Description: prompts the user for a valid move and calls the needed function
* Output: returns a string, so that if q is called, the program can quit
*/
string Game::valid_move(int num) {
   //get a command from the player and call the appropriate function
    get_command();
    if (first == "p3" || first == "p2" || first == "r"
    || first == "b" || first == "br" || first == "q") {
        if (first == "p3") {
            p3(&num);
        }
        if (first == "p2") {
            p2(num);
        }
        if (first == "r") {
            r(num);
        }
        if (first == "b") {
            buy(num);
        }
        if (first == "br") {
            br(num);
        }
        if (first == "q") {
            return "quit";
        }
    } else { //error checking
        cout << "Invalid move - enter a valid move: ";
        valid_move(num); //prompt the user for another valid move
        return "";
    }
    return "";
}

/* Return_gems
* Input: All funcs and vars in game,an int specifying which players turn it is
* Description: makes the user return gems until down to 10
* Output: No output as void function.
*/
void Game::Return_gems(int num) {
    string color;
    cout << "PLAYER " << num <<
    ", you have more than 10 gems!Name one gem you would like to return: ";
    getline(cin, color); //prompt the user for a color to return
    int remove_gems[6] = {0}; //
    //remove gems of teh specified color from the player
    remove_gems[ColorToIndex(color)] -= 1;
    get_player(num)->setGemCount(remove_gems);
    //add gems to the gem bank
    gem_bank[ColorToIndex(color)] += 1;
}

/* br
* Input: All funcs and vars in game, an int specifying which players turn it is
* Description: Allows the player to buy one of the cards in their reserve
* Output: No output as void function.
*/
void Game::br(int num) {
    if (get_player(num)->getReserve(intsecond - 1) == nullptr) {//error checking
        cout << "Invalid move (no card at that index) - enter a valid move: ";
        valid_move(num);
        return;
    }
    //define variables necessary
    int gold = 0;
    int remove_gems[6] = {0};
    int lacking[6] = {0};
    //calculates the price neccessary for purchase or reserve card
    for (int i = 0; i < 5; ++i) {
        remove_gems[i] = (get_player(num)->getReserve(intsecond - 1)->getPrice(i));
        lacking[i] = (remove_gems[i] - (get_player(num)->getGemCount(i)
        + get_player(num)->getDiscount(i)));
        remove_gems[i] = -(remove_gems[i]);
        if (-remove_gems[i] >= get_player(num)->getDiscount(i)) {
            remove_gems[i] += get_player(num)->getDiscount(i);
        } else {
            remove_gems[i] = 0;
        }
        if (lacking[i] > 0) {
            gold += lacking[i];
            remove_gems[i] = remove_gems[i] + lacking[i];
        }
        if (get_player(num)->getGemCount(5) < gold) { //error checking
            cout << "Invalid move (cannot afford card) - enter a valid move: ";
            valid_move(num);
            return;
        }
    }
    update_player(num, remove_gems, gold);//adds card attributes to  player
}

/* update_player
* Input: All funcs and vars in game, and an int specifying player turn
and int array containing the price of the card, and an int containing amount of
gold needed to purchase it.
* Description: Called in br, and updates the players discounts,
price and prestige and removes gems from them.
* Output: No output as void function.
*/
void Game::update_player(int num, int remove_gems[5], int gold){
    //adds gems to the gem bank
    remove_gems[5] = -gold;
    for (int j = 0; j < 6; ++j) {
        gem_bank[j] += (-remove_gems[j]);
    }
    //updates the players dicount, prestige, gemcounts and reserve
    get_player(num)->setDiscount
    (ColorToIndex(get_player(num)->getReserve(intsecond - 1)->getGemColor()));
    get_player(num)->setPrestige
    (get_player(num)->getReserve(intsecond - 1)->getPrestige());
    get_player(num)->setGemCount(remove_gems);
    get_player(num)->removeReserve(intsecond - 1);
}


void Game::implement_nobles(int num, int noble_num) {
    vector<bool> checks;
    bool implement = true;
    //makes sure the program doesn't seg fault if noble has already been taken
    if(nobles[noble_num] == nullptr){
        return;
    }
    for (int i = 0; i < 5; ++i) {
     //checks if the player can afford the noble
        if (get_player(num)->getDiscount(i) >= nobles[noble_num]->getPrice(i)) {
            checks.push_back(true);
        } else {
            checks.push_back(false);
        }
        if (checks[i] != true) {
            implement = false;
            return;
        }
    }
    //if can afford, adds 3 to prestige/deletes noble
    if(implement == true){
        get_player(num)->setPrestige(3);
        delete nobles.at(noble_num);
        nobles.at(noble_num) = nullptr;
    }
}

/* buy
* Input: All funcs and vars in game, an int specifying which players turn it is
* Description: Allows the player to buy a card from the grid
* Output: No output as void function.
*/
void Game::buy(int num) {
    //defines varibles and makes sure right card is drawn from grid
    mod_deck(num);
    int gold = 0;
    int remove_gems[6] = {0};
    int lacking[6] = {0};
    //finds the price of the card taking into account gold and discounts
    for (int i = 0; i < 5; ++i) {
        remove_gems[i] = (grid[get_row()][intthird - 1]->getPrice(i));
        lacking[i] = (remove_gems[i]
        - (get_player(num)->getGemCount(i) + get_player(num)->getDiscount(i)));
        remove_gems[i] = -(remove_gems[i]);
        if (-remove_gems[i] >= get_player(num)->getDiscount(i)) {//add discount
            remove_gems[i] += get_player(num)->getDiscount(i);
        } else {
            remove_gems[i] = 0;
        }
        if (lacking[i] > 0) {
            gold += lacking[i]; //find the amount of gold necessary to buy
            remove_gems[i] = remove_gems[i] + lacking[i];
        }
        if (get_player(num)->getGemCount(5) < gold) { //error checking
            cout << "Invalid move (cannot afford card) - enter a valid move: ";
            valid_move(num);
            return;
        }
    }
    //updates the grid of the player
    update_grid(num, remove_gems, gold);
}

/* update_grid
* Input: All funcs and vars in game,  an int specifying which players turn it is
and int array containing the price of the card, and an int containing amount of
gold needed to purchase it.
* Description: Called in br, and updates the players discounts,
price and prestige and removes gems from them. More importantly
updates the grid so the next card is drawn.
* Output: No output as void function.
*/
void Game::update_grid(int num, int remove_gems[5], int gold){
    remove_gems[5] = -gold;
    for (int j = 0; j < 6; ++j) { //remove gems from gem bank
        gem_bank[j] += (-remove_gems[j]);
    }
    //update players discounts, prestige and gem_counts
    get_player(num)->setDiscount
    (ColorToIndex(grid[get_row()][intthird - 1]->getGemColor()));
    get_player(num)->setPrestige(grid[get_row()][intthird - 1]->getPrestige());
    get_player(num)->setGemCount(remove_gems);
    //make sure the right card is drawn to the grid after card is bought
    deck_check();
    if (second == "m") {
        grid[get_row()][intthird - 1] = mines[39 - mines_count];
    }

    if (second == "t") {
        grid[get_row()][intthird - 1] = transport[29 - transport_count];
    }

    if (second == "v") {
        grid[get_row()][intthird - 1] = vendors[19 - vendors_count];
    }
}

void Game::deck_check(){
    if(mines_count >= 39 || transport_count >= 29 || vendors_count >= 19){
        if (second == "m") {
            delete grid[get_row()][intthird - 1];
            grid[get_row()][intthird - 1] = nullptr;
        }

        if (second == "t") {
            delete grid[get_row()][intthird - 1];
            grid[get_row()][intthird - 1] = nullptr;
        }

        if (second == "v") {
            delete grid[get_row()][intthird - 1];
            grid[get_row()][intthird - 1] = nullptr;
        }
    }
}

/* update_grid
* Input: All funcs and vars in game, an int specifying which players turn it is
* Description: Modifies the grid so the next card is drawn
* Output: No output as void function.
*/
void Game::mod_deck(int num){
    if (second != "m" && second != "t" && second != "v") {
        cout << "Invalid move (invalid card type) - enter a valid move: ";
        valid_move(num);
        return;
    }
    deck_check();
    //makes sure that the next card is drawn from mines, transport and vendors
    if (second == "m") {
        mines_count++;
    }
    if (second == "t") {
        transport_count++;
    }
    if (second == "v") {
        vendors_count++;
    }
}

/* r
* Input: All funcs and vars in game,an int specifying which players turn it is
* Description: allows the player to reserve a card from the grid
* Output: No output as void function.
*/
void Game::r(int num) {
    reserve_error(num); //error checking
    //add the card to the reserve
    get_player(num)->setReserveCards(grid[get_row()][intthird - 1]);
    //modify grid so correct next card is drawn
    if (second == "m") {
        mines_count++;
        grid[get_row()][intthird - 1] = mines[39 - mines_count];
    }
    //changes the count variables so the correct card is drawn
    if (second == "t") {
        transport_count++;
        grid[get_row()][intthird - 1] = transport[29 - transport_count];
    }

    if (second == "v") {
        vendors_count++;
        grid[get_row()][intthird - 1] = vendors[19 - vendors_count];
    }
    if (gem_bank[5] != 0) {
        int add_gems[6] = {0, 0, 0, 0, 0, 1};
        get_player(num)->setGemCount(add_gems);
        remove_gems(add_gems);
    }
}

/* reserve_error
* Input: All funcs and vars in game, an int specifying which players turn it is
* Description: makes sure a valid move is entered for the reserve function
(Error checking)
* Output: No output as void function.
*/
void Game::reserve_error(int num){
    if (intthird > 4 || intthird < 1) {
        cout << "Invalid move (invalid card column) - enter a valid move: ";
        valid_move(num);
        return;
    }

    if (get_player(num)->getReserve(0) != nullptr
        && get_player(num)->getReserve(1) != nullptr
        && get_player(num)->getReserve(2) != nullptr) {
        cout << "Invalid move (reserve is already full) - enter a valid move: ";
        valid_move(num);
        return;
    }
}


/* p3
* Input: All funcs and vars in game, and an int specifying which players turn it is
* Description: allows the player to pick 3 gems of different colors
* Output: No output as void function.
*/
void Game::p3(int *num) {
    if (second != "" && third != "" && fourth != "") {
        if (second != "yellow" && third != "yellow" && fourth != "yellow") {
            if (second != third && second != fourth
                && third != fourth) {
                if (gem_bank[ColorToIndex(second)] != 0 &&
                    gem_bank[ColorToIndex(third)] != 0 &&
                    gem_bank[ColorToIndex(fourth)] != 0) {
                    //move players from gembank into players hand
                    int add_gems[6] = {0};
                    add_gems[ColorToIndex(second)] = 1;
                    add_gems[ColorToIndex(third)] = 1;
                    add_gems[ColorToIndex(fourth)] = 1;
                    get_player(*num)->setGemCount(add_gems);
                    remove_gems(add_gems);
                } else {//error checking
                    cout
                    << "Invalid move (unavailable gems) - enter a valid move: ";
                    valid_move(*num);
                }
            } else {//error checking
                cout << "Invalid move (duplicate color) - enter a valid move: ";
                valid_move(*num);
            }
        } else {//error checking
            cout << "Invalid move (invalid color) - enter a valid move: ";
            valid_move(*num);
        }
    } else {//error checking
        cout << "Invalid move (too few colors) - enter a valid move: ";
        valid_move(*num);
    }
}

/* p2
* Input: All funcs and vars in game, and an int specifying which players turn it is
* Description: allows the player to pick 2 gems of one color if 4
gems are on the board
* Output: No output as void function.
*/
void Game::p2(int num) {
    if (second != "gold") {
        for (int i = 0; i < 6; ++i) {
            if (gem_bank[ColorToIndex(second)] < 4) {//error checking
                cout << "Invalid move (unavailable gems) - enter a valid move: ";
                valid_move(num);
                return;
            }
        }
        //move players from gem_bank into players hand
        int add_gems[6] = {0};
        add_gems[ColorToIndex(second)] = 2;
        get_player(num)->setGemCount(add_gems);
        remove_gems(add_gems);
    } else { //error checking
        cout << "Invalid move (invalid color) - enter a valid move: ";
        valid_move(num);
    }
}

/* get_row
* Input: All funcs and vars in game
* Description: converts the string row into an int of 0,1,2
* output: returns this int
*/
int Game::get_row() {
    //returns the corresponding int
    if (second == "m") {
        return 2;
    }
    if (second == "t") {
        return 1;

    }
    if (second == "v") {
        return 0;
    }
    return 0;
}


/* get_command
* Input: All funcs and vars in game
* Description: reads in the command from a player into variables
these are used to call the appropriate functions
* output: void function
*/
void Game::get_command() {
    //define string variables
    first = "";
    second = "";
    third = "";
    fourth = "";
    intsecond = 0;
    intthird = 0;
    string user_string;
    //gets the line from the user
    getline(cin, user_string);
    //set up a linestream
    istringstream line_stream;
    line_stream.str(user_string);
    //reads from linestream to get a command
    line_stream >> first;
    if (first == "p3") {
        line_stream >> second >> third >> fourth;
    }
    if (first == "r" || first == "b") {
        line_stream >> second >> intthird;
    }
    if (first == "br") {
        line_stream >> intsecond;
    }
    if (first == "p2") {
        line_stream >> second;
    }
}

/* get_command
* Input: All funcs and vars in game, and an int array containing gems to be removed
* Description: removes gems from gembank
* output: void function
*/
void Game::remove_gems(int gems[6]) {
    for (int i = 0; i < 6; ++i) {
        gem_bank[i] -= gems[i];
    }
}

/* get_player
* Input: All funcs and vars in game, and an int num which specifies curr player
* Description: takes in a number and returns a pointer to a player with that num
* output: returns a pointer to the current player
*/
Player *Game::get_player(int num) {
    if (num == 1) {
        return &player1;
    }
    if (num == 2) {
        return &player2;
    }
    return nullptr;
}

/* ColorToIndex
* Input: All funcs and vars in game, and an a string color
* Description: takes in a string color and returns the index which it specifies
* output: returns an int containing the index
*/
int Game::ColorToIndex(string color) {
    //returns pertinent color
    if (color == "white") {
        return 0;
    }
    if (color == "red") {
        return 1;
    }
    if (color == "pink") {
        return 2;
    }
    if (color == "blue") {
        return 3;
    }
    if (color == "green") {
        return 4;
    }
    if (color == "yellow") {
        return 5;
    }
    return 0;
};

/* clear_memoryy
* Input: All funcs and vars in game
* Description: deletes all the active heap memory:
by deleting all the cards in the decks when q is called
* output: void function
*/
void Game::clear_memory() {

    for (int i = 0; i < 40; i++) {
        if(mines[i] == nullptr){
            cout << "this is nullptr" << endl;
        }
        delete mines[i];
    }
    for (int i = 0; i < 30; i++) {
        if(transport[i] == nullptr){
            cout << " transport this is nullptr" << endl;
        }
        delete transport[i];
    }
    for (int i = 0; i < 20; i++) {
        if(vendors[i] == nullptr){
            cout << " vendors this is nullptr" << endl;
        }
        delete vendors[i];
    }
    for (int i = 0; i < 10; i++) {
        if (nobles[i] != nullptr){
            delete nobles[i];
        }
    }
}


/* Populate_grid
* Input: all vars and funcs defined in game
* Description: Populates the 2d array of card pointers(grid)
with card pointers pulled from the vectors (decks)
* Output: No output as void function,
however, it modifies the grid array defined in game.h.
*/
void Game::populate_grid() {
    //populates the first row of the grid with cards from the vendors deck
    for (int i = 0; i < Card_ROWS - Card_ROWS + 1; ++i) {
        for (int j = 0; j < Card_COLS; ++j) {
            grid[i][j] = vendors[19 - j];
        }
    }
    for (int i = 1; i < Card_ROWS - Card_ROWS + 2; ++i) {
        for (int j = 0; j < Card_COLS; ++j) {
            grid[i][j] = transport[29 - j];
        }
    }
    for (int i = 2; i < Card_ROWS - Card_ROWS + 3; ++i) {
        for (int j = 0; j < Card_COLS; ++j) {
            grid[i][j] = mines[39 - j];
        }
    }
}



/* Initialize decks
* Input: all funcs and vars in game, vectors of card pointers (decks),
num of cards
* Description: creates new objects and pushes pointers to them into vectors
* Output: No output as void function
*/
void Game::initialize_decks(vector<Card *> *point_to_deck, int num_Cards) {
    //create new card and push a pointer to it into the deck
    for (int i = 0; i < num_Cards; ++i) {
        Card *Card1 = new Card;
        point_to_deck->push_back(Card1);
    }
}

/* populate_vectors
* Input: all funcs and vars in game, vectors of card pointers (decks),
num of cards,an int containing how the line from which to start reading the file
* Description: reads in the data from the file into the card objects
* Output: No output as void function
*/
void Game::populate_vectors
(vector<Card *> *point_to_deck, int num_Cards, string filename, int skip) {
    //define necessary strings
    string row, discount, line_info;
    int prestige, white, red, pink, blue, green;
    //Open the file for the first pass and skip over the first line
    ifstream infile(filename);
    for (int j = 0; j < skip; ++j) {
        getline(infile, line_info);
    }
    //set up a stringstream
    istringstream line_stream;

    //read the info into the card objects
    for (int i = 0; i < num_Cards; i++) {
        getline(infile, line_info); //Store a line from the file in string line_info
        line_stream.str(line_info); //Set up a stream to read from string line_info
        line_stream >> row >> prestige >> discount >> white >> red >> pink >> blue
        >> green;//Read from the string stream
        //set all the card info;
        point_to_deck->at(i)->setrow(row);
        point_to_deck->at(i)->setprestige(prestige);
        point_to_deck->at(i)->setdiscount(discount);
        int arr_price[] = {white, red, pink, blue, green};
        point_to_deck->at(i)->setprice(arr_price);
        line_stream.clear(); //Clear out the stream for another string
    }
}

//draw the board
void Game::drawBoard() {
    int vpos = 0;
    int hpos = 1;

    //draw the Card decks
    if (!vendors.empty()) {
        drawDeck(vpos, hpos, "Vendors");
    }
    vpos += Card_HEIGHT + 1;
    if (!transport.empty()) {
        drawDeck(vpos, hpos, "Transport");
    }
    vpos += Card_HEIGHT + 1;
    if (!mines.empty()) {
        drawDeck(vpos, hpos, "Mines");
    }

    //draw the in play Cards
    vpos = 0;
    hpos = Card_WIDTH + 2;
    for (int i = 0; i < Card_ROWS; i++) {
        for (int j = 0; j < Card_COLS; j++) {
            if (grid[i][j] != nullptr) {
                drawCard(vpos, hpos, grid[i][j]);
            }
            hpos += Card_WIDTH + 1;
        }
        vpos += Card_HEIGHT + 1;
        hpos = Card_WIDTH + 2;
    }

    //draw the gems
    int gemOffset = ((Card_WIDTH + 1) * 5) + 3;
    vpos = 1;
    hpos = gemOffset;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            int index = (i * 2) + j;

            if (gem_bank[index] > 0) {
                board[vpos][hpos + 5] = gem_bank[index] + 48;
                board_color[vpos][hpos + 5] = indexToColor(index);
                drawGem(vpos + 1, hpos, indexToColor(index));
            }
            hpos += Card_WIDTH;
        }
        vpos += Card_HEIGHT + 1;
        hpos = gemOffset;
    }

    //draw players
    drawPlayer(vpos + 1, 1, &player1);
    drawPlayer(vpos + 1, ((Card_WIDTH + 1) * 4), &player2);

    //draw nobles
    if (play_nobles) {
        hpos = ((Card_WIDTH + 1) * 7) + 2;
        vpos = 2;
        for (int i = 0; i < NUM_NOBLES; i++) {
            if (nobles.at(i) != nullptr) {
                drawNoble(vpos, hpos, nobles.at(i));
            }
            vpos += Card_HEIGHT + 1;
        }
    }

    //print out the finished board
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board_color[i][j] != "white") {
                screen_fg(board_color[i][j]);
            }
            cout << board[i][j];
            screen_fg("white");
        }
        cout << endl;
    }
    cout << endl;
}


//converts a given index to a color
string Game::indexToColor(int number) {
    if (number == 0) {
        return "white";
    }
    if (number == 1) {
        return "red";
    }
    if (number == 2) {
        return "pink";
    }
    if (number == 3) {
        return "blue";
    }
    if (number == 4) {
        return "green";
    }
    if (number == 5) {
        return "yellow";
    }
    return "";
}


//draws a noble at the specified position of the board
void Game::drawNoble(int vpos, int hpos, Card *c) {
    string top = "|\\  /*\\  /|";
    string second = "| \\/   \\/ |";
    string blank = "N         N";
    string bottom = "~~~~~~~~~~~";

    //draw the basic Card outline
    int length = top.length();
    for (int i = 0; i < length; i++) {
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];

        for (int j = 2; j < 5; j++) {
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + 5][hpos + i] = bottom[i];
    }

    //draw the prices
    vpos += 2;
    for (int i = 0; i < GEMS - 1; i++) {
        int price = c->getPrice(i);
        if (price > 0) {
            board_color[vpos][hpos + 4] = indexToColor(i);
            board_color[vpos][hpos + 5] = indexToColor(i);
            board_color[vpos][hpos + 6] = indexToColor(i);
            board[vpos][hpos + 4] = '|';
            board[vpos][hpos + 5] = price + 48;
            board[vpos][hpos + 6] = '|';
            vpos++;
        }
    }
}

//draws a player at the specified position of the board
void Game::drawPlayer(int vpos, int hpos, Player *p) {
    //player string
    string prestige = to_string(p->totalPrestige());
    string player_s;
    if (hpos < 10) {
        player_s = " PLAYER 1: " + prestige + " prestige point(s)";
    } else {
        player_s = " PLAYER 2: " + prestige + " prestige point(s)";
    }
    int length = player_s.length();
    for (int i = 0; i < length; i++) {
        board[vpos][hpos + i] = player_s[i];
    }

    //bonus string
    vpos++;
    string bonus_s = "Discounts: ";
    length = bonus_s.length();
    for (int i = 0; i < GEMS - 1; i++) {
        int bonus = p->getDiscount(i);
        bonus_s += "|" + to_string(bonus) + "| ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
        if (bonus >= 10) {
            length += 1;
        }
    }
    for (int i = 0; i < length; i++) {
        board[vpos][hpos + i] = bonus_s[i];
    }

    //gem string
    vpos++;
    string gems_s = "     Gems: ";
    length = gems_s.length();
    for (int i = 0; i < GEMS; i++) {
        int gems = p->getGemCount(i);
        gems_s += "(" + to_string(gems) + ") ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
    }
    for (int i = 0; i < length; i++) {
        board[vpos][hpos + i] = gems_s[i];
    }

    //reserve string
    vpos++;
    string reserve_s = " Reserved:";
    length = reserve_s.length();
    for (int i = 0; i < length; i++) {
        board[vpos][hpos + i] = reserve_s[i];
    }

    hpos += reserve_s.length();
    for (int i = 0; i < 3; i++) {
        Card *c = p->getReserve(i);
        if (c == nullptr) {
            drawDeck(vpos, hpos, "");
        } else {
            drawCard(vpos, hpos, c);
        }
        hpos += Card_WIDTH + 1;
    }
}

//draws a Card deck at the specified position of the board
void Game::drawDeck(int vpos, int hpos, string type) {
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic Card outline
    int length = top.length();
    for (int i = 0; i < length; i++) {
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];
        for (int j = 2; j < Card_HEIGHT; j++) {
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + Card_HEIGHT][hpos + i] = bottom[i];
    }

    int start_pos = hpos + (blank.length() / 2) - (type.length() / 2);
    length = type.length();
    for (int i = 0; i < length; i++) {
        board[vpos + (Card_HEIGHT / 2)][start_pos + i] = type[i];
    }
}

//draws a gem at the specified position of the board
void Game::drawGem(int vpos, int hpos, string color) {
    string gem_image[6];
    gem_image[0] = "  _______  ";
    gem_image[1] = ".'_/_|_\\_'.";
    gem_image[2] = "\\`\\  |  /`/";
    gem_image[3] = " `\\  | //' ";
    gem_image[4] = "   `\\|/`   ";
    gem_image[5] = "     `     ";

    for (int i = 0; i < GEMS; i++) {
        int length = gem_image[i].length();
        for (int j = 0; j < length; j++) {
            board[vpos + i][hpos + j] = gem_image[i][j];
            board_color[vpos + i][hpos + j] = color;
        }
    }
}

//draws a Card at the specified position of the board
void Game::drawCard(int vpos, int hpos, Card *c) {
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic Card outline
    int length = top.length();
    for (int i = 0; i < length; i++) {
        board[vpos][hpos + i] = top[i];

        if (i == 8 || i == 9) {
            board_color[vpos + 1][hpos + i] = c->getGemColor();
        }
        board[vpos + 1][hpos + i] = second[i];

        for (int j = 2; j < Card_HEIGHT; j++) {
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + Card_HEIGHT][hpos + i] = bottom[i];
    }

    //draw the prestige number and gem
    board[vpos + 2][hpos + 3] = c->getPrestige() + 48;
    board_color[vpos + 2][hpos + 8] = c->getGemColor();
    board_color[vpos + 2][hpos + 9] = c->getGemColor();
    board[vpos + 2][hpos + 8] = '\\';
    board[vpos + 2][hpos + 9] = '/';
    board_color[vpos + 1][hpos + 8] = c->getGemColor();
    board_color[vpos + 1][hpos + 9] = c->getGemColor();
    board[vpos + 1][hpos + 8] = '_';
    board[vpos + 1][hpos + 9] = '_';

    //draw the prices
    vpos += 4;
    for (int i = 0; i < GEMS - 1; i++) {
        int price = c->getPrice(i);
        if (price > 0) {
            board_color[vpos][hpos + 2] = indexToColor(i);
            board_color[vpos][hpos + 3] = indexToColor(i);
            board_color[vpos][hpos + 4] = indexToColor(i);
            board[vpos][hpos + 2] = '(';
            board[vpos][hpos + 3] = price + 48;
            board[vpos][hpos + 4] = ')';
            vpos++;
        }
    }
}


//clear the board display
void Game::clearBoard() {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
    }
}
