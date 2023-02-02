/************************************************* 
 * EscapeRoombox / Teknologiapaja 2
 * Arduino mega atmega2560
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
#include <Servo.h>

Servo myservo1;
BH1750 lightMeter;

// ISR FLAGS
volatile int startFlag = 0;
volatile int switchFlag = 0;
volatile int lightFlag = 0;
volatile int doorFlag = 0;
volatile int life = 3;
volatile int resetFlag = 0;
const int button = 7;
volatile int lastbuttonstate = 0;


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
char code[]= {'6','5','8','1'};  //The default code, you can change it or make it a 'n' digits one
//char code2[]= {'5','6','7','8'};

char code_buff1[sizeof(code)];  //Where the new key is stored
char code_buff2[sizeof(code)];  //Where the new key is stored again so it's compared to the previous one

short a=0,i=0,s=0,j=0;          //Variables used later

byte rowPins[numRows] = {36,34,32,30}; //Rows 0 to 3 //if you modify your pins you should modify this too
byte colPins[numCols]= {28,26,24,22}; //Columns 0 to 3

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

ISR(INT4_vect)
{
    cli();


startFlag  = 1; 

sei();

}

ISR(INT5_vect)
{
    cli();


switchFlag  = 1; 

sei();

}



void(* resetFunc) (void) = 0;//declare reset function at address 0

void setup(){
  
cli();

Serial.begin(9600);
Serial1.begin(9600);

//Nappien alustus
PORTE |=(1<<PE4);
PORTE |=(1<<PE5);
DDRH |=(1<<PH4);


//INT4 INT5 alustus
EICRB |=(1<<ISC40);
EIMSK |= (1<<INT4);
EIFR |=(1<<INTF4);

EICRB |=(1<<ISC50);
EIMSK |= (1<<INT5);
EIFR |=(1<<INTF5);



Wire.begin();

//LEDien alustus
DDRA  |= ((1<<PA1)|(1<<PA3)|(1<<PA5)); // RED LED OUTPUT
DDRC  |= ((1<<PC2)|(1<<PC4)|(1<<PC6)); // GREEN LED OUTPUT

PORTA &=~((1<<PA1)|(1<<PA3)|(1<<PA5)); // turn off red led
PORTC &=~((1<<PC2)|(1<<PC4)|(1<<PC6)); // turn off green led


sei();


lightMeter.begin();
myservo1.attach(6,600,2300); //(pin,min,max)

myservo1.write(10);

  Serial.print("Setuppi tehty");
}

void ResetGame();
void GameOver();
void Openlock();
void GetCode();
void lightsensor();
void buttonPress();
void OpenDoor();
//void CloseDoor();
void Paniikki();

void loop(){


Serial.println("Alku");
Serial.println(resetFlag);
delay(1000);


  while (startFlag == 1)    //wait to start the game
  {
    volatile int buttonState = digitalRead(button);
     if(buttonState != lastbuttonstate)
      {
    if(buttonState == 1)
    {
      Paniikki();
    }
      }
    else
    {
      Serial1.print(1);                 //send data to ESP32 to start the timer
      Serial.println("Nappi 1");
      delay(10000);
      lightsensor();                    //go to the lightsensor puzzle
      while (lightFlag == 1)            //wait for lightsensor puzzle completed
      {
        Serial.println("Valo");
        delay(1000);
        PORTA |= (1<<PA3);              //RED LED until the switch puzzle is completed
        while(switchFlag == 1)               //wait for switch puzzle completition
        {
          PORTA &=~((1<<PA3)); // turn off red led
          PORTC &=~((1<<PC4)); // turn off green led
          PORTC |=(1<<PC4);             //turn green led on
          Serial.println("Nappi 2");
          Openlock();
          if(doorFlag == 1)
          { 
            Serial.print("Peli läpi");
            OpenDoor();
            Serial1.print(2);
            delay(1000);
            ResetGame() ; 
  
          }
          
        }
      }
    }
   
  }
  
}
  



void lightsensor(){
  
  float lux = 0;
 
  PORTA &=~((1<<PA1)|(1<<PA3)|(1<<PA5)); // turn off red led
  PORTC &=~((1<<PC2)|(1<<PC4)|(1<<PC6)); // turn off green led
  
  while(lux < 7000)                      // Wait for lux to be high enough
  {
    volatile int buttonState = digitalRead(button);
    if(buttonState != lastbuttonstate)
    {
      if(buttonState == 1)
       {
          Paniikki();
       }
    }
    lux = lightMeter.readLightLevel();
    PORTA |=(1<<PA1);
    Serial.print("Light: ");
    Serial.println(lux);
    delay(500);
  }
    PORTA &=~(1<<PA1);
    delay(500);
    PORTC |=(1<<PC2);                   // turn green led on
    lightFlag = 1;                      //mark the lux puzzle completed
  
  
}

 void Openlock(){
  Serial.print("Lock");
 keypressed = myKeypad.getKey(); //Constantly waiting for a key to be pressed
 Serial.print(keypressed);
    if(keypressed == 'E'){                      // * to open the lock
                       
            GetCode();                          //Getting code function
                  if(a==sizeof(code))           //The GetCode function assign a value to a (it's correct when it has the size of the code array)
                  {
                   OpenDoor();
                 doorFlag = 1;
                 PORTC |=(1<<PC6);
                 Serial.print("Oikein");//Open lock function if code is correct
                 delay(2000); 
                  }
                 if(a!=sizeof(code))
                 {
                  for(int i=0;i<3;i++)
                  {
                    PORTA |=(1<<PA5);
                   delay(500);
                   PORTA &=~(1<<PA5);
                   delay(500);
                  }
                   life--;
                   Serial1.print(3);
                   while(life == 0)
                   {
                    GameOver();
                   }
                 }
 
 
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
            if((keypressed == code[i]&& i<sizeof(code))/*||(keypressed == code2[i]&& i<sizeof(code2))*/){            //if the char typed is correct a and i increments to verify the next caracter
                 a++;                                              //Now I think maybe I should have use only a or i ... too lazy to test it -_-'
                 i++;
                 }
            else
                a--;                                               //if the character typed is wrong a decrements and cannot equal the size of code []
            }
            }
    keypressed = NO_KEY;

}


void OpenDoor(){

myservo1.write(40); // Here right angle to open bolt lock

}

/*void CloseDoor(){
 Serial1.print(4); //Send ESP32 Reset number 
 Serial.println("RESET"); 
myservo1.write(0); // Here angle to close bolt lock.
}
*/

void ResetGame(){

for(;;)
{
 volatile int buttonState = digitalRead(button);
 if(buttonState != lastbuttonstate)
 {
   if(buttonState == 1);
  {
    Serial1.print(4);
    delay(500);
    Serial.print("Reset");
    delay(2000);
    resetFunc();
  }
 }
 
  
}

    

 

 }

 void GameOver()
 {
  PORTA &=~((1<<PA1)|(1<<PA3)|(1<<PA5)); // turn off red led
  PORTC &=~((1<<PC2)|(1<<PC4)|(1<<PC6)); // turn off green led
   for(int i=0;i<10;i++)
   {
     PORTA &=~((1<<PA1)|(1<<PA3)|(1<<PA5));
     delay(500);
     PORTA |=((1<<PA1)|(1<<PA3)|(1<<PA5));
     delay(500);
   }
   Paniikki();
 }

 void Paniikki()
 {
  resetFlag = 0;
  OpenDoor();
  delay(5000);
  ResetGame();
  
 }
