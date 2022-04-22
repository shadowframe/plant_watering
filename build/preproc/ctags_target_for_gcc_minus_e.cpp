# 1 "/Users/jan/Projekte/arduino/plant_watering/plant_watering.ino"

/******************************************************************

 * Pflanzuino 3 - Automatische Bewaesserung

 * 

 * Ardutronix

 * www.ardutronix.de/pflanzuino-3

 * info@ardutronix.de

 ******************************************************************/
# 10 "/Users/jan/Projekte/arduino/plant_watering/plant_watering.ino"
////////////////////////////////////////////////////////////////// LIBRARIES

# 13 "/Users/jan/Projekte/arduino/plant_watering/plant_watering.ino" 2

////////////////////////////////////////////////////////////////// CONNECTIONS







////////////////////////////////////////////////////////////////// GND-OUTPUTS





///////////////////////////////////////////////////////////////// LCD & SETTINGS

bool LCDControl = 0, LowUmid = 0, HighUmid = 0;
byte UmidityPercent = 0, moisture = 0, PreviousValue = 0;
int ValUmidade = 0, AnalogValue = 0;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int MOISTURE_SET = 50;

///////////////////////////////////////////////////////////////// BUTTONS

int lcd_key = 0;
int adc_key_in = 0;
# 48 "/Users/jan/Projekte/arduino/plant_watering/plant_watering.ino"
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);
 if (adc_key_in > 1000) return 5;
 if (adc_key_in < 50) return 0;
 if (adc_key_in < 250) return 1;
 if (adc_key_in < 450) return 2;
 if (adc_key_in < 650) return 3;
 if (adc_key_in < 850) return 4;
 return 5;

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
  pinMode(A1, 0x0);
  PreviousValue = analogRead(A1);
  pinMode(13, 0x1);
  pinMode(12, 0x1);
  pinMode(11, 0x1);
  pinMode(A5, 0x1);
  pinMode(A3, 0x1);
  pinMode(2, 0x1);
  pinMode(3, 0x1);

  lcd.clear();
}

///////////////////////////////////////////////////////////////// VOID LOOP

void loop()
{

  AnalogValue = analogRead(A1);
  Serial.print("Gemessener Wert: ");
  Serial.println(AnalogValue);

  ///////////////////////////////////////////////////////////////// Feuchtigkeitssensor 
  UmidityPercent = map(AnalogValue, 300, 554, 0, 100);
  /////////////////////////////////////////////////////////////////

  moisture = ((moisture)<(0)?(0):((moisture)>(100)?(100):(moisture)));

  moisture = 100 - UmidityPercent;
  lcd.setCursor(10,1);
  lcd.print("[");
  lcd.setCursor(11,1);
  lcd.print(MOISTURE_SET);
  lcd.print("%");
  lcd.print("] ");

  digitalWrite(2, 0x0);
  digitalWrite(11, 0x0);
  digitalWrite(A3, 0x0);
  digitalWrite(A5, 0x0);

  if( (moisture > (PreviousValue)+1) || (moisture < (PreviousValue)- 1))
  {
    lcd.setCursor(1,0);
    lcd.print("Sensor: ");
    lcd.print("      ");
    lcd.setCursor(11,0);
    moisture = ((moisture)<(0)?(0):((moisture)>(100)?(100):(moisture)));
    lcd.print(moisture);
    lcd.print("%");

    if(moisture < MOISTURE_SET && LowUmid == 0)
    {
    digitalWrite(A5, 0x1);
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
  case 1:
     {
     MOISTURE_SET = MOISTURE_SET + 1;
     MOISTURE_SET = ((MOISTURE_SET)<(20)?(20):((MOISTURE_SET)>(100)?(100):(MOISTURE_SET)));
     delay(170);
     digitalWrite(A5, 0x1);
     delay(30);
     break;
     }
   case 2:
     {
     MOISTURE_SET = MOISTURE_SET - 1;
     MOISTURE_SET = ((MOISTURE_SET)<(20)?(20):((MOISTURE_SET)>(100)?(100):(MOISTURE_SET)));
     delay(170);
     digitalWrite(A5, 0x1);
     delay(30);
     break;
     }
     case 4:
     {
     digitalWrite(A5, 0x1);
     pinMode(10, 0x1);
     digitalWrite (10, 0x1);
     delay (30);
     break;
     }
     case 3:
     {
     digitalWrite(A5, 0x1);
     pinMode(10, 0x1);
     digitalWrite (10, 0x0);
     delay (30);
     break;
     }
 }
 }
void PumpOn(){
    digitalWrite(13, 0x1);
}
void LowLevelLed(){
    digitalWrite(3, 0x1);
}
void LowLevelLedOFF(){
    digitalWrite(3, 0x0);
}
void PumpOff(){
    digitalWrite(13, 0x0);
}
