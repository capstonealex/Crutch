#include "Crutch.h"

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>

#include "CANopen.h"

Crutch::Crutch(/* args */) {
    std::cout << "Crutch object created" << std::endl;

    lcd = new LCD();
#ifndef _NOLCD
    lcd->setup();
#endif

    stage = Default;
    index = 1;

#ifdef _KEYBOARD
    kb = new Keyboard();
#endif
}

Crutch::~Crutch() {
    lcd->~LCD();

    std::cout << "Crutch object deleted" << std::endl;
}

void Crutch::initCrutch() {
    lastState = Error;
    currState = Error;

    lastNextMove = RobotMode::SITDWN;  // This is irrelevant - just needed to make sure that it prints to the screen on the first press
    nextMove = RobotMode::NORMALWALK;
#ifndef _NOLCD
    lcd->commControlOn();
#endif
}

void Crutch::run() {
    // Take the current state from the object dictionary - this is passed on from the OD on the Main Beaglebone
    currState = static_cast<SMState>(CO_OD_RAM.currentState);

    // This is a record of the number of iterations of this device - not currently used
    // Could be used to detect a long press though.
    incrementCount();

    // Poll the buttons
    updateButtons();

    // If current State is a stationary State
    if (isStationaryState(currState)) {
        // Cycle forward with debounce
        /*if (!choosingMove){
    		choosingMove = 1;
    	}*/
        if (!nextBut && prevNextBut) {
            incrementIndex();
            nextMove = stageMovementList[stage][index];
        }
        if (!lastBut && prevLastBut) {
            decrementIndex();
            nextMove = stageMovementList[stage][index];
        }
        prevNextBut = nextBut;
        prevLastBut = lastBut;

        // Check if the Go Button has been pressed
        if (goBut) {
            if (nextMove == static_cast<RobotMode>(CO_OD_RAM.currentMovement)) {
                // If the movement set on the ExoBeagle is the same as the one on the screen,
                // Send the go button status on the OD to true
                CO_OD_RAM.goButton = static_cast<uint16_t>(goBut);
            } else {
                // If they are not the same, then send the next movement across to the exoskeleton
                // and do not update the go button on the exo side
                CO_OD_RAM.nextMovement = static_cast<uint16_t>(nextMove);

                //choosingMove = 0;
            }
        } else {
            // If the GoButton is not pressed, set the go button to false always
            CO_OD_RAM.goButton = static_cast<uint16_t>(goBut);
        }
    } else {
        // If not in a stationary state, just map the GoButton to to the ExoBeagle OD
        CO_OD_RAM.goButton = static_cast<uint16_t>(goBut);
    }

#ifndef _NOLCD
    if (!lcd->isQueueEmpty()) {
        lcd->sendNextCommand();
    }
#endif
}

void Crutch::printCSNM() {
    if (isStationaryState(currState)) {
        // Only update the screen if we are in a stationary state
        if (currState != lastState) {
// If the state has changed - update the screen to show the state
#ifndef _NOLCD
            lcd->printCurrState(stateToString[currState]);
#endif
            //sleep(1);
            std::cout << "Curr State: " << stateToString[currState] << std::endl;
            lastState = currState;
        }

        if (nextMove != lastNextMove) {
// If the selected move has changed, update the selected move
#ifndef _NOLCD
            lcd->printNextMove(movementToString[nextMove]);
#endif
            //sleep(1);
            std::cout << "Next Move: " << movementToString[nextMove] << std::endl;
            lastNextMove = nextMove;
        }

        if (lastStage != stage) {
#ifndef _NOLCD
            lcd->printStage(stage);
#endif
            std::cout << "Stage: " << stage << std::endl;
            lastStage = stage;
        }
    }

    // std::string name = nextMotion[RIGHT_FORWARD][3];
    // std::cout << nextMotion[RIGHT_FORWARD][3] << " : " << stateToIntODMap[name] << std::endl;
}
void Crutch::setHeartBeat(int val) {
}
void Crutch::setNextMotion(int val) {
    // CO_OD_RAM.nextMovement = val;
}
void Crutch::setGreenButon(int val) {
    // CO_OD_RAM.goButton = val;
}
int Crutch::getCurrentMotion() {
    // retrun CO_OD_RAM.currentMotion;
    return currState;
}
int Crutch::getCurrentState() {
    return currState;
}
// void Crutch::setCurrentState()
// {
//     currState = CO_OD_RAM.currentState;
// }

// TESTING LCD
void Crutch::setCurrentState(SMState state) {
    // currState = CO_OD_RAM.currentState;
    currState = state;
}
void Crutch::incrementCount() {
    counter++;
}
// TEST BBB OD
void Crutch::testOD() {
    // setCurrentState();
    std::cout << "Current state" << stateToString[currState] << std::endl;
    // TEST CURRENT MOTION
    // std::cout << "Current MOTION" << lcd->intToStateODMap[getCurrentMotion()] << std::endl;
}

/*Cycle through current state from 1- 10 and back again to test printing to screen CSNM*/
/*void Crutch::crutchTest()
{
    if (counter % 5000 == 0)
    {
        stateIndex++;
        if (stateIndex == 11)
        {
            stateIndex = 1;
        }
        setCurrentState(stateIndex);

        index = index % 11 + 1;

    }
}*/

void Crutch::printVector(vector<vector<std::string>> const &mat) {
    for (vector<std::string> row : mat) {
        for (std::string val : row) {
            cout << val << " ";
        }
        cout << '\n';
    }
}

/** This is legacy code to do with changing stages. It is not currently implemented, but I have kept it here because I don't 
 * quite understand how it was meant to work. 
 
    //Stages
    //TODO: stage 3 isnt accessible yet
    enterMap[1] = -1;
    enterMap[2] = 4;
    enterMap[3] = -1;
    enterMap[4] = 6;
    enterMap[5] = -1;
    enterMap[6] = -1;
    enterMap[7] = -1;
    enterMap[8] = -1;
    enterMap[9] = -1;
    enterMap[10] = 3;

    exitMap[1] = -1;
    exitMap[2] = -1;
    exitMap[3] = 5;
    exitMap[4] = -1;
    exitMap[5] = -1;
    exitMap[6] = -1;
    exitMap[7] = -1;
    exitMap[8] = 0;
    exitMap[9] = 1;
    exitMap[10] = -1;

    //sitting
    for (int i=1; i<11; i++){
    	stageMap[0][i+1] = 9;
    }

    //So that the movement selected by the index doesn't change when the stage changes
    indexMap[0] = 1;
    indexMap[1] = 1;
    indexMap[3] = 3;
    indexMap[4] = 3;
    indexMap[5] = 4;
    indexMap[6] = 2;
**/

bool Crutch::isStationaryState(SMState state) {
    return stateStationaryStatus[state];
}

void Crutch::updateButtons() {
#ifdef _KEYBOARD
    kb->updateInput();
    nextBut = kb->getA();
    lastBut = kb->getS();
    if (kb->getD()) {
        goBut = !goBut;
    }
#else
    nextBut = checkButton(nextButPath);
    lastBut = checkButton(lastButPath);
    goBut = checkButton(goButPath);
    // std::cout << "next: " << nextBut << " last: " << lastBut << " go: " << goBut << std::endl;  // testing button vals
#endif
}

bool Crutch::checkButton(std::string path) {
    char value;
    std::ifstream stream(path);
    stream >> value;
    stream.close();

    if (value == '1') {
        return true;
    } else {
        return false;
    }
}
/*
void Crutch::updateStageEnter(){
	if (enterMap[nextMove] != -1){
		stage = enterMap[nextMove];
	}
	if (nextMove == 6){
		feetTogether = 1;
	}
	updateIndex();
}

void Crutch::updateStageExit(){
	if (exitMap[nextMove] != -1){
		stage = exitMap[nextMove];
	}
	if (nextMove == 6){
		feetTogether = 0;
	}
	updateIndex();
}
*/
void Crutch::decrementIndex() {
    index = (index < 1) ? index = stageMovementList[stage].size() - 1 : index - 1;

    // If sitting, only option is to stand up, search for that entry in the list
    while (currState == SMState::Sitting && stageMovementList[stage][index] != RobotMode::STNDUP) {
        index = (index < 1) ? index = stageMovementList[stage].size() - 1 : index - 1;
    }

    // Only allow stand up in the sitting state
    while (currState != SMState::Sitting && stageMovementList[stage][index] == RobotMode::STNDUP) {
        index = (index < 1) ? index = stageMovementList[stage].size() - 1 : index - 1;
    }

    // To only allow sitting when feet are together
    while (stageMovementList[stage][index] == RobotMode::SITDWN && currState != SMState::Standing) {
        index = (index < 1) ? index = stageMovementList[stage].size() - 1 : index - 1;
    }

    // To prevent a Feet Together Movement when feet are already together
    while (currState == SMState::Standing && stageMovementList[stage][index] == RobotMode::FTTG) {
        index = (index < 1) ? index = stageMovementList[stage].size() - 1 : index - 1;
        ;
    }
}

void Crutch::incrementIndex() {
    index = (index + 1) % stageMovementList[stage].size();
    // If sitting, only option is to stand up, search for that entry in the list
    while (currState == SMState::Sitting && stageMovementList[stage][index] != RobotMode::STNDUP) {
        index = (index + 1) % stageMovementList[stage].size();
    }

    // Only allow stand up in the sitting state
    while (currState != SMState::Sitting && stageMovementList[stage][index] == RobotMode::STNDUP) {
        index = (index + 1) % stageMovementList[stage].size();
    }

    // To only allow sitting when feet are together
    while (stageMovementList[stage][index] == RobotMode::SITDWN && currState != SMState::Standing) {
        index = (index + 1) % stageMovementList[stage].size();
    }

    // To prevent a Feet Together Movement when feet are already together
    while (currState == SMState::Standing && stageMovementList[stage][index] == RobotMode::FTTG && stageMovementList[stage][index] == RobotMode::STNDUP) {
        index = (index + 1) % stageMovementList[stage].size();
    }
}
