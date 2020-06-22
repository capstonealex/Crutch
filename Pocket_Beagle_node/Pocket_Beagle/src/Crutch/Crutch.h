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

#include <array>
#include <map>
#include <string>
#include <vector>

#include "LCD.h"

using namespace std;
class Crutch {
   private:
    /* Jagged array for Current state, next motion relationship */
    // walking, standing and sitting w/ their Next motion lists
    // vector<vector<std::string>> nextMotion{
    //     {"normal", "feet together", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
    //     {"sit Down", "normal", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
    //     {"Stand Up"}};
    /* data */
    int currState;
    int lastState;
    int nextMove;
    int lastNextMove;
    int stage;
    int index;
    int choosingMove;
    int feetTogether;

    // Button Variables
    int nextBut;
    int prevNextBut;
    int lastBut;
    int prevLastBut;
    int goBut;

    std::string nextButPath = "/sys/class/gpio/gpio59/value";
    std::string lastButPath = "/sys/class/gpio/gpio58/value";
    std::string goButPath = "/sys/class/gpio/gpio57/value";

    std::map<int, int> enterMap;
    std::map<int, int> exitMap;
    std::map<int, std::array<int, 11>> stageMap;
    std::map<int, int> indexMap;

    void updateButtons();
    int checkButton(std::string path);
    int isStationaryState(int state);
    void updateStageEnter();
    void updateStageExit();
    void updateIndex();
    void decrementIndex();
    void incrementIndex();

   public:
    Crutch(/* args */);
    ~Crutch();
    void run();
    void printCSNM();
    LCD *lcd;
    void initCrutch();
    void printVector(vector<vector<std::string>> const &mat);
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
