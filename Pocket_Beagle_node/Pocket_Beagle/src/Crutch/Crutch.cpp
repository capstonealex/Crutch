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

    lcd = new LCD();
    lcd->setup();
    populateDictionary();
}

Crutch::~Crutch()
{
    lcd->~LCD();
    std::cout << "Crutch object deleted" << std::endl;
}

void Crutch::initCrutch()
{
    //lcd = new LCD();
    //lcd->setup();
    lastState = 50;
    lcd->commControlOn();
}

void Crutch::run()
{
    currState = CO_OD_RAM.currentState;

    incrementCount();
    //updateButtons();
    crutchTest();
    // If current State is a stationary State
    if (isStationaryState(currState))
    {
        //printf("true \n");
        // Cycle forward with debounce
        if (nextBut && !prevNextBut)
        {
            nextMove = nextMove % 11 + 1;
        }
        if (lastBut && !prevLastBut)
        {
            if (nextMove <= 1)
            {
                nextMove = 11;
            }
            else
            {
                nextMove = nextMove - 1;
            }
        }
        prevNextBut = nextBut;
        prevLastBut = lastBut;

        // Check if the Go Button has been pressed
        if (goBut)
        {
            if (nextMove == CO_OD_RAM.currentMovement)
            {
                CO_OD_RAM.goButton = goBut;
            }
            else
            {
                CO_OD_RAM.nextMovement = nextMove;
            }
        }
        else
        {
            CO_OD_RAM.goButton = goBut;
        }
    }
    else
    {
        // Just step through movement
        CO_OD_RAM.goButton = goBut;
    }

    if (!lcd->isQueueEmpty())
    {
    	lcd->sendNextCommand();
    }
}

void Crutch::printCSNM()
{

    if (currState != lastState)
    {
        lcd->setCurrState(this->currState);
        lcd->printCurrState();
        std::cout << "Curr State: " << lcd->intToStateODMap[currState] << std::endl;
        lastState = currState;
    }
    //printf("Test: %d \n", CO_OD_RAM.currentState);

    if (nextMove != lastNextMove)
    {
        lcd->setNextMove(nextMove);
        lcd->printNextMove();
        std::cout << "Next Move: " << lcd->intToMvmntODMap[nextMove] << std::endl;
        lastNextMove = nextMove;

        // lcd->setCurrState(this->currState);
        // lcd->printCurrState();
        // std::cout << "Curr State: " << lcd->intToStateODMap[currState] << std::endl;
        //lastState = currState;
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

void Crutch::printVector(vector<vector<std::string>> const &mat)
{
    for (vector<std::string> row : mat)
    {
        for (std::string val : row)
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

    intToStateODMap[1] = "Error";
    intToStateODMap[2] = "Init";
    intToStateODMap[3] = "Left Forward";
    intToStateODMap[4] = "Right Forward";
    intToStateODMap[5] = "Standing";
    intToStateODMap[6] = "Sitting";
    intToStateODMap[7] = "Sitting Down";
    intToStateODMap[8] = "Standing Up";
    intToStateODMap[9] = "Step 1st L";
    intToStateODMap[10] = "Step 1st R";
    intToStateODMap[11] = "Step last L";
    intToStateODMap[12] = "Step last R";
    intToStateODMap[13] = "Step L";
    intToStateODMap[14] = "Step R";

    std::cout << "Dictionary populated" << std::endl;
}

int Crutch::isStationaryState(int state)
{
    if (state >= 7)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void Crutch::updateButtons()
{
    nextBut = checkButton(nextButPath);
    lastBut = checkButton(lastButPath);
    goBut = checkButton(goButPath);
}

int Crutch::checkButton(std::string path)
{
    char value;
    std::ifstream stream(path);
    stream >> value;
    stream.close();

    if (value != '1')
    {
        //printf("%s", path);
        return 1;
        //CO_OD_RAM.nextMovement = CO_OD_RAM.nextMovement+1;
    }
    else
    {
        return 0;
    }
}
