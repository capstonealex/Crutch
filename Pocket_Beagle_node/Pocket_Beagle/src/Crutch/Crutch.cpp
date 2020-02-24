#include "Crutch.h"
#include <iostream>

Crutch::Crutch(/* args */)
{
    std::cout << "Crutch object created" << std::endl;
    populateDictionary();

}

Crutch::~Crutch()
{
    std::cout << "Crutch object deleted" << std::endl;
}

void Crutch::run()
{
    std::cout << "Crutch program running!" << std::endl;
    std::string name = nextMotion[RIGHT_FORWARD][3];
    std::cout << nextMotion[RIGHT_FORWARD][3]<< " : " << intLookupTable[name] << std::endl;
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
void Crutch::populateDictionary(){
    intLookupTable["normal"] = 1;
    intLookupTable["feet together"] = 2;
    intLookupTable["backstep"] = 3;
    intLookupTable["up stairs"] = 4;
    intLookupTable["down stairs"] = 5;
    intLookupTable["up slope"] = 6;
    intLookupTable["down slope"] = 7;
    intLookupTable["uneven"] = 8;
    intLookupTable["Sit Down"] = 9;
    intLookupTable["Stand Up"] = 10;
}
void Crutch::setHeartBeat(int val){

}
void Crutch::setNextMotion(int val){

}
void Crutch::setGreenButon(int val){

}
int Crutch::getCurrentMotion(){

}
int Crutch::getCurrentState(){
    
}