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

    stage = 0;
    index = 1;
    choosingMove = 0;
    feetTogether = 1;

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
    nextMove = 1;
    lcd->commControlOn();
}

void Crutch::run()
{
    currState = CO_OD_RAM.currentState;
    incrementCount();
    updateButtons();
    // crutchTest();
    // If current State is a stationary State
    if (isStationaryState(currState))
    {
        // Cycle forward with debounce
    	if (!choosingMove){
    		choosingMove = 1;
    		updateStageExit();
    	}
        if (nextBut && !prevNextBut)
        {
        	/*
        	index = index % 11 + 1;
            nextMove = stageMap[stage][index];
            */
        	incrementIndex();
            std::cout << "NEXT MOVE:" << lcd->intToMvmntODMap[nextMove] << std::endl;
        }
        if (lastBut && !prevLastBut)
        {
        	/*
            if (index <= 1)
            {
            	index = 11;
                nextMove = stageMap[stage][index];
            }
            else
            {
            	index -= 1;
                nextMove = stageMap[stage][index];
            }
            */
        	decrementIndex();
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
                updateStageEnter();
                choosingMove = 0;
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
    if (isStationaryState(currState))
    {

        if (currState != lastState)
        {
            lcd->setCurrState(this->currState);
            lcd->printCurrState();
            //sleep(1);
            std::cout << "Curr State: " << lcd->intToStateODMap[currState] << std::endl;
            lastState = currState;
        }
        //printf("Test: %d \n", CO_OD_RAM.currentState);

        if (nextMove != lastNextMove)
        {
            lcd->setNextMove(nextMove);
            lcd->printNextMove();
            //sleep(1);
            std::cout << "Next Move: " << lcd->intToMvmntODMap[nextMove] << std::endl;
            lastNextMove = nextMove;
            if (nextMove == 9)
            {
                CO_OD_RAM.currentState = 9;
            }
        }
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
    enterMap[10] = -1;

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
    for (int i=0; i<11; i++){
    	stageMap[0][i+1] = 9;
    }

    //regular
    stageMap[1][1] = 1;
    stageMap[1][2] = 7;
    stageMap[1][3] = 2;
    stageMap[1][4] = 3;
    stageMap[1][5] = 1;//TODO: add tilt option
    stageMap[1][6] = 4;
    stageMap[1][7] = 5;
    stageMap[1][8] = 10;
    stageMap[1][9] = 6;
    stageMap[1][10] = 8;

    //uneven ground
    stageMap[3][1] = 1;
    stageMap[3][2] = 7;
    stageMap[3][3] = 10;
    stageMap[3][4] = 2;
    stageMap[3][5] = 3;
    stageMap[3][6] = 4;
    stageMap[3][7] = 5;
    stageMap[3][8] = 1;//TODO: add tilt option
    stageMap[3][9] = 6;
    stageMap[3][10] = 8;

    //stairs
    stageMap[4][1] = 1;
    stageMap[4][2] = 3;
    stageMap[4][3] = 2;
    stageMap[4][4] = 1; // TODO: add tilt
    stageMap[4][5] = 4;
    stageMap[4][6] = 5;
    stageMap[4][7] = 10;
    stageMap[4][8] = 6;
    stageMap[4][9] = 7;
    stageMap[4][10] = 8;

    //tilt
    stageMap[5][1] = 1;
    stageMap[5][2] = 1; //TODO: add tilt
    stageMap[5][3] = 2;
    stageMap[5][4] = 3;
    stageMap[5][5] = 4;
    stageMap[5][6] = 5;
    stageMap[5][7] = 10;
    stageMap[5][8] = 6;
    stageMap[5][9] = 7;
    stageMap[5][10] = 8;

    //ramp
    stageMap[6][1] = 1;
    stageMap[6][2] = 4;
    stageMap[6][3] = 5;
    stageMap[6][4] = 2;
    stageMap[6][5] = 3;
    stageMap[6][6] = 1; //TODO: add tilt
    stageMap[6][7] = 10;
    stageMap[6][8] = 6;
    stageMap[6][9] = 7;
    stageMap[6][10] = 8;

    //So that the movement selected by the index doesn't change when the stage changes
    indexMap[1] = 1;
    indexMap[3] = 1; //TODO: entrance into stage 3
    indexMap[4] = 3;
    indexMap[5] = 4;
    indexMap[6] = 2;

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

void Crutch::decrementIndex(){
	if (feetTogether){
		if (index <= 1)
		{
			index = 11;
			nextMove = stageMap[stage][index];
		}
		else
		{
			index -= 1;
			nextMove = stageMap[stage][index];
		}
	}
	else if (!feetTogether){
		if (index <= 1){
			index = 11;
			nextMove = stageMap[stage][index];
		}
		else if (4 < index && index < 8){
			index = 4;
		}
		else{
			index -= 1;
		}
	}
}

void Crutch::incrementIndex(){
	if (feetTogether){
		index = index % 11 + 1;
		nextMove = stageMap[stage][index];
	}
	else if (!feetTogether){
		if (index > 4){
			index = 8;
		}
		else if (index > 10){
			index = 1;
		}
		else{
			index += 1;
		}
	}
}

void Crutch::updateIndex(){
	if (indexMap[stage]){
		index = indexMap[stage];
	}
}







