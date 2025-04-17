// Project 3: Comparison of 2 Data Structures
// Hash Table Custom Implementation

#include <clocale>
#include <cstdio>
#include <fstream>
#include <regex>
#include <sstream>
#include "HashTable.h"

using namespace std;

// inserts data into hash table
// returns if insertion was successful
static bool insertData(HashTable& hashTable, string&& fileName, int keyColumn = 1)
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

// prints formatted data if possible, otherwise returns false
static bool printFormattedData(string&& fileName, vector<string>& dataVector, int keyColumn = 1)
{
    // getting csv file data
    ifstream file(fileName);
    // check to see if file was successfully opened
    if(!file.is_open() || keyColumn < 0 || keyColumn >= ROW_LENGTH)
        return false;

    // examining first row to find column headers
    string firstRow;
    getline(file, firstRow);
    vector<string> headerVector;
    // parsing each header
    stringstream stringStream(firstRow);
    string entry;
    // for each item in the row
    while(getline(stringStream,entry,','))
    {
        headerVector.push_back(entry);
    }
    // removing column header corresponding to key
    headerVector.erase(headerVector.begin() + keyColumn);

    // patterns for matching and formatting
    regex patternInt("^[0-9]+$");
    regex patternDec("^[.0-9]+$");

    // setting correct locale
    setlocale(LC_ALL, "");

    // printing formatted data
    for(int i=0;i<ROW_LENGTH-1;i++)
    {
        if(dataVector[i].empty())
            printf("%-20s%s",(headerVector[i] + ":").c_str(),"None");
        else if(regex_match(dataVector[i],patternInt))
            printf("%-20s%'d",(headerVector[i] + ":").c_str(),stoi(dataVector[i]));
        else if(regex_match(dataVector[i],patternDec))
            printf("%-20s%-10.2f%%",(headerVector[i] + ":").c_str(),stod(dataVector[i]));
        else
            printf("%-20s%s",(headerVector[i] + ":").c_str(),dataVector[i].c_str());
        printf("\n");
    }
    return true;
}