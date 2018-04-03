/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.
 
 This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  float ballsk[32] = {0, 42, 75.5, 124.5, 131, 179.5, 216.5, 268.5, 224, 273.5, 311, 365, 374, 426, 465.5, 519, 382, 429, 467.5, 519, 529, 580, 620.5, 673.5, 631.5, 679.5, 718, 769.5, 779.5, 828.5, 866.5, 915};
  int sensorValue = analogRead(A0);
  // print out the value you read:
  for( int i=0; i<32; i++ ) {
    if( abs( ballsk[i] - (float)sensorValue ) < 1.5 ) {
      Serial.println( i );
      break;
    }    
  }
  
  Serial.println( sensorValue );
  delay(100);        // delay in between reads for stability
}
