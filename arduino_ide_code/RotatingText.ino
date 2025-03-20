#include <LiquidCrystal.h>

// Define the pins for the LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{

    // Initialize the LCD
    lcd.begin(16, 2);
}

void loop()
{
    // ASCII code starts at 65 for 'A'
    int asciiValue=65;
      // LCD is 16 x 2
    int lcd_Position = 0;
    char letter = ' ';

  byte AwithRing[8] = {
  B00100,
  B01010,
  B01110,
  B00001,
  B01111,
  B10001,
  B01111,
  };
  byte AwithDots[8] = {
  B01010,
  B00000,
  B01110,
  B00001,
  B01111,
  B10001,
  B01111,
  };
  byte OwithDots[8] = {
  B01010,
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01110,
  };


  lcd.createChar(1, AwithRing);
  lcd.createChar(2, AwithDots);
  lcd.createChar(3, OwithDots);

    while (true)
    {
        //Clearing the screen so it show just one character at a time.
        lcd.clear();

        //Set the cursor on correct location, rotating lcd screen like a clock, incrementing one per letter
        if (lcd_Position < 16)
        {
            lcd.setCursor(lcd_Position, 0);
            lcd_Position++;
        }
        else
        {
            lcd.setCursor(31 - lcd_Position, 1);
            lcd_Position++;
            if (lcd_Position == 32)
            {
                lcd_Position = 0;
            }
        }

        //Choose the correct letter to print, A(65) - Z(90), Å(143), Ä(142), Ö(153) and reset when Ö is reached 
        switch (asciiValue)
        {
        case 91:
            letter = ' ';//static_cast<char>(143);
            lcd.write(1);
            asciiValue++;
            break;
        case 92:
            letter = ' ';// static_cast<char>(142);
            lcd.write(2);
            asciiValue++;
            break;
        case 93:
            letter = ' ';// static_cast<char>(153);
            lcd.write(3);
            asciiValue=65;
            break;
        default:
            letter = static_cast<char>(asciiValue);
            asciiValue++;
            break;
        }

        //Printing the letter
        if(letter==' ')
        {
            
        }
        else
        {
            lcd.print(letter);
        }
        

        //Setting a 500ms delay so programm increments controllably
        delay(500);

    }

}