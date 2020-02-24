#include "Crutch.h"
#include <iostream>

Crutch::Crutch(/* args */)
{
    std::cout << "Crutch object created" << std::endl;
}

Crutch::~Crutch()
{
    std::cout << "Crutch object deleted" << std::endl;
}

void Crutch::run()
{
    std::cout << "Crutch program running!" << std::endl;
}