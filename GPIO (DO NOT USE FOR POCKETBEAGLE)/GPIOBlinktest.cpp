#include <iostream>
#include <unistd.h>
#include <stdio.h>
using namespace std;

int main()
{
	cout<< "LED Flash start" << endl;

   FILE *export_file = NULL; //declare pointers
   FILE *IO_direction = NULL;
   FILE *IO_value = NULL;
   char str1[] = "0";
   char str2[] = "1";
   char str3[] = "out";
   char str[] = "59";
	
   //this part here exports gpio59
   // echo 59 > export
//    export_file = fopen ("/sys/class/gpio/export", "w");
//    fwrite (str, 1, sizeof(str), export_file);
//    fclose (export_file);

   //this part here sets the direction of the pin
   // echo out > direction
   IO_direction = fopen("/sys/class/gpio/gpio59/direction", "w");
   fwrite(str3, 1, sizeof(str3), IO_direction); //set the pin to HIGH
   fclose(IO_direction);


   usleep (100000);
    // echo 1 > value
    // echo 0 > value
   for (int i=0; i<100; i++){ //blink LED 10 times
        IO_value = fopen ("/sys/class/gpio/gpio59/value", "w");
        fwrite (str2, 1, sizeof(str2), IO_value); //set the pin to HIGH
        fclose (IO_value);
        usleep (100000); //delay for a second

        IO_value = fopen ("/sys/class/gpio/gpio59/value", "w");
        fwrite (str1, 1, sizeof(str1), IO_value); //set the pin to LOW
        fclose (IO_value);
        usleep (100000); //delay for a second
        }
cout << "LED Flash end" << endl;
    return 0;
}