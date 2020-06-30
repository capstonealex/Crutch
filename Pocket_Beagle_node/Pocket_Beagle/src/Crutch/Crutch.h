/*
* Crutch Object running on pocket Beagle
*
*
*/
// VECTOR INDEX FOR nextMOTION lists

#ifndef CRUTCH_H_DEFINED
#define CRUTCH_H_DEFINED

#define LEFT_FORWARD 0
#define RIGHT_FORWARD 1
#define STANDING 2
#define SITTING 3

#define _NOLCD
#define _KEYBOARD

#include <array>
#include <map>
#include <string>
#include <vector>

#include "LCD.h"

#ifdef _KEYBOARD
    #include "Keyboard.h"
#endif


using namespace std;


enum MovementType{Normal, Backstep, FeetTogether, UpStairs, DownStairs, UpSlope, DownSlope, Uneven, SitDown, StandUp }; 
enum SMState{Error, Init, LeftForward, RightForward, Standing, Sitting, SittingDown, StandingUp, StepFirstL, StepFirstR, StepLastL, StepLastR, StepL, StepR}; 
enum Stage{Default, UnevenGnd, Stairs, Tilt, Ramp};


static std::map <SMState, bool> stateStationaryStatus = {
    {Error, true},
    {Init, true},
    {LeftForward, true},
    {RightForward, true},
    {Standing, true}, 
    {Sitting, true},
    {SittingDown, false},
    {StandingUp, false},
    {StepFirstL, false},
    {StepFirstR, false},
    {StepLastL, false},
    {StepLastR, false},
    {StepL, false}, 
    {StepR, false}
};


/*Look Up table to convert between nextMotion selections and OD int outputs to exo BBB*/
static std::map<MovementType, std::string> movementToString = {
    {Normal, "Normal"},
    {Backstep, "Backstep"},
    {FeetTogether, "Feet Together"}, 
    {UpStairs, "Up stairs"}, 
    {DownStairs, "Down stairs"},
    {UpSlope, "Up slope"},
    {DownSlope, "Down slope"},
    {Uneven, "Uneven"},
    {SitDown, "Sit Down"}, 
    {StandUp, "Stand Up"}
};

static std::map<SMState, std::string> stateToString = {
    {Error, "Error"},
    {Init, "Init"},
    {LeftForward, "Left Forward"},
    {RightForward, "Right Forward"}, 
    {Standing, "Standing"}, 
    {Sitting, "Sitting"},
    {SittingDown, "Sitting Down"},
    {StandingUp, "Standing Up"},
    {StepFirstL, "Step 1st Left"},
    {StepFirstR, "Step 1st Right"},
    {StepLastL, "Step Last L"},
    {StepLastR, "Step Last R"},
    {StepL, "Step Left"},
    {StepR, "Step Right"}
};

static std::map<Stage, std::vector<MovementType>> stageMovementList = {
    {Default, {Normal, DownSlope, Backstep, FeetTogether, Normal, UpStairs, DownStairs, StandUp, UpSlope, Uneven}},
    {UnevenGnd, {Normal, DownSlope, StandUp, Backstep, FeetTogether, UpStairs, DownStairs, Normal, UpSlope, Uneven}},
    {Stairs, {Normal, FeetTogether, Backstep, Normal, UpStairs, DownStairs, StandUp, UpSlope, DownSlope, Uneven}},
    {Tilt, {Normal, Normal, Backstep, FeetTogether, UpStairs, DownStairs, StandUp, UpSlope, DownSlope, Uneven}},
    {Ramp, {Normal, Uneven, DownStairs, Backstep, FeetTogether, Normal, StandUp, UpSlope, DownSlope, Uneven}}
};


class Crutch {

   private:
    /* Jagged array for Current state, next motion relationship */
    // walking, standing and sitting w/ their Next motion lists
    // vector<vector<std::string>> nextMotion{
    //     {"normal", "feet together", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
    //     {"sit Down", "normal", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
    //     {"Stand Up"}};
    /* data */
    SMState currState;
    SMState lastState;

    MovementType nextMove;
    MovementType lastNextMove;

    Stage stage;
    Stage lastStage;

    int index;
    int choosingMove;
    int feetTogether;

    // Button Variables
    bool nextBut;
    bool prevNextBut; // for debounce
    bool lastBut;
    bool prevLastBut; // for debounce
    bool goBut;

    std::string nextButPath = "/sys/class/gpio/gpio59/value";
    std::string lastButPath = "/sys/class/gpio/gpio58/value";
    std::string goButPath = "/sys/class/gpio/gpio57/value";

    std::map<int, int> enterMap;
    std::map<int, int> exitMap;
    std::map<int, std::array<int, 11>> stageMap;
    std::map<int, int> indexMap;


    void updateButtons();
    bool checkButton(std::string path);
    bool isStationaryState(SMState state);

    void updateStageEnter();
    void updateStageExit();

    void decrementIndex();
    void incrementIndex();

    #ifdef _KEYBOARD
        Keyboard *kb;
    #endif

   public:
    Crutch(/* args */);
    ~Crutch();
    void run();
    void printCSNM();
    LCD *lcd;
    void initCrutch();
    void printVector(vector<vector<std::string>> const &mat);

    void populateDictionary();
    //OD interfaces
    void setHeartBeat(int val);
    void setNextMotion(int val);
    void setGreenButon(int val);
    int getCurrentMotion();
    int getCurrentState();
    //For Testing w.o. object Dicitonary
    void crutchTest();
    void setCurrentState(SMState state);
    // void setCurrentState();
    void incrementCount();
    int counter;
    int stateIndex;
    void testOD();
};

#endif