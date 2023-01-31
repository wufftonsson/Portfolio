/************************************************* 
 * EscapeRoombox / Teknologiapaja 2
 * Arduino mega atmega2560
 * Tekijät: Christian Antonsson ja Maiju Lehtonen
 * Aloituspvm: 27.01.2021
 * Valmis: 
 * Ryhmätunnus: TTV19SA
 ************************************************** 
*/

#include <Arduino.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <Wire.h>
#include <BH1750.h>


BH1750 lightMeter;

// ISR FLAGS
volatile int flag1 = 0;

//Keypad buttons
const byte numRows= 4;          //number of rows on the keypad
const byte numCols= 4;          //number of columns on the keypad

char keymap[numRows][numCols]= 
{
{'1', '4', '7', 'A'}, 
{'2', '5', '8', '0'}, 
{'3', '6', '9', 'B'},
{'F', 'E', 'D', 'C'}
};

char keypressed;                 //Where the keys are stored it changes very often
char code[]= {'1','2','3','4'};  //The default code, you can change it or make it a 'n' digits one

char code_buff1[sizeof(code)];  //Where the new key is stored
char code_buff2[sizeof(code)];  //Where the new key is stored again so it's compared to the previous one

short a=0,i=0,s=0,j=0;          //Variables used later

byte rowPins[numRows] = {36,34,32,30}; //Rows 0 to 3 //if you modify your pins you should modify this too
byte colPins[numCols]= {28,26,24,22}; //Columns 0 to 3

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

ISR(PCINT0_vect)
{
    cli();


flag1  = 1; 

sei();

}

void setup(){
  
cli();

Serial.begin(9600);


PCIFR |=(1<<PCIF0);
PCMSK0 |= (1<<PCINT0);
PCICR |=(1<<PCIE0);
Wire.begin();

sei();

lightMeter.begin();



  
}


//void SLEEP();
//void STARTGAME();
void Openlock();
void GetCode();
void lightsensor();

void loop(){
Openlock();

/*SLEEP();
while(flag1 == 1)
{
  
 sleep_disable();
  
}*/
}
  



//Sleep
/*void SLEEP(){
  
set_sleep_mode(SLEEP_MODE_PWR_DOWN);
sleep_enable();
sei();
sleep_cpu();

sei();
}*/

/*//Game start, when start button is pressed.
void STARTGAME(){
  
  
  
 }*/

void lightsensor(){
  
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(2000);
  
}

 void Openlock(){
  
 keypressed = myKeypad.getKey(); //Constantly waiting for a key to be pressed
 Serial.print(keypressed);
    if(keypressed == 'F'){                      // * to open the lock
                       
            GetCode();                          //Getting code function
                  if(a==sizeof(code))           //The GetCode function assign a value to a (it's correct when it has the size of the code array)
                 // OpenDoor();
                 Serial.print("Oikein");//Open lock function if code is correct
                 delay(2000);
 
 
 }
 
 }

void GetCode(){                  //Getting code sequence
       i=0;                      //All variables set to 0
       a=0;
       j=0; 
                                     
     while(keypressed != 'A'){                                     //The user press A to confirm the code otherwise he can keep typing
           keypressed = myKeypad.getKey();                         
             if(keypressed != NO_KEY && keypressed != 'A' ){       //If the char typed isn't A and neither "nothing"
          
              j++;
            if(keypressed == code[i]&& i<sizeof(code)){            //if the char typed is correct a and i increments to verify the next caracter
                 a++;                                              //Now I think maybe I should have use only a or i ... too lazy to test it -_-'
                 i++;
                 }
            else
                a--;                                               //if the character typed is wrong a decrements and cannot equal the size of code []
            }
            }
    keypressed = NO_KEY;

}
