#include "Crutch.h"
#include <iostream>
#include <unistd.h>

Crutch::Crutch(/* args */)
{
    std::cout << "Crutch object created" << std::endl;
    lcd = new LCD();
    lcd->setup();
}

Crutch::~Crutch()
{
	lcd->~LCD();
    std::cout << "Crutch object deleted" << std::endl;
}

void Crutch::run()
{
    std::cout << "Crutch program running!" << std::endl;
}

void Crutch::printCSNM()
{
	lcd->setCurrState(currState);
	lcd->printCurrState();
	lcd->setNextMove(nextMove);
	lcd->printNextMove();
}

















