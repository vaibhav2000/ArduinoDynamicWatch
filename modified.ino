/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

*******IMPORTANT***********
PUT SIREN IN 7TH PIN
****************************

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

long int curr, alarm,timer, stoptime;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial Genotronex(8, 9);//rx tx 

int mode;
bool timeInput;
bool timerflag;

bool stopwatch;



void setup() {
  Genotronex.begin(9600); 
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Enter Time:");


  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, OUTPUT);
  
  timeInput=false;
  timerflag= false;
  stopwatch=false;
  stoptime=curr=0;
  alarm=-1;
  timer=-1;
  mode=-1;

}

bool checkifValid(String content)
{
   int cnt=0;

        for(int i=0;i<content.length();i++)
         if(content[i]== ':')
           cnt++;
        else
        if(!isDigit(content[i]))
         cnt=5;
 
          lcd.setCursor(0,1);
           lcd.print((String)cnt+ (String)" " + (String)content.length());
          
        if(cnt==2&&content.length()==8&&content[2]==':'&&content[5]==':')
         return true;
     
     return false;    
  }

void doInitalTimeSetup(String content)
  { 
        if(checkifValid(content))
         {
           timeInput=true;
           mode=0;
           curr= 3600*(content.substring(0,2)).toInt() + 60*(content.substring(3,5)).toInt() + (content.substring(6,8)).toInt() ;
           lcd.clear();
           lcd.print("Enter Time:");
           
           lcd.setCursor(0,1);
           lcd.print("Success!");
           delay(1000);
          }
        else
        {
          lcd.setCursor(0,1);
          lcd.print("Wrong Format");
           }    
    }

void showTime()
{
   curr%=  (long long)24*3600;
   
  long int copy= curr;
   
  String hr= (String)(curr/3600);  
  
  
  if(hr.length()==1)
   hr = (String)"0"+ hr;

  curr%= 3600;  

  String ms= (String)(curr/60);
  if(ms.length()==1)
   ms= (String)"0"+ ms;

  curr%=60;

  String sec= (String)curr;
  if(sec.length()==1)
   sec=(String)"0"+ sec; 
  
  // print the number of seconds since reset:
 curr= copy;
    lcd.print(hr+":"+ms+":"+ sec);
 
   }

  String convertToHumanTime(unsigned long int timer)
  {
    unsigned long int curr=timer;
     String hr= (String)(curr/3600);  
  
  
  if(hr.length()==1)
   hr = (String)"0"+ hr;

  curr%= 3600;  

  String ms= (String)(curr/60);
  if(ms.length()==1)
   ms= (String)"0"+ ms;

  curr%=60;

  String sec= (String)curr;
  if(sec.length()==1)
   sec=(String)"0"+ sec; 
  
  // print the number of seconds since reset:
     
    String temp= hr+":"+ms+":"+ sec;
  return temp;
    }

  void setTimer(String content)
  {
    curr++;
    if(checkifValid(content)==false)
     {lcd.setCursor(0,1);
     lcd.print("Wrong input");
     delay(1000);
     return;}
    
    timer= 3600*(content.substring(0,2)).toInt() + 60*(content.substring(3,5)).toInt() + (content.substring(6,8)).toInt() ;
    timer%= (long long)24*3600;

     lcd.setCursor(0,1);
     lcd.print("Start!     ");

    delay(1000);
    } 


  
  void setAlarm(String content)
  {
    curr++;
    
    if(checkifValid(content)==false)
     {lcd.setCursor(0,1);
     lcd.print("Wrong input");
     delay(1000);
     return;}

    lcd.clear();
    lcd.print("Next Alarm");

    lcd.setCursor(0,1);
    lcd.print((String)"At "+ content);

    
    alarm= 3600*(content.substring(0,2)).toInt() + 60*(content.substring(3,5)).toInt() + (content.substring(6,8)).toInt() ;
    alarm%= (long long)24*3600;
    
    delay(1000);
    
     
    } 

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  Serial.println(analogRead(A1));
 
  char c;
  String content="";
  while(Genotronex.available())
   {
     c= Genotronex.read();
     content.concat(c);
     delay(10);
     }

  content.toLowerCase();
  
  if(timeInput==false)
  {
  if(content!="")
   doInitalTimeSetup(content);
    
    }
  else
    if(content.substring(0,4)=="mode")
    {
      if(content.substring(5,10)=="clock")
       mode=0;
      else
      if(content.substring(5,10)=="alarm")
       mode=1;
      else
      if(content.substring(5,10)=="timer")
       mode=2;
      else
      if(content.substring(5,14)=="stopwatch")
       mode=3;
       
     stopwatch=false;
     digitalWrite(LED_BUILTIN, LOW);
     digitalWrite(7, LOW);
    }  
  else
  if(mode==0)
  {
   lcd.clear();
   lcd.print("Clock");

   lcd.setCursor(0,1);
   showTime();
  }
  else
  if(mode==1)
  {
    lcd.clear();
    lcd.print("Alarm Mode:");

    if(content!="")
    setAlarm(content);
      
    }
  else
  if(mode==2)
  {
   lcd.clear();
   lcd.print("Timer:");

  if(timer==-1)
   {if(content=="")
    {lcd.setCursor(0,1);
     lcd.print("00:00:00");}
   else
    {digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(7, LOW);
    timerflag=true;
    setTimer(content);}
   }
   else
   { 
     if(content=="0")
      {timer=-1;  
       digitalWrite(LED_BUILTIN, LOW);
         digitalWrite(7, LOW);}
     else
     if(timer==0)
      {
         timer=-1;
         timerflag=false;
         digitalWrite(LED_BUILTIN, HIGH);
         digitalWrite(7, HIGH);
        }
    else 
     {lcd.setCursor(0,1);
     lcd.print(convertToHumanTime(timer));

     if(content=="+")
      timerflag=true;
     else
     if(content=="-")
      timerflag=false;


    if(timerflag)    
     timer--;
     
     
     }
     
     } 
  }    
  else
  if(mode==3)
  {
   lcd.clear();
   lcd.print("Stopwatch");

   lcd.setCursor(0,1);
   lcd.print(convertToHumanTime(stoptime)); 

   if(content=="-")
     stopwatch=false;
   else
   if(content=="+")
    stopwatch= true;
   else
   if(content=="0")
    {
      stopwatch=false;
      stoptime=0;}
     
   if(stopwatch)
    stoptime++;
    }


 if(curr==alarm)
 {
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(7, HIGH);
  //Kill bill's Siren here
  
 }
 if(digitalRead(LED_BUILTIN)== HIGH&&curr-alarm==30)
 {
   alarm=-1;
   digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(7, LOW);
   
  }
  


 curr++;
 delay(1000);
}
