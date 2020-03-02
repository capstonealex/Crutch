#include "Crutch.h"
#include <iostream>
#include <unistd.h>

Crutch::Crutch(/* args */)
{
    std::cout << "Crutch object created" << std::endl;

    lcd = new LCD();
    lcd->setup();
    populateDictionary();
}

Crutch::~Crutch()
{
    lcd->~LCD();
    std::cout << "Crutch object deleted" << std::endl;
}

void Crutch::run()
{
    incrementCount();
    crutchTest();
}

void Crutch::printCSNM()
{
    lcd->setCurrState(this->currState);
    lcd->printCurrState();
    lcd->setNextMove(nextMove);
    lcd->printNextMove();

    std::string name = nextMotion[RIGHT_FORWARD][3];
    std::cout << nextMotion[RIGHT_FORWARD][3] << " : " << intLookupTable[name] << std::endl;
}
void Crutch::setHeartBeat(int val)
{
}
void Crutch::setNextMotion(int val)
{
}
void Crutch::setGreenButon(int val)
{
}
int Crutch::getCurrentMotion()
{
    return currState;
}
int Crutch::getCurrentState()
{
    return nextMove;
}
void Crutch::setCurrentMotion(int motion)
{
    currState = motion;
}
void Crutch::setCurrentState(int state)
{
    nextMove = state;
}
void Crutch::incrementCount()
{
    counter++;
}
/*Cycle through current state from 1- 10 and back again for print CSNM*/
void Crutch::crutchTest()
{
    if (counter % 50 == 0)
    {
        stateIndex++;
        if (stateIndex == 11)
        {
            stateIndex = 1;
        }
        setCurrentState(stateIndex);
    }
}

void Crutch::printVector(vector<vector<string>> const &mat)
{
    for (vector<string> row : mat)
    {
        for (string val : row)
        {
            cout << val << " ";
        }
        cout << '\n';
    }
}
void Crutch::populateDictionary()
{
    intLookupTable["normal"] = 1;
    intLookupTable["backstep"] = 2;
    intLookupTable["feet together"] = 3;
    intLookupTable["up stairs"] = 4;
    intLookupTable["down stairs"] = 5;
    intLookupTable["up slope"] = 6;
    intLookupTable["down slope"] = 7;
    intLookupTable["uneven"] = 8;
    intLookupTable["Sit Down"] = 9;
    intLookupTable["Stand Up"] = 10;

    // stateLookupTable[1] = "normal";
    // stateLookupTable[2] = "backstep";
    // stateLookupTable[3] = "feet together";
    // stateLookupTable[4] = "up stairs";
    // stateLookupTable[5] = "down stairs";
    // stateLookupTable[6] = "up slope";
    // stateLookupTable[7] = "down slope";
    // stateLookupTable[8] = "uneven";
    // stateLookupTable[9] = "Sit Down";
    // stateLookupTable[10] = "Stand Up";

    std::cout << "Dictionary populated" << std::endl;
}