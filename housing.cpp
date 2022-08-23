/*
* COMP11
* Spring 2021
* housing.cpp
* purpose: reads in info from a file then indexes that in an array of house
* structs and lets the user perform operations on the array.
* Ethan Nanavati
*/


#include <iostream>
#include <fstream>

using namespace std;


const int ROWS = 50;
const int COLS = 20;



struct House {


        bool   no_house_here;  // true if there is NO house on this location
        int    id;             // id from the data file, converted to int
        string lot_code;       // lot_code string from the data file

        float price;
        int bedrooms;
        string color;
        string available;
};



int user_input(House all_houses[ROWS][COLS]);

// read one line of the input_file; return corresponding House
House read_one_house(ifstream& input_file);

// print information about a single house
void print_house(House h);

// given a lot code, compute the column and row (respectively)
// in the all_houses array
int
col_from_lot(string lot);

int
row_from_lot(string lot);

//  set the no_house_here marker for every position
//  in the array
void
fill_with_no_houses(int rows, int cols, House all_houses[ROWS][COLS]);


bool
read_all_house_data(string filename, House all_houses[ROWS][COLS]);

// PROTOTYPES FOR WEEK 2

bool command_loop(House all_houses[ROWS][COLS]);

void atest(House all_houses[ROWS][COLS]);
bool is_available(int house_id, House all_houses[ROWS][COLS]);

void mtest(House all_houses[ROWS][COLS], House results[ROWS * COLS]);
int matching_houses(float max_price, string desired_color,
                    int desired_bedrooms, House all_houses[ROWS][COLS],
                    House results[ROWS * COLS]);

void ntest(House all_houses[ROWS][COLS]);
int num_neighbors(int house_id, House all_houses[ROWS][COLS]);
int side_find(int row, int col);
int neighbor_calculator(int row, int col, House all_houses[ROWS][COLS], 
int position);
int house_tester(int row, int col, House all_houses[ROWS][COLS]);

bool rtest(House all_houses[ROWS][COLS]);
int rent_house(int house_id, House all_houses[ROWS][COLS]);


#include "housing_hooks.hpp"


int
main(int argc, char *argv[])
{
        // make sure there is a filename inputed in the command line       
        if (argc != 2) {
                cerr << "housing.cpp: please input a data file"
                     << endl;
                return 1;
        }
        
        // read from command line
        string filename = argv[1];
        bool filetest;
        House all_houses[ROWS][COLS];
        
        
        // fills the array with no houses
        fill_with_no_houses(ROWS, COLS, all_houses);
        
        // stop if the file wasn't read
        filetest = read_all_house_data(filename, all_houses);               
        if (filetest == false) { 
                return 1;
        }
        
        // begin asking the user which element they want and outputting it
        //user_input(all_houses);       
        
        command_loop(all_houses);
        
        return 0;
}

// to run the command loop required for week 2
bool command_loop(House all_houses[ROWS][COLS]) {
        
        string action;
        
        House results[ROWS * COLS];
        do {
                cout << "query-> ";
                cin >> action;
                
                if (action == "a") {
                        atest(all_houses);
                }
                else if (action == "m") {
                        mtest(all_houses, results);
                }
                else if (action == "n") {
                        ntest(all_houses);
                }
                else if (action == "r") {
                        if (rtest(all_houses)) {
                                return true;
                        }        
                }
                else if (action != "q") {
                        cout << "Unknown command " << action << endl;
                }
        
        } while (action != "q");
        return true;
}


// purpose: take input for is_available and print if the house is available
// Arg: the array of houses
void atest(House all_houses[ROWS][COLS]) {
        int house_id;
        
        cin >> house_id;
        
        if (is_available(house_id, all_houses)) {
                cout << "House " << house_id << " is available" << endl;
        }
        
        else {
                cout << "Sorry, House " << house_id << " is not available";
                cout << endl;
        }
        
}

// purpose: check to see if a house (ID number) is available
// Arg: the house id and the array of houses
// returns: tells whether the house is available
// figure how to reduce the level of indentation here
bool is_available(int house_id, House all_houses[ROWS][COLS]){
        
        // set to -1 to see if it is altered or not
        int user_row = -1;
        int user_col;
        
        // goes through the array elements which have houses and finds a match
        for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                        if ((all_houses[i][j].no_house_here == false) and
                        (all_houses[i][j].id == house_id)) {
                                user_row = i;
                                user_col = j;
                        }
                }
        }
        
        if (user_row == -1) {
                return false;
        }
        
        if (all_houses[user_row][user_col].available == "available") {
                return true;
        }
        
        else {
                return false;
        }
}



// purpose: take input for matching_houses and print the matches
// Arg: the house array and the matches
void mtest(House all_houses[ROWS][COLS], House results[ROWS * COLS]) {
        
        (void)results;
        int length;
        float max_price;
        string desired_color;
        int desired_bedrooms;
        
        cin >> max_price;
        cin >> desired_color;
        cin >> desired_bedrooms;
        
        
        length = matching_houses(max_price, desired_color, desired_bedrooms,
                                all_houses, results);
        
        for (int i = 0; i < length; ++i) {
                print_house(results[i]);
        }
        
}

// purpose: match user preferences to element in the house array
// Arg: the user's house specifications, the array of houses, and the matches
// returns: the length of the array of matches
// make the indentations fewer
int matching_houses(float max_price, string desired_color,
                    int desired_bedrooms, House all_houses[ROWS][COLS],
                    House results[ROWS * COLS]) {
        
        int length = 0;   
        
        cout << "Matching Houses:" << endl;
        
        // find all houses matching the specs and store in an array
        for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                        if ((all_houses[i][j].no_house_here == false) and
                        (all_houses[i][j].price <= max_price) and 
                        (all_houses[i][j].color == desired_color) and 
                        (all_houses[i][j].bedrooms >= 
                        desired_bedrooms)) {
                                results[length] = all_houses[i][j];
                                length = length + 1;
                        }
                }
        }
        
        return length;
}    



// purpose: take inputs for num_neighbors and output messages
// Arg: the array of houses
void ntest(House all_houses[ROWS][COLS]) {
        int house_id;
        int neighbors;
        
        cin >> house_id;
        
        // calculate number of neighbors
        neighbors = num_neighbors(house_id, all_houses);
        
        if (neighbors == 0) {
                cout << "You have no neighbors, practice your drums!" << endl;
        }
        else {
                cout << "You have " << neighbors << " neighbors!" << endl;
        }
        
}

// purpose: find the position of the house in the array and how many neighbors
// Arg: the position of the array element and the array of houses
// returns: tells how many neighbors there are
int num_neighbors(int house_id, House all_houses[ROWS][COLS]){
        
        int position; 
        int user_row = -1;
        int user_col;
        
        // goes through the array elements which have houses and finds a match
        for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                        if ((all_houses[i][j].no_house_here == false) and
                        (all_houses[i][j].id == house_id)) {
                                user_row = i;
                                user_col = j;
                        }
                }
        }
        // if there was no ID match
        if (user_row == -1) {
                return 0;
        }
        
        // this is where we find the number of neighbors
        position = side_find(user_row, user_col);
        return neighbor_calculator(user_row, user_col, all_houses, position);
}

// purpose: determine which side/corner the house is in or if it is not on the
// edge
// Arg: the position of the array element
// returns: tells which sides of the array the element is touching
int side_find(int row, int col) {
        if (row == 0) {
                if (col == 0){
                        return 0;
                }
                if (col == COLS - 1) {
                        return 2;
                }
                else 
                        return  1;
        }
        if (col == COLS - 1) {
                if (row == ROWS - 1) {
                        return 4;
                }
                else 
                        return 3;       
        }
        if (row ==  ROWS - 1) {
                if (col == 0) {
                        return 6;
                }
                else
                        return 5;
        }
        if (col == 0) {
                return 7;
        }
        else
                return 8;
}

// purpose: calculate how many neighbors for a house
// Arg: the coordinates of the house, the house array, and the way the house is 
// positioned in the array ie. which walls touch it
// returns: the number of neighbors a house has
// for the style grader: sorry about this one
int neighbor_calculator(int row, int col, House all_houses[ROWS][COLS], 
        int position) {
        int neighbors = 0;
        
        // each spot that has a neighbor contributes + 1 to 'neighbors'
        if (position == 0) {
                neighbors = neighbors + house_tester(row + 1, col, all_houses)
                 + house_tester(row, col + 1, all_houses) + house_tester(row +
                 1, col + 1, all_houses);
        }
        else if (position == 1) {
                neighbors = neighbors + house_tester(row + 1, col, all_houses)
                 + house_tester(row, col + 1, all_houses) + house_tester(row +
                 1, col + 1, all_houses) + house_tester(row, col - 1,
                 all_houses) + house_tester(row + 1, col - 1, all_houses);
        }
        else if (position == 2) {
                neighbors = neighbors + house_tester(row + 1, col, all_houses)
                 + house_tester(row, col - 1, all_houses) + house_tester(row +
                 1, col - 1, all_houses);
        }
        else if (position == 3) {
                neighbors = neighbors + house_tester(row + 1, col, all_houses)
                 + house_tester(row - 1, col, all_houses) + house_tester(row,
                 col - 1, all_houses) + house_tester(row - 1, col - 1,
                 all_houses) + house_tester(row + 1, col - 1, all_houses);
        }
        else if (position == 4) {
                neighbors = neighbors + house_tester(row - 1, col, all_houses)
                 + house_tester(row, col - 1, all_houses) + house_tester(row -
                 1, col - 1, all_houses);
        }
        else if (position == 5) {
                neighbors = neighbors + house_tester(row, col + 1, all_houses)
                 + house_tester(row - 1, col, all_houses) + house_tester(row,
                 col - 1, all_houses) + house_tester(row - 1, col - 1,
                 all_houses) + house_tester(row - 1, col + 1, all_houses);
        }
        else if (position == 6) {
                neighbors = neighbors + house_tester(row, col + 1, all_houses)
                 + house_tester(row - 1, col, all_houses) + house_tester(row -
                 1, col + 1, all_houses);
        }
        else if (position == 7) {
                neighbors = neighbors + house_tester(row + 1, col, all_houses)
                 + house_tester(row, col + 1, all_houses) + house_tester(row +
                 1, col + 1, all_houses) + house_tester(row - 1, col,
                 all_houses) + house_tester(row - 1, col + 1, all_houses);
        }
        else if (position == 8) {
                neighbors = neighbors + house_tester(row + 1, col, all_houses)
                 + house_tester(row, col + 1, all_houses) + house_tester(row +
                 1, col + 1, all_houses) + house_tester(row - 1, col,
                 all_houses) + house_tester(row, col - 1, all_houses) +
                 house_tester(row - 1, col - 1, all_houses) + house_tester(row
                 + 1, col - 1, all_houses) + house_tester(row - 1, col + 1,
                 all_houses);
        }

        // to see if some error occured
        else {
                return -1;
        }
        
        return neighbors;
}

// purpose: test if a house is booked
// Arg: the coordinates in the array and the array itself
// returns: tells whether the house is booked or not
int house_tester(int row, int col, House all_houses[ROWS][COLS]) {
        if ((all_houses[row][col].no_house_here == false) and
        (all_houses[row][col].available == "booked"))
                return 1;
        else
                return 0;
}



// purpose: take inputs for rent_house and output messages based on the results
// of rent_house
// Arg: the array of houses
// returns: tells whether the house was rented
bool rtest(House all_houses[ROWS][COLS]) {
        int house_id;
        int rented;
        
        cin >> house_id;
        
        rented = rent_house(house_id, all_houses);
        
        //see if a house was rented and output a message
        if (rented == 0) {
                cout << 
                "You can't rent a house that doesn't exist, but good try";
                cout << endl;
        }
        else if (rented == 1) {
                cout << "Congrats, you rented a house! Hope your door knobs";
                cout << " don't fall off" << endl;
                return true;
        }
        else {
                cout << "Too late...i hear the dumpster behind hill has some";
                cout << " spaces open" << endl;
        }
        return false;
}

// purpose: convert id into an array element and see if it is available and if
// so, books it
// Arg: the house id and the array of houses
// returns: tells if a house is available
int rent_house(int house_id, House all_houses[ROWS][COLS]) {
        
        int user_row = -1;
        int user_col;
        
        // goes through the array elements which have houses and finds a match
        for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                        if ((all_houses[i][j].no_house_here == false) and
                        (all_houses[i][j].id == house_id)) {
                                        user_row = i;
                                        user_col = j;
                        }
                }
        }
        
        // if there was no ID match
        if (user_row == -1) {
                return 0;
        }
        
        // books the house for renter if it is available
        if (all_houses[user_row][user_col].available == "available") {
                all_houses[user_row][user_col].available = "booked";
                return 1;
        }
        
        // if the house was already booked
        else 
                return 2;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                      WEEK 1 STUFFS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



// read one line of the input_file; return corresponding House
// purpose: to read a house in from the input pile
// Arg: the input file
// returns: a house corresponding to one line of the input file
House read_one_house(ifstream& input_file) {
        
        int id;             
        string lot;      
        float price;
        int bedrooms;
        string color;
        string available;
        
        //read in the data on each house from the file
        input_file >> id >> lot >> price >> bedrooms >> color >> available;
        
        // store each house's data in a struct
        House one_house = {false, id, lot, price, bedrooms, color, available};
        
        return one_house;
        
}

// print information about a single house
// purpose: to print a house struct in the way the spec wants
// Arg: the house that will be printed
void print_house(House h)  {
                
        if (h.no_house_here == true) {
            cout << "No house" << endl;
        }
        else {
                if (h.available == "available") {
                        h.available = "Yes";
                }
                
                else {
                        h.available = "No";
                }
                
                //make sure that 'available' matches the required output
                cout << "Id: " << h.id << " Lot: " << h.lot_code;  
                cout << " Color: " << h.color << " Price: "; 
                cout << h.price << " Bedrooms: " << h.bedrooms; 
                cout  << " Available: " << h.available << endl; 
        }        
}


// given a lot code, compute the column and row (respectively)
// in the all_houses array
int
col_from_lot(string lot) {
                
        int column;
        
        // increments the ascii values to find a match to an integer
        for (int i = 0; i < 26; ++i) {
                if (lot[0] == 'A' + i) {
                        column = i;
                }
        }
        
        return column;
        
}

// purpose: to parse the row info from the lot number
// Arg: the lot info
// returns: the row number corresponding to the lot code
int
row_from_lot(string lot) {
        
        int digit = 0;
        int tens = 0;
        int row;
        
        // the following if statements parse the numbers from the string 'lot'
        if (lot.length() == 2) {
                for (int i = 0; i < 10; ++i) {
                        if (lot[1] == '0' + i) {
                                digit = i;
                        }
                }
        }
        
        if (lot.length() == 3) {
                for (int k = 0; k < 10; ++k) {
                        if (lot[2] == '0' + k) {
                                digit = k;
                        }
                        if (lot[1] == '0' + k) {
                                tens = k;
                        }
                }
        
        }
        
        row = digit + (tens * 10) - 1;
        
        return row;
}


// purpose: to set all elements in the array to no_house_here: true
// Arg: the row and column of the element being modified
void
fill_with_no_houses(int rows, int cols, House all_houses[ROWS][COLS]) {
        
        // fill all elements with no houses
        for (int i = 0; i < rows; ++i) {
                for (int k = 0; k < cols; ++k) {
                        all_houses[i][k].no_house_here = true;
                }
        }
        
}


// purpose: open a file, then feed house data into an array
// Arg: the filename for the file being read and the array of houses
// returns: tells whether the file was successfully opened
bool
read_all_house_data(string filename, House all_houses[ROWS][COLS]) {
                
        int numHouses;
        ifstream input_file;

        input_file.open(filename);
        
        // test to see if it opened
        if (not input_file.is_open()) {
                cerr << "housing.cpp: could not open file: "
                     << filename << endl;
                return false;           
        }
        
        input_file >> numHouses;
        
        //where the magic happens
        for (int i = 0; i < numHouses; ++i) {
                House Ahouse;
                int row;
                int col;
                
                //get a house, find its row and column, then put in the array
                Ahouse = read_one_house(input_file);
                row = row_from_lot(Ahouse.lot_code);
                col = col_from_lot(Ahouse.lot_code);
                all_houses[row][col] = Ahouse;
        }
        
        return true; 
}

// purpose: to tell the user what values are stored in the array
// Arg: the house array that will be used
// returns: the returns don't matter, it is just there so I can end it early
int user_input(House all_houses[ROWS][COLS]) {
        
        int row;
        int col = 1;
        
        cout << "Enter row: ";
        cin >> row;
        
        while (row >= 0) {
                
                cout << "Enter col: ";
                cin >> col;
                
                // see if there was a negative col input and if so end the loop
                if (col < 0) {
                        return 1;
                }
                
                print_house(all_houses[row][col]);
                
                // starts the next input here
                cout << "Enter row: ";
                cin >> row;
        }
        
        return 0;
}
