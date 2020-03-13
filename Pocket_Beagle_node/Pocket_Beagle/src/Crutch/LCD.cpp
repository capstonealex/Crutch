/*
 * LCD.cpp
 *
 *  Created on: 25Feb.,2020
 *      Author: Angus Wallace
 */

#include "LCD.h"
#include <LiquidCrystal_I2C.h>
#include <map>
#include <inttypes.h>
#include <iostream>
#include <unistd.h>

#define LCD_ADDR 0x20
#define LCD_COLS 16
#define LCD_ROWS 2
#define I2C_BUS 1 // NB: Bus 2 is on port 1 on PB
#define PIN1 9	//NB: on port 1 (i2c bus 2) of PB
#define PIN2 11   //NB: on port 1 (i2c bus 2) of PB

LCD::LCD()
{
	lcd = new LiquidCrystal_I2C(LCD_ADDR, LCD_COLS, LCD_ROWS, I2C_BUS);
	lcd->configure_i2c_pins(PIN1, PIN2);
	lcd->init();
	populateMap();
	std::cout << "LCD object created" << std::endl;
}

LCD::~LCD()
{
	lcd->commControlOff();
	lcd->clear();
	lcd->noCursor();
	lcd->noBacklight();
	lcd->noDisplay();
	lcd->i2c_close(lcd->getHandle());
	delete lcd;
	std::cout << "LCD object deleted" << std::endl;
}

void LCD::printStr(std::string str)
{
	lcd->writeStr(str);
}

void LCD::printStr(std::string str, uint8_t col, uint8_t row)
{
	lcd->cursor();
	lcd->setCursor(col, row);
	printStr(str);
	lcd->noCursor();
}

void LCD::setup()
{
	lcd->backlight();
	printStr("CS:", 0, 0);
	sleep(1);
	printStr("NM:", 0, 1);
	sleep(1);

	lcd->commControlOn();
}

void LCD::printCurrState()
{
	clearCurrState();
	printStr(intToStateODMap[currState], 3, 0);
}

void LCD::printNextMove()
{
	std::cout << "LCD LEVEL NEXT MOVE:" << intToMvmntODMap[nextMove] << std::endl;
	clearNextMove();
	printStr(intToMvmntODMap[nextMove], 3, 1);
}

void LCD::clearCurrState()
{
	lcd->cursor();
	lcd->setCursor(3, 0);
	for (auto i = 0; i < LCD_COLS; i++)
	{
		lcd->writeI2C(' ');
	}
	lcd->noCursor();
}

void LCD::clearNextMove()
{
	lcd->cursor();
	lcd->setCursor(3, 1);
	for (auto i = 0; i < LCD_COLS; i++)
	{
		lcd->writeI2C(' ');
	}
	lcd->noCursor();
}

int LCD::getCurrState()
{
	return currState;
}

int LCD::getNextMove()
{
	return nextMove;
}

void LCD::setCurrState(int cs)
{
	currState = cs;
}

void LCD::setNextMove(int nm)
{
	nextMove = nm;
}

void LCD::flash()
{
	lcd->noBacklight();
	sleep(1);
	lcd->backlight();
}

void LCD::commControlOn()
{
	lcd->commControlOn();
}

void LCD::commControlOff()
{
	lcd->commControlOff();
}

int LCD::sendNextCommand()
{
	return lcd->sendNextCommand();
}

int LCD::isQueueEmpty()
{
	return lcd->isQueueEmpty();
}

void LCD::populateMap()
{

	intToMvmntODMap[1] = "Normal";
	intToMvmntODMap[2] = "Up stair";
	intToMvmntODMap[3] = "Dwm stair";
	intToMvmntODMap[4] = "Up slope";
	intToMvmntODMap[5] = "Dwn slope";
	intToMvmntODMap[6] = "Ft TG";
	intToMvmntODMap[7] = "Backstep";
	intToMvmntODMap[8] = "Sit Dwn";
	intToMvmntODMap[9] = "Stand Up";
	intToMvmntODMap[10] = "Uneven";

	intToStateODMap[1] = "Error";
	intToStateODMap[2] = "Init";
	intToStateODMap[3] = "Left Frwd";
	intToStateODMap[4] = "Right Frwd";
	intToStateODMap[5] = "Standing";
	intToStateODMap[6] = "Sitting";
	intToStateODMap[7] = "Sit dWn";
	intToStateODMap[8] = "Stand Up";
	intToStateODMap[9] = "1st L";
	intToStateODMap[10] = "1st R";
	intToStateODMap[11] = "last L";
	intToStateODMap[12] = "last R";
	intToStateODMap[13] = "Step L";
	intToStateODMap[14] = "Step R";
}
