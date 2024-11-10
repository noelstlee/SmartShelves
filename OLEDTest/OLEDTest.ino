/*
OLED Directions and Shelf Stage Test Display
*/
#include <Wire.h>
#include <SFE_MicroOLED.h>

#define PIN_RESET 9

MicroOLED oled(PIN_RESET);

void setup()
{
  Serial.begin(115200); // Begin the Serial port
  Serial.println(F("Smart Shelf OLED Test Code"));
  
  delay(100);
  Wire.begin();

  // This is the new way of initializing the OLED.
  // We can pass a different I2C address and TwoWire port
  // If 0x3D does not work, try 0x3C
  oled.begin(0x3D, Wire);    // Initialize the OLED

  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)

  delay(1000); // Delay 1000 ms

  oled.clear(PAGE); // Clear the buffer.
}

void loop()
{
  oled.setFontType(3);
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.write('1');
  oled.display();
  delay(500); 
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.write('2');
  oled.display();
  delay(500); 

  oled.setFontType(1);
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.write('L');
  oled.write('E');
  oled.write('F');
  oled.write('T');
  oled.display();
  delay(500); 

  oled.setFontType(1);
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.write('R');
  oled.write('I');
  oled.write('G');
  oled.write('H');
  oled.write('T');
  oled.display();
  delay(500); 
 
}