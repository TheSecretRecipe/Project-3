// Project 3: Comparison of 2 Data Structures
// Hash Table Custom Implementation

/*

Notes:
    Hash Table will use separate chaining to resolve collisions.

*/

#include "HashTable.h"

using namespace std;

// int obj hash function (modulo-based hash calculation)
int HashTable::hash(int intObj)
{
    return intObj % size;
}

// str obj hash function (modulo-based hash calculation)
int HashTable::hash(string str)
{
    long long n = 0;
    // assuming input is mostly lowercase letters
    for(int i=0;i<str.length();i++)
    {
        int charIndex = ((int)str[i]-97)/3;
        n += (long long)(charIndex*pow(9,i));
    }
    // modulo to fit hash size
    return (int)(abs(n) % (long long)size);
}

// method to resize the hash table if needed
void HashTable::resize()
{
    // create larger array, each resizing doubles array size
    vector<vector<string>>** largerArr = new vector<vector<string>>*[2*size]();

    // copies all elements
    for(int i=0;i<size;i++)
        largerArr[i] = arr[i];
    // frees up memory from old array
    delete[] arr;

    // updates array pointer
    arr = largerArr;

    // updates size & lf
    size *= 2;
    loadFactor = (double)entries/size;
}

// constructor
HashTable::HashTable()
{
    // initializing values
    loadFactor = 0;
    size = INITIAL_SIZE;
    entries = 0;
    // all pointers are initially nullptr
    arr = new vector<vector<string>>*[size]();
}

// destructor
HashTable::~HashTable()
{
    // deletes all dynamic chains within array
    for(int i=0;i<size;i++)
        delete arr[i];
    // deletes dynamic array
    delete[] arr;
}

// returns true if separate chaining is required
bool HashTable::insert(string obj, vector<string> additionalData)
{
    bool chainingRequired = false;
    // adds the object itself to the data
    additionalData.push_back(obj);
    // get hash code
    int hashCode = hash(obj);
    // if bucket is empty, create a new list, then add the data as an entry to the list
    if(arr[hashCode] == nullptr)
    {
        // must dynamically allocate memory for accessibility
        vector<vector<string>>* list = new vector<vector<string>>();
        arr[hashCode] = list;
        arr[hashCode]->push_back(additionalData);
    }
    // otherwise, simply append the new entry to the list
    else
    {
        arr[hashCode]->push_back(additionalData);
        chainingRequired = true;
    }
    // update entries and lf
    entries++;
    loadFactor = (double)entries/size;
    // resize if load factor threshold reached
    if(loadFactor > MAX_LOAD_FACTOR)
        resize();
    // returns if separate chaining is required
    return chainingRequired;
}

// returns data associated with object, or empty vector if search fails
vector<string> HashTable::search(string obj)
{
    // get hash code
    int hashCode = hash(obj);
    // at least one entry exists, so we must search
    if(arr[hashCode] != nullptr)
        for(vector<string> data : *arr[hashCode])
            if(obj == data.back())
            {
                data.pop_back();
                return data;
            }
    // not found
    return vector<string>();
}

// returns false if no data is stored at index
bool HashTable::remove(string obj)
{
    // get hash code
    int hashCode = hash(obj);
    // at least one entry exists, so we must search
    if(arr[hashCode] != nullptr)
        for(int i=0;i<arr[hashCode]->size();i++)
            if(obj == (*arr[hashCode])[i].back())
            {
                arr[hashCode]->erase(arr[hashCode]->begin() + i);
                // update entries and lf
                entries--;
                loadFactor = (double)entries/size;
                // successfully removed
                return true;
            }
    // not found
    return false;
}

// returns number of inserted objects
int HashTable::cardinality()
{
    return entries;
}

bool HashTable::isEmpty()
{
    return entries == 0;
}

void HashTable::clear()
{
    // deletes all dynamic chains within array
    for(int i=0;i<size;i++)
        delete arr[i];
    // deletes dynamic array
    delete[] arr;

    // initializing values
    loadFactor = 0;
    size = INITIAL_SIZE;
    entries = 0;
    // all pointers are initially nullptr
    arr = new vector<vector<string>>*[size]();
}