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


enum class RobotMode {
    NORMALWALK,
    SITDWN,
    STNDUP,
    UPSTAIR,
    DWNSTAIR,
    TILTUP,
    TILTDWN,
    BKSTEP,
    FTTG,
    UNEVEN,
    INITIAL,
    ERROR,
};


// Incompatible state:Movement Pairs:
// Sitting -> can only stand
// Feet Together -> only when not in standing

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
static std::map<RobotMode, std::string> movementToString = {
    {RobotMode::NORMALWALK, "Normal"},
    {RobotMode::BKSTEP, "Backstep"},
    {RobotMode::FTTG, "Feet Together"}, 
    {RobotMode::UPSTAIR, "Up stairs"}, 
    {RobotMode::DWNSTAIR, "Down stairs"},
    {RobotMode::TILTUP, "Up slope"},
    {RobotMode::TILTDWN, "Down slope"},
    {RobotMode::UNEVEN, "Uneven"},
    {RobotMode::SITDWN, "Sit Down"}, 
    {RobotMode::STNDUP, "Stand Up"}
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


// Note: Every stage MUST have RobotMode::STNDUP in it
static std::map<Stage, std::vector<RobotMode>> stageMovementList = {
    {Default, {RobotMode::NORMALWALK, RobotMode::TILTDWN, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::NORMALWALK, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::STNDUP, RobotMode::TILTUP, RobotMode::UNEVEN, RobotMode::SITDWN}},
    {UnevenGnd, {RobotMode::NORMALWALK, RobotMode::TILTDWN, RobotMode::STNDUP, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::NORMALWALK, RobotMode::TILTUP, RobotMode::UNEVEN, RobotMode::SITDWN}},
    {Stairs, {RobotMode::NORMALWALK, RobotMode::FTTG, RobotMode::BKSTEP, RobotMode::NORMALWALK, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::STNDUP, RobotMode::TILTUP, RobotMode::TILTDWN, RobotMode::UNEVEN, RobotMode::SITDWN}},
    {Tilt, {RobotMode::NORMALWALK, RobotMode::NORMALWALK, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::UPSTAIR, RobotMode::DWNSTAIR, RobotMode::STNDUP, RobotMode::TILTUP, RobotMode::TILTDWN, RobotMode::UNEVEN, RobotMode::SITDWN}},
    {Ramp, {RobotMode::NORMALWALK, RobotMode::DWNSTAIR, RobotMode::BKSTEP, RobotMode::FTTG, RobotMode::STNDUP, RobotMode::TILTUP, RobotMode::TILTDWN, RobotMode::UNEVEN, RobotMode::SITDWN}}
};


class Crutch {

   private:
    /* Jagged array for Current state, next motion relationship */
    // walking, standing and sitting w/ their Next motion lists
    // vector<vector<std::string>> nextMotion{
    //     {"RobotMode::NORMALWALK", "feet together", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
    //     {"sit Down", "RobotMode::NORMALWALK", "backstep", "up stairs", "down stairs", "up slope", "down slope", "uneven"},
    //     {"Stand Up"}};
    /* data */
    SMState currState;
    SMState lastState;

    RobotMode nextMove;
    RobotMode lastNextMove;

    Stage stage;
    Stage lastStage;

    int index;
    
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