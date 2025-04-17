// Project 3: Comparison of 2 Data Structures
// Hash Table Custom Implementation

#include <fstream>
#include <sstream>
#include "HashTable.h"

using namespace std;

// inserts data into hash table
// returns if insertion was successful
static bool insertData(HashTable& hashTable, string& fileName, int keyColumn = 1)
{
    // getting csv file data
    ifstream file(fileName);
    // check to see if file was successfully opened
    if(!file.is_open() || keyColumn < 0 || keyColumn >= ROW_LENGTH)
        return false;

    // processing and inserting csv data
    string dataRow;
    // variables to assist with hash table input
    vector<string> dataVector;
    dataVector.reserve(ROW_LENGTH);
    // skip first line (header labels)
    getline(file, dataRow);
    // for each row of data (excluding header labels)
    while(getline(file,dataRow))
    {
        stringstream stringStream(dataRow);
        string entry;
        // for each item in the row
        while(getline(stringStream,entry,','))
        {
            dataVector.push_back(entry);
        }
        // separating key and value (vector of additional data)
        string key = dataVector[keyColumn];
        dataVector.erase(dataVector.begin() + keyColumn);
        // add each item and associated data to the hash table
        hashTable.insert(key, dataVector);
        dataVector.clear();
    }

    file.close();
    return true;

}