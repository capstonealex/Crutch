/*
* Cructch Object running on pocket Beagle
*
*
*/
// VECTOR INDEX FOR nextMOTION lists
#define LEFT_FORWARD 0
#define RIGHT_FORWARD 1
#define STANDING 2
#define SITTING 3

#include <string>
#include <vector>
#include <map>
using namespace std;
class Crutch
{
private:
    /* data */
    vector<vector<string>> nextMotion{
        {"normal", "feet together", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
        {"normal", "feet together", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
        {"Sit Down", "normal", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
        {"Stand Up"}};

public:
    Crutch(/* args */);
    ~Crutch();
    void run();
    void initCrutch();
    void printVector(vector<vector<string>> const &mat);
    /*Look Up table to convert between nextMotion selections and OD int outputs to exo BBB*/
    std::map<std::string, int> intLookupTable;
    void populate();
};
