#include <iostream>
#include <unordered_map>
#include <vector>
#include <map>
using namespace std;

class Cache {
public:
    struct Address {
        string offsetName;
        //used for LRU; will increment every time there is a hit to determine "most used"
        int accessCounter;
        // used for FIFO; will give the number val of address being added
        int recencyCounter;

        Address(string offsetName, int accessCounter, int recencyCounter){
            this->offsetName = offsetName;
            this->accessCounter = accessCounter;
            this->recencyCounter = recencyCounter;
        }
    };
    float totalAccesses = 0;
    float hitRateCounter = 0;

    // calc number of lines in the cache
    int NumberOfLines(int cacheSize, int lineSize);
    //increments the total accesses
    void AccessIncrement();
    // calcs tag size, then removes it from the string
    int CalcTag(string address, int tagSize);

};
class Full : public Cache {
    // where int is the tag and address is Address obj that stores the counters and the offset
    unordered_map<int , vector<Address>> fullCache;

public:
    //check, add, hit rate
    bool CheckAddress(int tag, string address);
    void AddFullItem(int tag, Address offset);
    void HitIncrement(int tag, string address);

    // consider the replacement strategies to replace a repeat; remove item with lowest counter (access or hit rate)
    void LRUReplace(int tag, Address newAddress);
    void FIFOReplace(int tag, Address newAddress);
    int SizeOfCache(int tag);

};
class Direct : public Cache {
    // first int is tag, second is line, vector stores all possible addresses
    // multidimensional map time
    multimap<int, pair<int, Address>> directCache;
    //multimap<int, int> directCache;

public:
    //check, add, hit rate, line; no replacement :)
    bool CheckAddress(int tag, int line, string address);
    void AddDirectItem(int tag, int line, Address offset);
    void HitIncrement(int tag, int line, string offset);
    int CalcLine(int tagSize, int lineSize, string address);
    // replacement
    void ReplaceAddress(int tag, int line, Address address);
    int SizeOfCache(int tag);

};
class Set : public Cache {
    // first int is tag, second is set, vector stores all possible addresses

    //unordered_map<int, vector<pair<int, Address>>> setCache;
    // first int is set, then tag and address
    unordered_map<int, vector<pair<int, Address>>> setCache;

public:
    //add, check, hit, calc set
    void AddSetItem(int tag, int set, Address offset);
    bool CheckAddress(int tag, int set, string address);
    void HitIncrement(int tag, int set, string address);
    int CalcSet(int tagSize, int setSize, string address);

    // consider the replacement strategies to replace a repeat; remove item with lowest counter (access or hit rate)
    void LRUReplace(int tag, int set, Address address);
    void FIFOReplace(int tag, int set, Address address);
    int SizeOfCache(int tag, int set);

};