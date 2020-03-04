#include "Crutch.h"
#include <iostream>
#include <unistd.h>
#include "CANopen.h"

#include <fcntl.h>
#include <sys/epoll.h>
#include <fstream>
#include <algorithm>


Crutch::Crutch(/* args */)
{
    std::cout << "Crutch object created" << std::endl;

    //lcd = new LCD();
    //lcd->setup();
    populateDictionary();
}

Crutch::~Crutch()
{
    //lcd->~LCD();
    std::cout << "Crutch object deleted" << std::endl;
}

void Crutch::run()
{
    currState = CO_OD_RAM.currentState;
    
    incrementCount();
    updateButtons();
    //crutchTest();
    // If current State is a stationary State
    if (isStationaryState(currState))
    {
        //printf("true \n");
        // Cycle forward with debounce
        if (nextBut && !prevNextBut)
        {
            nextMove = nextMove + 1;
        }
        if (lastBut && !prevLastBut)
        {
            nextMove = nextMove - 1;
        }
        prevNextBut = nextBut;
        prevLastBut = lastBut;
        
        // Check if the Go Button has been pressed
        if (goBut)
        {
            if(nextMove == CO_OD_RAM.currentMovement)
            {
                CO_OD_RAM.goButton = goBut;
            } else
            {
                CO_OD_RAM.nextMovement = nextMove;
            }
        }
    } else{
        // Just step through movement
        CO_OD_RAM.goButton = goBut;
    }
}

void Crutch::printCSNM()
{
    
    if (currState != lastState)
    {
        //lcd->setCurrState(this->currState);
        //lcd->printCurrState();
        printf("Curr State: %d\n", currState);
        lastState = currState;
    }
    //printf("Test: %d \n", CO_OD_RAM.currentState);
    
    if (nextMove != lastNextMove){
        //lcd->setNextMove(nextMove);
        //lcd->printNextMove();
        printf("Next Move: %d\n", nextMove);
        lastNextMove = nextMove;
    }
 

    // std::string name = nextMotion[RIGHT_FORWARD][3];
    // std::cout << nextMotion[RIGHT_FORWARD][3] << " : " << stateToIntODMap[name] << std::endl;
}
void Crutch::setHeartBeat(int val)
{
}
void Crutch::setNextMotion(int val)
{
    // CO_OD_RAM.nextMovement = val;
}
void Crutch::setGreenButon(int val)
{
    // CO_OD_RAM.goButton = val;
}
int Crutch::getCurrentMotion()
{
    // retrun CO_OD_RAM.currentMotion;
    return currState;
}
int Crutch::getCurrentState()
{
    return currState;
}
// void Crutch::setCurrentState()
// {
//     currState = CO_OD_RAM.currentState;
// }
// TESTING LCD
void Crutch::setCurrentState(int state)
{
    // currState = CO_OD_RAM.currentState;
    currState = state;
}
void Crutch::incrementCount()
{
    counter++;
}
// TEST BBB OD
void Crutch::testOD()
{
    // setCurrentState();
    std::cout << "Current state" << lcd->intToStateODMap[getCurrentState()] << std::endl;
    // TEST CURRENT MOTION
    // std::cout << "Current MOTION" << lcd->intToStateODMap[getCurrentMotion()] << std::endl;
}

/*Cycle through current state from 1- 10 and back again to test printing to screen CSNM*/
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
    stateToIntODMap["normal"] = 1;
    stateToIntODMap["backstep"] = 2;
    stateToIntODMap["feet together"] = 3;
    stateToIntODMap["up stairs"] = 4;
    stateToIntODMap["down stairs"] = 5;
    stateToIntODMap["up slope"] = 6;
    stateToIntODMap["down slope"] = 7;
    stateToIntODMap["uneven"] = 8;
    stateToIntODMap["Sit Down"] = 9;
    stateToIntODMap["Stand Up"] = 10;
    stateToIntODMap["Error"] = 11;

    intToStateODMap[1] = "normal";
    intToStateODMap[2] = "backstep";
    intToStateODMap[3] = "feet together";
    intToStateODMap[4] = "up stairs";
    intToStateODMap[5] = "down stairs";
    intToStateODMap[6] = "up slope";
    intToStateODMap[7] = "down slope";
    intToStateODMap[8] = "uneven";
    intToStateODMap[9] = "Sit Down";
    intToStateODMap[10] = "Stand Up";
    intToStateODMap[11] = "Error";

    std::cout << "Dictionary populated" << std::endl;
}

int Crutch::isStationaryState(int state){
    // TODO : Needs to have proper definition of states....
    return 1;
}


void Crutch::updateButtons()
{
    nextBut = checkButton(nextButPath);
    lastBut = checkButton(lastButPath);    
    goBut = checkButton(goButPath);
}


int Crutch::checkButton(char path[])
{
    char value;
    std::ifstream stream(path);
    stream >> value;
    stream.close();
  
    if(value != '1')
    {
        //printf("%s", path);
        return 1;
        //CO_OD_RAM.nextMovement = CO_OD_RAM.nextMovement+1;
    } else
    {
        return 0;
    }
}

