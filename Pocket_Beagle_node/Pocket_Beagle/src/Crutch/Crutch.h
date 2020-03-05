/*
* Crutch Object running on pocket Beagle
*
*
*/
// VECTOR INDEX FOR nextMOTION lists
#define LEFT_FORWARD 0
#define RIGHT_FORWARD 1
#define STANDING 2
#define SITTING 3

#include "LCD.h"
#include <string>
#include <vector>
#include <map>
using namespace std;
class Crutch
{
private:
    /* Jagged array for Current state, next motion relationship */
    // walking, standing and sitting w/ their Next motion lists
    vector<vector<string>> nextMotion{
        {"normal", "feet together", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
        {"sit Down", "normal", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
        {"Stand Up"}};
    /* data */
    int currState;
    int lastState;
    int nextMove;
    int lastNextMove;

    // Button Variables
    int nextBut;
    int prevNextBut;
    int lastBut;
    int prevLastBut;
    int goBut;

    char nextButPath[50] = "/sys/class/gpio/gpio59/value";
    char lastButPath[50] = "/sys/class/gpio/gpio58/value";
    char goButPath[50] = "/sys/class/gpio/gpio57/value";

    void updateButtons();
    int checkButton(char path[]);
    int isStationaryState(int state);

public:
    Crutch(/* args */);
    ~Crutch();
    void run();
    void printCSNM();
    LCD *lcd;
    void initCrutch();
    void printVector(vector<vector<string>> const &mat);
    /*Look Up table to convert between nextMotion selections and OD int outputs to exo BBB*/
    std::map<std::string, int> stateToIntODMap;
    std::map<int, std::string> intToStateODMap;
    void populateDictionary();
    //OD interfaces
    void setHeartBeat(int val);
    void setNextMotion(int val);
    void setGreenButon(int val);
    int getCurrentMotion();
    int getCurrentState();
    //For Testing w.o. object Dicitonary
    void crutchTest();
    void setCurrentState(int state);
    // void setCurrentState();
    void incrementCount();
    int counter;
    int stateIndex;
    void testOD();
};
