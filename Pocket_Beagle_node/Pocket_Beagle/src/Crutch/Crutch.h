/*
* Crutch Object running on pocket Beagle
*
*
*/

#include "LCD.h"

class Crutch
{
private:
    /* data */
	int currState;
	int nextMove;
public:
    Crutch(/* args */);
    ~Crutch();
    void run();
    void printCSNM();
    LCD *lcd;
};
