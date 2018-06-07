/*
  The circuit:
 * 5V to Arduino 5V pin
 * GND to Arduino GND pin
 * CLK to Analog #5
 * DAT to Analog #4
*/

/*-----( Import needed libraries )-----*/
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

boolean debug = false;
 
/*-----( Declare Variables )-----*/
//NONE
void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  if (debug) {
    Serial.begin(9600);  // Used to type in characters
    Serial.println ( "Ready" );
    delay (1000);
  }

  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);   // backlight on  

//-------- Write characters on the display ------------------
// NOTE: Cursor Position: (CHAR, LINE) start at 0  

  Wire.begin(10);                // join i2c bus with address #1
  Wire.onReceive(receiveEvent); // register event
  lcd.setCursor(0,0);
  lcd.print("Ada Ready");
  
}/*--(end setup )---*/

unsigned long timeout = 0;
String message = "";

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  char ch;
  int count = 0;
  while (1 < Wire.available()) { // loop through all but the last
    int c = Wire.read(); // receive byte as a character
    if (c != -1) {
       message += (char) c;
       timeout = millis() + 500;
    }
    // Convert character to number 
  }
  int x = Wire.read();    // receive byte as an integer
  if (x != -1) { 
    message += (char)x;
    timeout = millis() + 500;
  }  

}
void writeCh (char ch) {
  static int x = 0;
  static int y = 0;
      // display each character to the LCD
      if (ch == '^') {
        lcd.clear(); // Clear the screen 
        x = 0;
        y = 0;
      } else if ((ch != 10) && (ch != 13)) {
        if (x == 20) {
          x = 0;
          y = y + 1;
          if (y == 4) { 
            y = 0;                          
          }
        }
        lcd.setCursor (x,y);
        if (debug) {
          Serial.print (ch);
        }
        lcd.print(ch);
        x = x + 1;
      }   
  
}
void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  char ch;

  if (timeout) { 
     if (millis() > timeout) { 
        if (debug) {
          Serial.print ( "Now send " );
          Serial.print (message);
          Serial.println ( " to the display." );
        }
        for (int i=0; i<message.length(); i++) {
           ch = message.charAt(i);
           writeCh (ch);
        }
        message = "";
        timeout = 0;
     }
  }
}
