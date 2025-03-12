#include <LiquidCrystal.h>
#define alc A1 
#define stb 3   // connect normal push button (it acts as a start button)
#define motor 6
#define bz 5
#define hst A0
#include <TinyGPS.h>
TinyGPS gps;
float flat=0, flon=0;
const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 =12, d7 =13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int gval=0;
int vhs=0;
int ss=A3;
void read_gps()
{
    bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      if (gps.encode(c)) 
        newData = true;
    }
  }

  if (newData)
  {
    unsigned long age;
    gps.f_get_position(&flat,&flon,&age);

  }
}
int aval;
void setup()
{ 
  Serial.begin(9600);
  lcd.begin(16, 2);  
  lcd.print("  WELCOME");
  lcd.setCursor(0,1);
  lcd.print("INITIALIZING");
  
  lcd.clear();
  lcd.print("INITIALIZED");
   Serial.println("AT");
  delay(1500);
  Serial.println("AT+CMGF=1");
  delay(500);
  //pinMode(ebs,INPUT);
  pinMode(stb,INPUT_PULLUP);
  pinMode(bz,OUTPUT);
  pinMode(motor,OUTPUT);
  pinMode(alc,INPUT);  
  pinMode(hst,INPUT_PULLUP);
  digitalWrite(bz,0);
 
 
}
void loop()
{   
  digitalWrite(motor,1); 
 gval=analogRead(alc);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("A:" + String(gval));
 lcd.setCursor(0,1);
 lcd.print("H:" + String(hst));
 delay(300);
 if(gval>800)
      {
      digitalWrite(bz,1);
      lcd.clear();
      lcd.print("Driver alcoholic ");
      lcd.setCursor(0,1);
      lcd.print("VEHICLE STOPPED");
      digitalWrite(motor,0);
      read_gps();
      send_sms(2);    
      digitalWrite(bz,0);
      while(1);
}
if(digitalRead(hst)==1)
{
      digitalWrite(bz,1);
      lcd.clear();
      lcd.print("Accident occured ");
      digitalWrite(motor,0);
      read_gps();
      send_sms(1);
     
      digitalWrite(bz,0);
      while(1); 
}
}
  void send_sms(int k)
  {
   read_gps();  
Serial.println("Sending SMS...");
Serial.println("AT");    
delay(1000);  
Serial.println("ATE0");    
delay(1000);  
Serial.println("AT+CMGF=1");    
delay(1000);  
Serial.print("AT+CMGS=\"x\"\r\n");// Replace x with mobile number
delay(1000);
if(k==1)
Serial.print("Vehicle got accident detected at");
if(k==2)
Serial.print("Vehicle stopped due to Driver Alcoholic, at");

Serial.println("https://www.google.com/maps/search/?api=1&query=" + String(flat,6)+ "," + String(flon,6));
delay(500);
Serial.print(char(26));
delay(2000); 
  }
