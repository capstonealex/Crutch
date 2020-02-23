#include <iostream>
#include <unistd.h>
#include <stdio.h>
using namespace std;

int main()
{
    cout << "LED Flash start" << endl;

    FILE *LEDHandle = NULL;
    char *LEDBrightness = "/sys/class/leds/beaglebone:green:usr3/brightness";

    for (int i = 0; i < 10; i++)
    {

        if ((LEDHandle = fopen(LEDBrightness, "r+")) != NULL)
        {
            fwrite("1", sizeof(char), 1, LEDHandle);
            fclose(LEDHandle);
        }
        usleep(1000000);

        if ((LEDHandle = fopen(LEDBrightness, "r+")) != NULL)
        {
            fwrite("0", sizeof(char), 1, LEDHandle);
            fclose(LEDHandle);
        }
        usleep(1000000);
    }
    cout << "LED Flash end" << endl;
    return 0;
}