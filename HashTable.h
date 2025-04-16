#ifndef HASHTABLE_H
#define HASHTABLE_H

// Project 3: Comparison of 2 Data Structures
// Hash Table Custom Implementation

/*

Notes:
    Hash Table will use separate chaining to resolve collisions.

*/

#include <climits>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

// initial size of the hash table
const int INITIAL_SIZE = 1000;
// maximum tolerable load factor
const double MAX_LOAD_FACTOR = 1.0;

class HashTable
{

private:
    // (entries) / (total number of buckets)
    double loadFactor;
    // current size of the hash table
    int size;
    // number of buckets that are filled
    int entries;
    // dynamically allocated array for hash table
    vector<vector<string>>** arr;

    // hash function for int objects (modulo-based)
    int hash(int intObj);
    // hash function for str objects (modulo-based)
    int hash(string str);
    // method to resize the hash table if needed
    void resize();

public:
    // constructor
    HashTable();
    // destructor
    ~HashTable();

    // returns true if separate chaining is required
    bool insert(string obj, vector<string> additionalData);
    // returns data associated with object, or empty vector if search fails
    vector<string> search(string obj);
    // returns false if no data is stored at index
    bool remove(string obj);
    // returns number of inserted objects
    int cardinality();
    // returns if the hash table is empty
    bool isEmpty();
    // clears hash table and sets everything to default
    void clear();
};

#endif //HASHTABLE_H