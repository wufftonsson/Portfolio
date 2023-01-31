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
#include <Servo.h>



// ISR FLAGS
volatile int flag1 = 0; 
volatile int lightFlag = 0;

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

char keypressed;                 //Where the keys are stored it ch)||(keypressed == code2[i]&& i<sizeof(code2))anges very often)||(keypressed == code2[i]&& i<sizeof(code2))
char code[]= {'1','2','3','4'};//The default code, you can change it or make it a 'n' digits one
//char code2[]= {'5','6','7','8'};

char code_buff1[sizeof(code)];  //Where the new key is stored
char code_buff2[sizeof(code)];  //Where the new key is stored again so it's compared to the previous one

short a=0,i=0,s=0,j=0;          //Variables used later

byte rowPins[numRows] = {36,34,32,30}; //Rows 0 to 3 //if you modify your pins you should modify this too
byte colPins[numCols]= {28,26,24,22}; //Columns 0 to 3

Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

Servo myservo1;
BH1750 lightMeter;

// TÄSSÄ ON SETUP HOX

void setup(){

cli();
 Serial.begin(9600);

// ext-interrupt
PCIFR |=(1<<PCIF0);
PCMSK0 |= (1<<PCINT0);
PCICR |=(1<<PCIE0);


//LEDien alustus
DDRA  |= ((1<<PA1)|(1<<PA3)|(1<<PA5)); // RED LED OUTPUT
DDRC  |= ((1<<PC2)|(1<<PC4)|(1<<PC6)); // GREEN LED OUTPUT

PORTA &=~((1<<PA1)|(1<<PA3)|(1<<PA5)); // turn off red led
PORTC &=~((1<<PC2)|(1<<PC4)|(1<<PC6)); // turn off green led

// Switch puzzle button
DDRB &=~(1<<PB2);
// Reset button
DDRL &= ~(1<<PL0);

sei();
// Servo setup
myservo1.attach(2,600,2300); //(pin,min,max)

lightMeter.begin();
Wire.begin();
Serial.print("Setuppi tehty");  
}


//void STARTGAME();
void Openlock();
void GetCode();
//void OpenDoor();
//void CloseDoor();

void loop(){
//Serial.print("1");
 Openlock();
/*while(flag1 == 1)
{
  
 
}*/
}




  ISR(PCINT0_vect)
{
 cli();


flag1  = 1; 

sei();

}



/*//Game start, when start button is pressed.
void STARTGAME(){
  
  
  
 }*/

void lightsensor(){
  
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(2000);

  PORTA &=~((1<<PA1)|(1<<PA3)|(1<<PA5)); // turn off red led
  PORTC &=~((1<<PC2)|(1<<PC4)|(1<<PC6)); // turn off green led

  if(lux >=12000)
  {
    PORTC |=(1<<PC2);
    lightFlag = 1;
  }
  else
  {
    PORTA |=(1<<PA1);
  }
  
}

 void Openlock(){
  
 keypressed = myKeypad.getKey(); //Constantly waiting for a key to be pressed
 Serial.print(keypressed);
    if(keypressed == 'F'){                      // * to open the lock
                       
            GetCode();                          //Getting code function
                  if(a==sizeof(code)){           //The GetCode function assign a value to a (it's correct when it has the size of the code array)
                 // OpenDoor();
                 Serial.print("Oikein");//Open lock function if code is correct
                 delay(2000);}
 
 
 }
 
 }

void GetCode(){                  //Getting code sequence
       i=0;                      //All variables set to 0
       a=0;
       j=0; 
                                     
     while(keypressed != 'A'){                                     //The user press A to confirm the code otherwise he can keep typing
           keypressed = myKeypad.getKey();                         
             if(keypressed != NO_KEY && keypressed != 'A' ){       //If the char typed isn't A and neither "nothing"
          
              j++;}
            if(keypressed == code[i]&& i<sizeof(code))/*||(keypressed == code2[i]&& i<sizeof(code2))*/{            //if the char typed is correct a and i increments to verify the next caracter
                 a++;                                              //Now I think maybe I should have use only a or i ... too lazy to test it -_-'
                 i++;
                 }
            else{
                a--;                                               //if the character typed is wrong a decrements and cannot equal the size of code []
            }
            }
    keypressed = NO_KEY;

}

/*void OpenDoor(){

myservo1.write(); // Here right angle to open bolt lock

}

void CloseDoor(){

myservo1.write(); // Here angle to close bolt lock.
}
*/

/*void ResetGame(){
 
 CloseDoor();
 }

 void red_led(int x){
int x;
 

if(x ==1)
{
PORTA |=(1<<PA1);
}

if(x ==2)
{
PORTA |=(1<<PA3);
}

if(x ==3)
{
PORTA |=(1<<PA5);
}
 
 }


*/
