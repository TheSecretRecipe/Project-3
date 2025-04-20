// Project 3: Comparison of 2 Data Structures

#ifndef METHODS_H
#define METHODS_H

using namespace std;

#include <cctype>
#include <chrono>
#include <clocale>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "HashTable.h"
#include "BTree.h"

// SFML API for GUI
#include <SFML/graphics.hpp>

using namespace std;

// path names for files
const string DATA_FILE_PATH_NAME  = "data/players_blitz.csv";
const string FONT_FILE_PATH_NAME  = "fonts/ELEGANT TYPEWRITER Bold.ttf";
const string IMAGE_FILE_PATH_NAME = "images/Chess Background.png";

// maximum number of rows of data
const int MAX_DATA_ROWS = 200000;

class Methods
{

public:
    // inserts data into hash table
    // returns if insertion was successful
    static bool insertData(HashTable& hashTable, const string& fileName, int keyColumn = 1);
    // inserts data into B-Tree
    // returns if insertion was successful
    static bool insertData(BTree& tree, const string& fileName);
    // prints formatted data if possible
    // otherwise returns false
    static bool printFormattedData(const string& fileName, vector<string>& dataVector, const string& key, int keyColumn = 1);
    // prints formatted data for each data set
    static void printFormattedData(const string& fileName, vector<vector<string>>& doubleVector, const string& key, int keyColumn = 1, int maxEntries = MAX_DATA_ROWS);
    // returns a vector of data from a Player struct
    static vector<string> playerToVector(Player& p);
    // returns a vector of data from a Player struct for each player
    static vector<vector<string>> playerToVector(vector<Player>& vp);

    // prints an introduction paragraph to the program
    static void printIntroduction();

    // prints formatted data according to the following inputs:
    // string searchCriterion is "username", "country", "rating", or "games played"
    // string dataStructure is "B-tree" or "Hash Table"
    // string key is the search term from the user
    // int numEntries is between 0 and 999
    static void printData(const string& searchCriterion, const string& dataStructure, const string& key, int numEntries);

    // Methods for SFML handling
    static void render();

};

#endif //METHODS_H
