#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
using namespace std;
#define MAX 64

int flashGPIOLED(int, int);
int readButton(int);

int main()
{
    // const int GPIOLED = 59;
    // int times;
    // cout<<"Number of times to flash: ";
    // cin>> times;
    // flashGPIOLED(GPIOLED, times);

    const int GPIOButton = 26;
    readButton(GPIOButton);
    return 0;
}

int flashGPIOLED(int GPIOPin, int times)
{
    cout << "GPIO LED Flash Pin: " << GPIOPin << " start" << endl;
    FILE *LEDHandle = NULL;
    char setValue[2], GPIOString[2], GPIOValue[MAX], GPIODirection[MAX];
    sprintf(GPIOString, "%d", GPIOPin);
    sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
    sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);

    // // Export the pin
    // if ((LEDHandle = fopen("sys/class/gpio/export", "w")) == NULL)
    // {
    //     printf("Cannot export the GPIO pin.\n");
    //     return 1;
    // }
    // strcpy(setValue, GPIOString);
    // fwrite(setValue, sizeof(char), 2, LEDHandle);
    // fclose(LEDHandle);

    // Set direction of the pin
    if ((LEDHandle = fopen(GPIODirection, "rb+")) == NULL)
    {
        printf("Cannot open direction handle.\n");
        return 1;
    }
    strcpy(setValue, "out");
    fwrite(setValue, sizeof(char), 3, LEDHandle);
    fclose(LEDHandle);

    for (int i = 0; i < (times * 2); i++)
    {
        if((LEDHandle = fopen(GPIOValue, "rb+"))==NULL){
            printf("Cannot open value handle.\n");
            return 1;
        }
        if(i%2==1){
            strcpy(setValue, "0");
        }
        else{
            strcpy(setValue, "1");}
        fwrite(setValue, sizeof(char), 1, LEDHandle);
        fclose(LEDHandle);
        usleep(100000);
    }

    // // Unexport the pin
    // if ((LEDHandle = fopen("sys/class/gpio/unexport", "ab")) == NULL)
    // {
    //     printf("Cannot unexport the GPIO pin.\n");
    //     return 1;
    // }
    // strcpy(setValue, GPIOString);
    // fwrite(setValue, sizeof(char), 2, LEDHandle);
    // fclose(LEDHandle);

    cout << "GPIO Led Flash Pin: " << GPIOPin << " end" << endl;

    return 0;
}

int readButton(int inputPin)
{
    cout << "GPIO Input Pin: " << inputPin << " start" << endl;
    FILE *buttonHandle = NULL;
    char setValue[4], GPIOString[4], GPIOValue[MAX], GPIODirection[MAX];

    sprintf(GPIOString, "%d", inputPin);
    sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", inputPin);
    sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", inputPin);

    if((buttonHandle = fopen(GPIODirection, "rb+"))==NULL)
    {
        printf("Cannot open direction handle.\n");
        return 1;
    }
    strcpy(setValue,"in");
    fwrite(setValue, sizeof(char), 3, buttonHandle);
    fclose(buttonHandle);

    if((buttonHandle = fopen(GPIOValue, "rb+"))==NULL)
    {
        printf("Cannot open value handle.\n");
        return 1;
    }
    fread(setValue, sizeof(char),1,buttonHandle);
    fclose(buttonHandle);
    cout<<"The value that was read in is: "<<setValue[0]<<endl;

}