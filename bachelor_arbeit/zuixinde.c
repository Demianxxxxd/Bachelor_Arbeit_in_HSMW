#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>   
#include <EEPROM.h> 
                
int Flag=0;
int X = 0;
int Y = 0;
int count = 0;
int lastbottonstate = 0;
int buttonstate =0 ;
unsigned long starttime;
unsigned long timeoff;
unsigned long timenow;
boolean started = false;
int addr=5;
String startime;
unsigned long timeTotal=0;
unsigned long total=0;
float volt1;
float volt2;

LiquidCrystal_I2C lcd(0x3F,16,2);                     
DS1302 rtc(8,7,6);
    
void setup() {
  Serial.begin(9600);
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,U1,U2");
  lcd.init();                                       
  lcd.backlight();    
  pinMode(A0,INPUT); 
  pinMode(A2,INPUT);
  pinMode(2,OUTPUT); 
  pinMode(3,OUTPUT);    
  pinMode(4,OUTPUT);                              
  pinMode(5,INPUT);
  
  EEPROM.get(addr,total);
  rtc.halt(false);
  rtc.writeProtect(false);
  //rtc.setDOW(TUESDAY);
  //rtc.setTime(19, 02, 12);
  //rtc.setDate(11, 1, 2022);     
}

void loop(){  
  //Serial.print("Pin 5 stasue");
  //Serial.println(digitalRead(5));
  //Serial.print("count statues");
  //Serial.println(count);
  //Serial.print(rtc.getTimeStr());
  //Serial.print("LDR1 Reading");
 //Serial.println(analogRead(A0));
 // Serial.print("LDR2 Reading");
  //Serial.println(analogRead(A1));
  
  X = analogRead(A0);         //Daten von Sensor 1 = X
  volt1= X * (5.0 / 1024.0);  //Die Spannung des gemessenen Widerstands 1
  Y = analogRead(A2);         //Daten von Sensor 2 = Y
  volt2= Y * (5.0 / 1024.0);  //Die Spannung des gemessenen Widerstands 2
  buttonstate=digitalRead(5); //buttonstate mit digitalpin 5 zugeordnet
  if (buttonstate == 1) {                          
    delay(200); Flag++;       //Die vier Zustände der Umschaltung，Jedes Status-LCD zeigt unterschiedliche Inhalte an                  
    if (Flag>=4) {                                  
      Flag = 1;               //Ein Zyklus: Die Taste wird dreimal gedrückt
      }
    }
    

    if  (Flag==0)
    {
    digitalWrite(4,LOW);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    
    lcd.setCursor(0,0); 
    
    lcd.print(rtc.getDOWStr());
    lcd.print("     ");
    lcd.print(rtc.getTimeStr());

    lcd.setCursor(0,1);
    lcd.print("Willkommen ");

   }


    if(Flag==1){                                //Umstand 1 
      
    if (analogRead(A0)>300 && started == false) //Wenn die Zündkerze eingeschaltet ist und boolsche Umstand false ist
  {
    startime=rtc.getTimeStr();                  //Echtzeit durch RTC aufzeichnen 
    starttime = millis();                       //Gesamtlaufzeit des Mikrocontrollers
    started = true;                             //boolsche Umstand true
  }
    if(started=true&&analogRead(A0)>300)        //Wenn die Zündkerze eingeschaltet ist und boolsche Umstand true ist
    {  
    timenow=(millis()-starttime)/1000;          //timenow：Laufzeit der Zündkerze bei diesem Test  
    timeTotal=timenow+total;                    //timeTotal：Die Gesamtlaufzeit der bekannten Zündkerze，jeder Test
    EEPROM.put(addr,timeTotal);                 //timeTotal wird ins ROM gespeichert
    lcd.setCursor(0,0);                         //LCD zeigt Laufzeit und Startzeitpunkt
    lcd.print("Startime");
    lcd.print(startime);
    lcd.setCursor(0,1);
    lcd.print("Laufzeit:");
    lcd.print(timenow);
    lcd.print("s      ");
  }
    
  if (analogRead(A0) < 300){
    timeoff=millis()-starttime;
    digitalWrite(4,LOW);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    lcd.setCursor(0,0); 
    lcd.print("Bitte das Auto    ");
    lcd.setCursor(0,1);
    lcd.print("starten         ");
    
  if(started==true){
    EEPROM.get(addr,total);
    started = false;
    }
  }
}
   
if(Flag==2){
  
if(X-50 <= Y &&Y <= X+50) {           //Wenn der Messwert von Helligkeit 1 und die Daten von Helligkeit 2 ähnlich sind   
      
    lcd.setCursor(0,0);               //LCD zeigt an, dass die unbekannte Zündkerze funktioniert
    lcd.print("Zündkerze   fun");
    lcd.setCursor(0,1);
    lcd.print("ktioniert H1=H2   ");  
    digitalWrite(3,HIGH);              //Pin3 wird auf High-Pegel gesetzt und die grüne LED leuchtet
    digitalWrite(2,LOW);
    digitalWrite(4,LOW);
    }

    if(Y < X-50) {              
      
    lcd.setCursor(0,0); 
    lcd.print("Zündkerze        ");
    lcd.setCursor(0,1);
    lcd.print("kaputt   H1>H2      ");
    digitalWrite(4,HIGH);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    }
    
if(Y > X+50) {             
      
    lcd.setCursor(0,0); 
    lcd.print("Zündkerze      ");
    lcd.setCursor(0,1);
    lcd.print("zu heller  H1<H2     ");
  digitalWrite(2,HIGH);
   digitalWrite(3,LOW);
    digitalWrite(4,LOW); 
    }
    
    Serial.print("DATA, ");
    Serial.print(rtc.getTimeStr());
    Serial.print(",");
    Serial.print(volt1);
    Serial.print(",");
    Serial.println(volt2);  
  }


 
 if(Flag==3){
  
 if (analogRead(A0)>300 && started == false)
  {
    starttime = millis();
    started = true;
  }
  
    if(started=true&&analogRead(A0)>300)
    {
      timenow=(millis()-starttime)/1000;
      timeTotal=timenow+total;
      EEPROM.put(addr,timeTotal);
      Serial.println(timeTotal); 
      lcd.setCursor(0,0); 
      lcd.print("Lebensdauer       ");   
      lcd.setCursor(0,1);
  
      lcd.print(timeTotal);   
      lcd.print("s                ");  
 
      }
    
  if (analogRead(A1) < 300){
    timeoff=millis()-starttime;
    lcd.setCursor(0,0); 
    lcd.print("Bitte das Auto    ");
    lcd.setCursor(0,1);
    lcd.print("starten         ");
    
  if(started==true){
    EEPROM.get(addr,total);
    started = false;
    }   
  }
  } 
}











  
