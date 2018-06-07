
/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>

/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 16 chars 2 line display
// A FEW use address 0x3F
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
 
/*-----( Declare Variables )-----*/
//NONE
void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);  // Used to type in characters
  Serial.println ( "Ready" );
  delay (1000);

  lcd.begin(20,4);   // initialize the lcd for 20 chars 4 lines, turn on backlight
  lcd.backlight();   // backlight on  

//-------- Write characters on the display ------------------
// NOTE: Cursor Position: (CHAR, LINE) start at 0  

  Wire.begin(10);                // join i2c bus with address #1
  Wire.onReceive(receiveEvent); // register event
  lcd.setCursor(0,0);
  lcd.write("Ada Ready");
  
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
        Serial.print (ch);
        lcd.write(ch);
        x = x + 1;
      }   
  
}
void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  char ch;

  if (timeout) { 
     if (millis() > timeout) { 
        Serial.print ( "Now send " );
        Serial.print (message);
        Serial.println ( " to the display." );
        for (int i=0; i<message.length(); i++) {
           ch = message.charAt(i);
           writeCh (ch);
        }
        message = "";
        timeout = 0;
     }
  }
}
