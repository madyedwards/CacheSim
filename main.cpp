#include <iostream>
#include <sstream>
#include <fstream>
#include "Cache.h"
using namespace std;

int main() {
    // read in .trace file
    // only care about the second element, skip the first and the third in a line
    Full fullyCache;
    Direct directCache;
    Set setCache;

    int choice;

    cout << "Code by Madison Edwards for CDA3101!" << endl;
    cout << "Which cache to simulate?" << endl;
    cout << "1. Fully Associative" << endl;
    cout << "2. Direct Associative" << endl;
    cout << "3. Set Associative" << endl;
    cin >> choice;

    int numSets = 0;
    int lineSize = 0;
    int replacementStrategy= 0;
    int setLength = 0;

    if (choice == 1) {
        cout << "What replacementStrategy will be used?" << endl;
        cout << "1. LRU" << endl;
        cout << "2. FIFO" << endl;
        cin >> replacementStrategy;

    }
    else if (choice == 2) {
        cout << "What is the line size? Please input in bytes." << endl;
        cin >> lineSize;
    }
    else if (choice == 3) {
        cout << "How many sets are there? " << endl;
        cin >> numSets;
        cout << "What is the line size?" << endl;
        cin >> lineSize;
        cout << "What replacementStrategy will be used?" << endl;
        cout << "1. LRU" << endl;
        cout << "2. FIFO" << endl;
        cin >> replacementStrategy;
        if (numSets == 2)
            setLength = 1;
        else if (numSets == 4)
            setLength = 2;
        else if (numSets == 8)
            setLength = 3;


    }

    int tagSize;
    cout << "What is the tag size?" << endl;
    cin >> tagSize;

    int cacheSize;
    cout << "And the cache size?" << endl;
    cin >> cacheSize;

    // all hard-coded data collected, now to implement into the structure

    fstream file("swim.trace", ios::in);
    string row, hexAddress;

    int numOfAddresses = 0;

    cout << "Running simulation..." << endl;

    if (file.is_open()) {
        while (getline(file, row)) {

            //grab the address from the .trace
            stringstream split(row);

            getline(split, hexAddress, ' ');
            getline(split, hexAddress, ' ');
            //cout << hexAddress << " is ";

            // increment number of addresses that are in the cache
            numOfAddresses++;
            //cout << numOfAddresses << endl;

            if (choice == 1) {
                //calculate the tag
                int tag = fullyCache.CalcTag(hexAddress, tagSize);
                bool exist = fullyCache.CheckAddress(tag,hexAddress);

                // create address item that has hexAddress instead of an offset
                Cache::Address address = Cache::Address(hexAddress, 0, numOfAddresses);

                if (!exist) {
                    //add to the cache; remember to check for duplicates in add item
                    // increment the total number of accesses there have been
                    int currentCacheSize = fullyCache.SizeOfCache(tag);

                    if (cacheSize <= currentCacheSize) {
                        // if program too big
                        //replacement strategies
                        if (replacementStrategy == 1)
                            fullyCache.LRUReplace(tag,address);
                        else
                            fullyCache.FIFOReplace(tag, address);

                        fullyCache.AccessIncrement();
                    }
                    else {
                        // if size fine, add
                        fullyCache.AccessIncrement();
                        fullyCache.AddFullItem(tag, address);
                    }
                }
                else {

                    fullyCache.HitIncrement(tag, hexAddress);
                    //maybe also in an else statement
                    fullyCache.AccessIncrement();
                }

            }
            else if (choice == 2){
                //adding, checks, etc when its direct
                // calc tag and line
                int tag = directCache.CalcTag(hexAddress,tagSize);
                //for now, hard code the cache size and line size *****************************

                int numLines = directCache.NumberOfLines(cacheSize,lineSize);
                int line = directCache.CalcLine(tagSize,numLines, hexAddress);

                bool exist = directCache.CheckAddress(tag, line, hexAddress); //use this to determine if needing replacement
                Cache::Address address = Cache::Address(hexAddress, 0, numOfAddresses);

                if (!exist) {
                    int currentCacheSize = directCache.SizeOfCache(tag);
                    if (cacheSize <= currentCacheSize) {
                        //if too big, replace with address
                        directCache.ReplaceAddress(tag, line, address);
                        directCache.AccessIncrement();
                    }
                    else {
                        //add to the cache; remember to check for duplicates in add item
                        directCache.AccessIncrement();
                        directCache.AddDirectItem(tag, line, address);
                    }
                }
                else {
                    directCache.HitIncrement(tag, line, hexAddress);
                    directCache.AccessIncrement();
                }
            }
            else if (choice == 3) {
                //calculate the tag
                int tag = setCache.CalcTag(hexAddress, tagSize);

                int numLines = setCache.NumberOfLines(cacheSize,lineSize);
                int numSet = numSets * numLines; //number of lines in a set

                int set = setCache.CalcSet(tagSize,setLength, hexAddress);

                // create address item that has hexAddress instead of an offset
                Cache::Address address = Cache::Address(hexAddress, 0, numOfAddresses);
                bool exist = setCache.CheckAddress(tag,set, hexAddress);

                if (!exist) {
                    //add to the cache; remember to check for duplicates in add item
                    // increment the total number of accesses there have been
                    int currentCacheSize = setCache.SizeOfCache(tag, set);

                    if (cacheSize <= currentCacheSize) {
                        //replacement strategies
                        if (replacementStrategy == 1)
                            setCache.LRUReplace(tag,set,address);
                        else
                            setCache.FIFOReplace(tag, set, address);

                        setCache.AccessIncrement();
                    }
                    else {
                        setCache.AccessIncrement();
                        setCache.AddSetItem(tag, set, address);
                    }
                }
                else {

                    //maybe also in an else statement
                    setCache.HitIncrement(tag, set, hexAddress);
                    setCache.AccessIncrement();
                }

            }
        }
    }
    if (choice == 1) {
        float hits = fullyCache.hitRateCounter;
        float total = fullyCache.totalAccesses;
        float hitRate = hits/total;
        cout << "hit rate is: " << hitRate << endl;
        cout << hits << " " << total;
    }
    else if (choice == 2) {
        float hits = directCache.hitRateCounter;
        float total = directCache.totalAccesses;
        float hitRate = hits/total;
        cout << "hit rate is: " << hitRate << endl;
        cout << hits << " " << total;
    }
    else if (choice == 3) {
        float hits = setCache.hitRateCounter;
        float total = setCache.totalAccesses;
        float hitRate = hits/total;
        cout << "hit rate is: " << hitRate << endl;
        cout << hits << " " << total;
    }
}
