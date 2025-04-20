// Project 3: Comparison of 2 Data Structures
// B-Tree Custom Implementation

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

// minimum degree (defines the range for number of keys)
const int T = 3;

struct Player {
    string title;
    string username;
    string name;
    int rate;
    string country;
    int games;
    int won, draw, lost;
    double wonperc, drawperc, lostperc;
};

class BTreeNode {

public:
    string keys[2 * T - 1];
    Player players[2 * T - 1];
    BTreeNode* children[2 * T];
    int n;
    bool leaf;

    explicit BTreeNode(bool isLeaf);

    void traverse();
    void insertNonFull(string& k, const Player& player);
    void splitChild(int i, BTreeNode* y);
    Player searchUsername(const string& k);

};

class BTree {

public:
    BTreeNode* root;

    BTree();

    void traverse();
    void insert(string& k, const Player& player);
    Player search(const string& k);

    ~BTree(); // Destructor

};

#endif //BTREE_H
