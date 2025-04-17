#include <TimerOne.h>
#include <LiquidCrystal.h>

//LCD
const int rs=11, en=10, d4=8, d5=7, d6=6 , d7=9;
LiquidCrystal lcd(8,7,6,5,4,3);

// digital signal input for humidity
int inPin_digital = 5; // 5 = D2
int hum = 0;  // Variable for humidity percent
volatile int signalCount = 0; // number of digital signal high states

void setup() {

  Serial.begin(9600);
  lcd.begin(16,2);
  

  //Digital signal processing
  pinMode(inPin_digital,INPUT);
  //Initialize timer for calculating time
  Timer1.initialize();
  Timer1.attachInterrupt(myTimerRoutine);
  attachInterrupt(digitalPinToInterrupt(2), pin_ISR, RISING); // inPin, Routine: pin_ISR, rising Edge
}

void loop() 
{

    lcd.clear();
    lcd.write("Hum: ");
    //lcd.print(hum);
    delay(500);
    
}

void pin_ISR(void)
{
    signalCount++;
}

void myTimerRoutine()
{
    hum = map(signalCount, 6900, 7900, 100, 40);
    signalCount = 0;

}