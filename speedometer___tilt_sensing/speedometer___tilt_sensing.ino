/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */
 
 #include "Accelerometer.h"
 Accelerometer myAccelerometer = Accelerometer();

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 7;
int previousState = 0;
int lastMillis = 0;
int count = 0;
int t = 0;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  lastMillis = millis();
  //                    SL GS 0G   X  Y  Z
  myAccelerometer.begin(2, 3, 4, A0, A1, A2);
  delay( 500 );
  myAccelerometer.calibrate();
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int buttonState = digitalRead(pushButton);
  if( buttonState != previousState )
    if( buttonState == 0 ) {
      int newMillis = millis();
      if( newMillis - lastMillis > 10 ) {
        count += 1;
        float mph1 = 2 * 3.1415926 * 1000 * 3600;
        float mph2 = ((newMillis - lastMillis) * 12 * 5280 );
        Serial.print( "s" );
        Serial.print( newMillis );
        Serial.print( "," );
        Serial.println( float( 357 ) / (newMillis - lastMillis) );
        lastMillis = newMillis;
      }
    }
    
  if( t % 5 == 0 ) {
    myAccelerometer.read();
    Serial.print( "a" );
    Serial.print( myAccelerometer._Xgs );
    Serial.print(", ");
    Serial.print( myAccelerometer._Ygs );
    Serial.print(", ");
    Serial.println( myAccelerometer._Zgs );
  }
    
  delay(1);        // delay in between reads for stability
  t += 1;
  previousState = buttonState;
}
