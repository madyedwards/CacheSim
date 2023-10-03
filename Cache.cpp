#include "Cache.h"
#include <iostream>
#include <sstream>
using namespace std;

// BASE CACHE FUNCTIONS ************************************************************
int Cache::CalcTag(std::string address, int tagSize) {
    //iterate first n elements that = tag size
    //then add those elements to a string
    string tag = " ";

    for (int i = 2; i < tagSize; i ++)
        tag = tag + address[i];

    // convert from hex to string
    signed int decimalTag;
    std::stringstream ss;
    ss << std::hex << tag;
    ss >> decimalTag;

    return decimalTag;
}
void Cache::AccessIncrement() {
    // increases when an access is made to the entire cache
    totalAccesses++;
}

int Cache::NumberOfLines(int cacheSize, int lineSize) {
    return cacheSize/lineSize;
}
// FULL CACHE FUNCTIONS ******************************************************************
void Full::HitIncrement(int tag, std::string address) {
    for (int i = 0; i < fullCache[tag].size(); i++) {
        string currentAddress = fullCache[tag][i].offsetName;

        if (address == currentAddress) {
            // it has been found !
            // need to increment counters if searching here
            fullCache[tag][i].accessCounter++;
            hitRateCounter++;

        }
    }
}
bool Full::CheckAddress(int tag, std::string address) {
    // return true if it exists already, false if it does not
    for (int i = 0; i < fullCache[tag].size(); i++) {
        string currentAddress = fullCache[tag][i].offsetName;

        if (address == currentAddress) {
            return true;
        }
    }
    return false;
}

void Full::AddFullItem(int tag, Cache::Address offset) {
    fullCache[tag].push_back(offset);
}

void Full::LRUReplace(int tag, Address newAddress) {
    int LRUcounter = fullCache[tag][0].accessCounter;
    int index = 0;

    //get smallest hit rate in the program
    for (int i = 0; i < fullCache[tag].size(); i++) {

        int currentHitRate = fullCache[tag][i].accessCounter;
        if (currentHitRate < LRUcounter) {
            LRUcounter = currentHitRate;
            index = i;

        }

    }
    //cout << "replacing " << fullCache[tag][index].offsetName << endl;
    // then replace the string with the smallest hit rate with the new address
    fullCache[tag][index] = newAddress;

}
void Full::FIFOReplace(int tag, Address address) {
    int leastAccessed = fullCache[tag][0].recencyCounter;
    int index = 0;

    //get first element that was added to be replaced
    for (int i = 0; i < fullCache[tag].size(); i++) {

        int currentHitRate = fullCache[tag][i].recencyCounter;
        if (currentHitRate < leastAccessed) {
            leastAccessed = currentHitRate;
            index = i;

        }
    }
    //cout << "replacing " << fullCache[tag][index].offsetName << endl;
    fullCache[tag][index] = address;

}
int Full::SizeOfCache(int tag) {
    return fullCache[tag].size();
}
// DIRECT CACHE FUNCTIONS **************************************************************
bool Direct::CheckAddress(int tag, int line, std::string address) {
    for (auto iter = directCache.begin(); iter != directCache.end(); ++iter) {
        if (iter->second.first == line && iter->second.second.offsetName == address) {
            return true;
        }
    }
    return false;
}
void Direct::AddDirectItem(int tag, int line, Cache::Address offset) {
    //directCache[tag][line].second = offset;
    directCache.insert(make_pair(tag, make_pair(line,offset)));
}
int Direct::CalcLine(int tagSize, int lineSize, std::string address) {
    //iterate first n elements that = tag size
    //then add those elements to a string
    string line = " ";

    for (int i = 2 + tagSize; i < lineSize; i ++)
        line = line + address[i];

    // THEN convert from hex to string
    signed int decimalLine;
    std::stringstream ss;
    ss << std::hex << line;
    ss >> decimalLine;

    return decimalLine;

}
void Direct::HitIncrement(int tag, int line, std::string offset) {
    for (auto iter = directCache.begin(); iter != directCache.end(); ++iter) {
        string currentAddress = iter->second.second.offsetName;

        if (offset == currentAddress) {
             //it has been found !
            // need to increment counters if searching here
            iter->second.second.accessCounter++;
            hitRateCounter++;
        }
    }
}

void Direct::ReplaceAddress(int tag, int line, Cache::Address address) {
    for (auto iter =  directCache.begin(); iter != directCache.end(); ++iter) {
        if (iter->first == line) {
            iter->second.second = address;
            return;
        }
    }
}
int Direct::SizeOfCache(int tag) {
    return directCache.size();
}
// SET CACHE FUNCTIONS *****************************************************************
void Set::AddSetItem(int tag, int set, Cache::Address offset) {
    setCache[set].push_back(make_pair(tag,offset));
}

int Set::CalcSet(int tagSize, int setSize, std::string address) {
    string set = " ";

    for (int i = 2 + tagSize; i < setSize; i ++)
        set = set + address[i];

    // THEN convert from hex to string
    signed int decimalSet;
    std::stringstream ss;
    ss << std::hex << set;
    ss >> decimalSet;

    return decimalSet;
}

bool Set::CheckAddress(int tag, int set, std::string address) {
    for (int i = 0; i < setCache[set].size(); i++) {
        string currentAddress = setCache[set][i].second.offsetName;
        if (currentAddress == address) {
            return true;
        }
    }
    return false;

}

void Set::HitIncrement(int tag, int set, std::string address) {
    for (int i = 0; i < setCache[set].size(); i++) {
        string currentAddress = setCache[set][i].second.offsetName;

        if (address == currentAddress) {
            // it has been found !
            // need to increment counters if searching here
            setCache[set][i].second.accessCounter++;
            hitRateCounter++;

        }
    }
}

void Set::LRUReplace(int tag, int set, Cache::Address address) {
    int LRUcounter = setCache[set][0].second.accessCounter;
    int index = 0;

    //get smallest hit rate in the program
    for (int i = 0; i < setCache[set].size(); i++) {

        int currentHitRate = setCache[set][i].second.accessCounter;
        if (currentHitRate < LRUcounter) {
            LRUcounter = currentHitRate;
            index = i;
        }

    }
    // then replace the string with the smallest hit rate with the new address
    setCache[set][index].second = address;
}

void Set::FIFOReplace(int tag, int set, Cache::Address address){
    int leastAccessed = setCache[set][0].second.recencyCounter;
    int index = 0;

    //get first element that was added to be replaced
    for (int i = 0; i < setCache[set].size(); i++) {

        int currentHitRate = setCache[set][i].second.recencyCounter;
        if (currentHitRate < leastAccessed) {
            leastAccessed = currentHitRate;
            index = i;
        }
    }
    setCache[set][index].second = address;

}
int Set::SizeOfCache(int tag, int set) {
    int totalSize = 0;

    for(auto iter = setCache.begin(); iter != setCache.end(); ++iter) {
        totalSize = totalSize + setCache[iter->first].size();

   }

    return totalSize;
}