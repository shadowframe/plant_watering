
/******************************************************************
 * Pflanzuino 3 - Automatische Bewaesserung
 * 
 * Ardutronix
 * www.ardutronix.de/pflanzuino-3
 * info@ardutronix.de
 ******************************************************************/

////////////////////////////////////////////////////////////////// LIBRARIES

#include <LiquidCrystal.h>

////////////////////////////////////////////////////////////////// CONNECTIONS

#define sensor         A1
#define pump           13
#define piezo          A5
#define lowlevelled    3
#define backlight      10

////////////////////////////////////////////////////////////////// GND-OUTPUTS

#define GND1           2
#define GND2           11
#define GND3           A3

///////////////////////////////////////////////////////////////// LCD & SETTINGS

bool LCDControl = 0, LowUmid = 0, HighUmid = 0;
byte UmidityPercent = 0, moisture = 0, PreviousValue = 0;
int ValUmidade = 0, AnalogValue = 0;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int MOISTURE_SET     = 50;

///////////////////////////////////////////////////////////////// BUTTONS

int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);
 if (adc_key_in > 1000) return btnNONE;
 if (adc_key_in < 50)   return btnRIGHT;
 if (adc_key_in < 250)  return btnUP;
 if (adc_key_in < 450)  return btnDOWN;
 if (adc_key_in < 650)  return btnLEFT;
 if (adc_key_in < 850)  return btnSELECT;
 return btnNONE;

}

///////////////////////////////////////////////////////////////// VOID SETUP

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("- PFLANZUINO 3 -");
  delay(3000);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("MADE BY");
  lcd.setCursor(3,1);
  lcd.print("ARDUTRONIX");
  delay(2000);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("SUPPORT AUF");
  lcd.setCursor(1,1);
  lcd.print("ARDUTRONIX.DE");
  delay(3500);
  pinMode(sensor, INPUT);
  PreviousValue = analogRead(sensor);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT); 
  pinMode(11, OUTPUT); 
  pinMode(A5, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  lcd.clear(); 
}

///////////////////////////////////////////////////////////////// VOID LOOP

void loop()
{
  
  AnalogValue = analogRead(sensor);
  Serial.print("Gemessener Wert: ");
  Serial.println(AnalogValue);
  
  ///////////////////////////////////////////////////////////////// Feuchtigkeitssensor 
  UmidityPercent = map(AnalogValue, 300, 554, 0, 100); 
  /////////////////////////////////////////////////////////////////
  
  moisture = constrain(moisture, 0, 100);
  
  moisture = 100 - UmidityPercent;
  lcd.setCursor(10,1);
  lcd.print("[");
  lcd.setCursor(11,1);
  lcd.print(MOISTURE_SET);
  lcd.print("%");
  lcd.print("] ");
  
  digitalWrite(GND1, LOW);
  digitalWrite(GND2, LOW);
  digitalWrite(GND3, LOW);
  digitalWrite(piezo, LOW);

  if( (moisture > (PreviousValue)+1) || (moisture < (PreviousValue)- 1))
  {
    lcd.setCursor(1,0);
    lcd.print("Sensor: ");
    lcd.print("      ");
    lcd.setCursor(11,0);
    moisture = constrain(moisture, 0, 100);
    lcd.print(moisture);
    lcd.print("%");
    
    if(moisture < MOISTURE_SET && LowUmid == 0)
    {
    digitalWrite(piezo, HIGH);
    delay(30);
    PumpOn();
    LowLevelLed();
    lcd.setCursor(1,1);
    lcd.print("trocken ");
    LowUmid = 1;
    HighUmid = 0;
    }
    if(moisture >= MOISTURE_SET && HighUmid == 0)
    {
    PumpOff();
    LowLevelLedOFF();
    HighUmid = 1;
    LowUmid = 0;
    lcd.setCursor(1,1);
    lcd.print("feucht   ");
    }  
    
    PreviousValue = moisture;
  }
  lcd_key = read_LCD_buttons();
switch (lcd_key)               
 { 
  case btnUP:
     {
     MOISTURE_SET = MOISTURE_SET + 1;
     MOISTURE_SET = constrain(MOISTURE_SET,20,100);
     delay(170);
     digitalWrite(piezo, HIGH);
     delay(30);
     break;
     }
   case btnDOWN:
     {
     MOISTURE_SET = MOISTURE_SET - 1;
     MOISTURE_SET = constrain(MOISTURE_SET,20,100);
     delay(170);
     digitalWrite(piezo, HIGH);
     delay(30);
     break;
     }
     case btnSELECT:
     {
     digitalWrite(piezo, HIGH);
     pinMode(10, OUTPUT);
     digitalWrite (10, HIGH);
     delay (30);
     break;
     }
     case btnLEFT:
     {
     digitalWrite(piezo, HIGH);
     pinMode(10, OUTPUT);
     digitalWrite (10, LOW);
     delay (30);
     break;
     }
 }
 }
void PumpOn(){
    digitalWrite(pump, HIGH);
}
void LowLevelLed(){
    digitalWrite(lowlevelled, HIGH);  
}
void LowLevelLedOFF(){
    digitalWrite(lowlevelled, LOW);
}
void PumpOff(){
    digitalWrite(pump, LOW);
}
