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
#define I2C_BUS 2 // NB: Bus 2 is on port 1 on PB
#define PIN1 26   //NB: on port 1 (i2c bus 2) of PB
#define PIN2 28   //NB: on port 1 (i2c bus 2) of PB

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
	lcd->clear();
	lcd->noCursor();
	lcd->noBacklight();
	lcd->noBlink();
	lcd->noDisplay();
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
	lcd->cursor();
	lcd->setCursor(0, 0);
	printStr("CS:");
	lcd->setCursor(0, 1);
	printStr("NM:");
	lcd->noCursor();
}

void LCD::printCurrState()
{
	clearCurrState();
	printStr(intToStateODMap[currState], 3, 0);
}

void LCD::printNextMove()
{
	clearNextMove();
	printStr(intToStateODMap[nextMove], 3, 1);
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

void LCD::populateMap()
{
	//TODO: add maps from integer to name of state/move
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
}
