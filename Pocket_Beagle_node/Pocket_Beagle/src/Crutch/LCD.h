/*
 * LCD.h
 *
 *  Created on: 25Feb.,2020
 *      Author: Angus Wallace
 */

#include <string>
#include <LiquidCrystal_I2C.h>
#include <map>
#include <inttypes.h>

class LCD
{
public:
	LCD();
	virtual ~LCD();
	void printStr(std::string str);
	void printStr(std::string str, uint8_t col, uint8_t row);
	void setup();
	void printCurrState();
	void printNextMove();
	void clearCurrState();
	void clearNextMove();
	int getCurrState();
	int getNextMove();
	void setCurrState(int cs);
	void setNextMove(int nm);
	void flash();
	void populateMap();
	std::map<int, std::string> intToStateODMap;

private:
	int currState;
	int nextMove;
	LiquidCrystal_I2C *lcd;
};
