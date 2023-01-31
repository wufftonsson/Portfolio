#include <LiquidCrystal.h>
#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>



LiquidCrystal lcd(22,23,5,18,19,21);
hw_timer_t * timer = NULL;
volatile int second = -10;
volatile int minute = 0; 
volatile char Sernum = 0;
volatile char Startnum = 0; 
volatile char Winnum = 0; 
volatile int Life  = 2; 
volatile int tempSec = 0;
volatile int tempMin = 0;
volatile int tempH = 0;

IPAddress server_addr(195,148,70,117);  // IP of the MySQL *server* here - eero.dclabra.fi
char user[] = "tekkismysql";            // MySQL user login username
char password[] = "TietoKanta2!sQl";

void insert_SQL(MySQL_Cursor* cur){
  char query[128];
  char INSERT_DATA[]="INSERT INTO testinki.highscore (hours, minutes, seconds) VALUES (%d,%d,%d)";
  sprintf(query,INSERT_DATA, tempH, tempMin, tempSec);
  Serial.println(query);
  cur->execute(query);
 
}

//#define RXD2 16
//#define TXD2 17

void IRAM_ATTR onTimer(){

  second += 1;
}

// WiFi information
const char* ssid = "Campus-LAB";
const char* pass =  "77LABnetti63";

WiFiClient client;  // Use this for WiFi instead of EthernetClient
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;


void connectToNetwork() {
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting");
  lcd.setCursor(4,1);
  lcd.print("TO WIFI");
  Serial.print("yhdistetaan");
  }
 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connection");
  lcd.setCursor(0,1);
  lcd.print("Succesful");
  Serial.print("yhdistetty");
 
}
void connectToSQL()
{
    Serial.print("Connecting to SQL...  ");
  if (conn.connect(server_addr, 3306, user, password))

  Serial.println("OK.");
  else
  Serial.println("FAILED.");
  cursor = new MySQL_Cursor(&conn);
}


void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600);
  
  // Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
   WiFi.mode(WIFI_MODE_STA);
    timer = timerBegin(0,80,true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000, true);
    timerAlarmEnable(timer);
    
    lcd.begin(16, 2);

 

    lcd.clear();
connectToNetwork();
connectToSQL();
    //Serial receive order.

    // go to row 1 column 0, note that this is indexed at 0
   
}



void serialreceive();
void startcode();
void countdown();
void winner();
void gameover();


void loop (){
  
 lcd.clear();
 for(;;)
 {
 lcd.setCursor(0,0);
 lcd.print("PRESS START");
 lcd.setCursor(0,1);
 lcd.print("TO BEGIN");

 


  

serialreceive();

 if(Startnum == 1){
  second = -10;
  while(second < -2)
{ 
  lcd.clear();
lcd.setCursor(0,0);
lcd.print("Here is code");
lcd.setCursor(6,1);
lcd.print("1234");
 }
  while(second >=-2 && second < 0)
 {
 lcd.clear();
 }
  
  for(;;){
   countdown();
   serialreceive();

   if(Winnum == 1 || Life == 0){
    break;
   }
   }
   
 } 
   
 while(Life == 0)
{
  gameover();
}

 while(Winnum == 1){
 winner();
 }
 }
}

void serialreceive(){


 while (Serial2.available()) {    

     Sernum = Serial2.read();
     Serial.print(Sernum);
     
     if(Sernum == '1'){
        
        Startnum = 1;
        Serial.print("start");
         
     }
     
     if(Sernum == '2'){
      Winnum = 1;
      Serial.print("WIN");
      
     }
     
     if(Sernum == '3'){
      Serial.print("outs");
      Life --; 
     }

     if(Sernum == '4'){
      Serial.print("Reset");
      ESP.restart();     
     }
     }
  
}

void countdown(){
  
   lcd.setCursor(0,0);
   lcd.print("Time:");
   lcd.setCursor(8,0);
   lcd.print("Lives:3");

    if(second == 60)
  {
    minute += 1;
    second = 0;
    lcd.clear();
    lcd.print("Time:");
    lcd.setCursor(8,0);
    lcd.print("Lives:3");
  }
  if(minute == 60)
    {
      tempH += 1;
  }

  lcd.setCursor(1,1);
  lcd.print(minute);
  lcd.setCursor(4,1);
  lcd.print("min");

  lcd.setCursor(9,1);
  lcd.print(second);
  lcd.setCursor(12,1);
  lcd.print("sec");
  
  }

  void winner(){

  //Tallennetaan aika lähetettäväksi databaseen
  tempSec = second;
  tempMin = minute;
Serial.print("Olentaalla");
  insert_SQL(cursor);
    Serial.print("SQL sent");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Congratulations");
  lcd.setCursor(4,1);
  lcd.print("YOU WON!"); 
  
  for(;;){ 
  serialreceive();
  }
  }

  void gameover(){
    lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Game Over");
  lcd.setCursor(3,1);
  lcd.print("Game Over");
  serialreceive();
  
    
  }
