
#include <LiquidCrystal_I2C.h>
#define LCD_ADDR 0x20
#define LCD_COLS 16
#define LCD_ROWS 2
#define I2C_BUS 1 // NB: Bus 2 is on port 1 on PB
#define PIN1 9    //NB: on port 1 (i2c bus 2) of PB
#define PIN2 11   //NB

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS, I2C_BUS);
// set the LCD address to 0x20 for a 16 chars and 2 line display

void setup()
{
    lcd.init(); // initialize the lcd

    // Print a message to the LCD.
    lcd.backlight();
    lcd.print("Hello, world!");
}

void loop()
{
}