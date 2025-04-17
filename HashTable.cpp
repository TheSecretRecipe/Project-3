// Project 3: Comparison of 2 Data Structures
// Hash Table Custom Implementation

/*

Notes:
    Hash Table will use separate chaining to resolve collisions.

*/

#include "HashTable.h"

using namespace std;

// int obj hash function (modulo-based hash calculation)
int HashTable::hash(const int& intObj)
{
    return intObj % size;
}

// str obj hash function (modulo-based hash calculation)
int HashTable::hash(const string& str)
{
    long long n = 0;
    // uses exponentiation for 15 letters at most
    int numExpo = min((int)str.length(),15);
    // hash calculation
    int i = 0;
    for(;i<numExpo;i++)
    {
        int charIndex = str[i] % STOI_HASH_EXPO;
        n += (long long)(charIndex*pow(STOI_HASH_EXPO,i));
    }
    // remaining letters
    for(;i<(int)str.length();i++)
    {
        n += (long long)(str[i]);
    }
    // modulo to fit hash size
    return (int)(n % (long long)size);
}

// method to resize the hash table if needed
void HashTable::resize()
{
    // updates size & lf
    size *= 2;
    loadFactor = (double)entries/size;

    vector<vector<string>>** oldArr = arr;

    // update to larger array, each resizing doubles array size
    arr = new vector<vector<string>>*[size]();

    // copies all elements, recalculates hash code for old insertions
    string obj;
    vector<vector<string>>* list;
    for(int i=0;i<size/2;i++)
        if(oldArr[i] != nullptr)
        {
            for(int j=0;j<oldArr[i]->size();j++)
            {
                obj = (*oldArr[i])[j].back();
                // insertion
                int newHashCode = hash(obj);
                // if bucket is empty, create a new list, then add the data as an entry to the list
                if(arr[newHashCode] == nullptr)
                {
                    // must dynamically allocate memory for accessibility
                    list = new vector<vector<string>>();
                    arr[newHashCode] = list;
                    arr[newHashCode]->push_back((*oldArr[i])[j]);
                }
                // otherwise, simply append the new entry to the list
                else
                {
                    arr[newHashCode]->push_back((*oldArr[i])[j]);
                }
            }
        }

    // deletes all dynamic chains within old array
    for(int i=0;i<size/2;i++)
        delete oldArr[i];
    // deletes old array
    delete[] oldArr;
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
bool HashTable::insert(const string& obj, vector<string>& additionalData)
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
    // removes the object itself after processing
    additionalData.pop_back();
    // returns if separate chaining is required
    return chainingRequired;
}

// returns data associated with object(s), or empty vector if search fails
vector<vector<string>> HashTable::search(const string& obj)
{
    vector<vector<string>> allData;
    // get hash code
    int hashCode = hash(obj);
    // at least one entry exists, so we must search
    if(arr[hashCode] != nullptr)
        for(vector<string> data : *arr[hashCode])
            if(obj == data.back())
            {
                data.pop_back();
                allData.push_back(data);
            }
    return allData;
}

// returns false if no data is stored at index
bool HashTable::remove(const string& obj)
{
    // get hash code
    int hashCode = hash(obj);
    // at least one entry exists, so we must search
    if(arr[hashCode] != nullptr)
        {
        int bef = arr[hashCode]->size();
        arr[hashCode]->erase(remove_if(arr[hashCode]->begin(), arr[hashCode]->end(),
            [&](const vector<string>& data){return obj == data.back();}), arr[hashCode]->end());
        int aft = arr[hashCode]->size();
        // nothing found
        if(bef == aft)
            return false;
        // update entries and load factor
        entries -= (bef - aft);
        loadFactor = (double)entries/size;
        }
    // found
    return true;
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