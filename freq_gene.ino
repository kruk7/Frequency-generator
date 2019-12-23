#include <Timers.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//Arduino connections
//(3 pin digital) -> frequency signal
//(A0 pin analog) -> potentiometer
//(A4 pin analog) -> SDA from I2C driver
//(A5 pin analog) -> SCL from I2c driver

LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int inticatorSignal = 3;
int MIN = 0;
int MAX = 0;
float fraction = 0;
float percent = 0;
int duty = 0;

void setup()
{
  // avtivate indicator signal
  pinMode(inticatorSignal, OUTPUT);
  
  // activate LCD module
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  
  //Start program
  calibration();
}

void loop()
{
  lcd.clear();
  lcd.home();
  sensorValue = analogRead(sensorPin);
  lcd.print("Value:");
  lcd.setCursor(7,0);
  lcd.print(sensorValue);
  lcd.setCursor(12,0); //temporary views
  lcd.print(fraction); //temporary views
  lcd.setCursor(0,1);
  lcd.print("Perce:");
  lcd.setCursor(7,1); 
  lcd.print(getPercent(sensorValue));
  lcd.print("%");
  analogWrite(inticatorSignal, getDuty(fraction));
  
  lcd.setCursor(12,1);//temporary views
  lcd.print(duty); //temporary views
  delay(500);
}

int getDuty(float fraction)
{
  duty = (int)(fraction*255);
  return duty;
}

int getPercent(int sensorValue)
{
  fraction = (float)(sensorValue-MIN)/(MAX-MIN);
  percent = fraction * 100;
  
  return (int)percent;
}

void calibration()
{
  Timer counter;
  int time = 5;

  bool calibLeft = false;
  bool calibRight = false;

   lcd.clear();
   lcd.home();
   lcd.print("  potentiometer");
   lcd.setCursor(3,1);
   lcd.print("calibration");
   delay(2000);

  //MIN Calibration
  counter.begin(1000);
  while(calibLeft == false)
  {
      if(counter.available())
      {
        lcd.clear();
        lcd.home();
        lcd.print("Turn knop to max");
        lcd.setCursor(0,1);
        lcd.print("left until: ");
        lcd.print(time);
        time--;
          if(time == 0)
          {
            MIN = analogRead(sensorPin);  
            calibLeft=true;  
          }
      counter.restart();
      }
  }
  //MAX Calibration
  counter.begin(1000);
  time = 5;
  while(calibRight == false)
  {
      if(counter.available())
      {
        lcd.clear();
        lcd.home();
        lcd.print("Turn knop to max");
        lcd.setCursor(0,1);
        lcd.print("right until: ");
        lcd.print(time);
        time--;
          if(time == 0)
          {
            MAX = analogRead(sensorPin);  
            calibRight=true;  
          }
      counter.restart();
      }
  }

  
}
