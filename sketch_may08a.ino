#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLwG5gVBHR"
#define BLYNK_DEVICE_NAME "Water Level Monitoring System"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include <BlynkSimpleEsp8266.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;
bool motPin = 0;
bool automate = 0;
const byte tLength = 50;
short t,cm,percent;

#define trig D3
#define echo D4
#define relay D5

void setup()
{
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(relay, OUTPUT);
  
  Wire.begin(D2, D1);
  lcd.init();
  lcd.backlight();
  

  Blynk.begin("authcodehere", "wifi_ssid", "wifi_password");
  timer.setInterval(10L, Wlevel);
  digitalWrite(relay, HIGH);
  
}


BLYNK_WRITE(V0) {
  motPin = param.asInt();
  if (motPin == 1) {
    digitalWrite(relay, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Mot:ON ");
  } else {
    digitalWrite(relay, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Mot:OFF");
  }
}



BLYNK_WRITE(V2) {
  automate = param.asInt();
   if (automate == 1){
    Blynk.virtualWrite(V0, 0);
    Blynk.syncVirtual(V0);
    Blynk.setProperty(V0, "isDisabled", true);
    lcd.setCursor(8, 1);
    lcd.print("Auto:ON ");
    
    }
  else {
    Blynk.setProperty(V0, "isDisabled", false);
    lcd.setCursor(8, 1);
    lcd.print("Auto:OFF");
    
  }
  
}
 
void loop() {
  Blynk.run();
  timer.run();
}



void checkLevel(){
    if(percent>=0 && percent<50){
    Blynk.virtualWrite(V0,1);
    Blynk.syncVirtual(V0);
  }
    else{
    Blynk.virtualWrite(V0,0);
    Blynk.syncVirtual(V0);
  }
}

 
void Wlevel() {
  
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  t = pulseIn(echo, HIGH);
  cm = t /29 / 2;
  Serial.println(cm);
  percent = ((tLength-cm)*100)/tLength;
  Blynk.virtualWrite(V1,percent);

  if(automate){
    checkLevel();
  }


  
  
  lcd.setCursor(0, 0);
  lcd.print("Water Level: ");
  if(percent >=0 &&  percent <=100)
      lcd.print(percent);
  else lcd.print("  ");
  lcd.print("%  ");
}
