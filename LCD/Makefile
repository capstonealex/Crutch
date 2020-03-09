
CXX = arm-linux-gnueabihf-g++
CXXFLAGS = -Wall -std=c++11

SRC_PATH = ./src/
LIB_PATH = ./lib/

libLiquidCrystal_I2C.a : LiquidCrystal_I2C.o
	$(CXX) -c -static $(CXXFLAGS) LiquidCrystal_I2C.o
	ar -rs $(LIB_PATH)libLiquidCrystal_I2C.a LiquidCrystal_I2C.o
	cp $(SRC_PATH)LiquidCrystal_I2C.h $(LIB_PATH)

LiquidCrystal_I2C.o : $(SRC_PATH)LiquidCrystal_I2C.h 
	$(CXX) $(CXXFLAGS) -c $(SRC_PATH)LiquidCrystal_I2C.cpp