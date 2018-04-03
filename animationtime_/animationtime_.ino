

#include "Tlc5940.h"
#include "stdlib.h"

struct rgbvalue {
  int r;
  int g;
  int b;
};

struct rock {
  char animation;
};



int t;
int prev;
rock rockArray[5];

void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
  Tlc.init(0);
  Serial.begin( 9600 );
  t=0;
  prev=0;
  
  rock rockArray[5] = { {char(102)}, {'f'}, {'f'}, {'f'}, {'f'} };
}

struct rgbvalue animate( int t, char animation, int light ) {
  struct rgbvalue ret;
  Serial.println( int(animation) );
  if( animation == 0 ) {
    ret.r = 0;
    ret.g = 0;
    ret.b = 0;
  }
  
  if( animation == 'o' ) {
    ret.r =  4095;
    ret.g = float (240
    * 4095) / 256;
    ret.b = 0;
  }
  
  if( animation == 'f' ) {
    ret.r = abs( 4095 - 2 * float(t % 5) * 4095 / 5 );
    ret.g = 0;
    ret.b = 0;
  }
  
  
  
  if( animation == 's' ) {
    ret.r = 800;
    ret.g = 200;
    ret.b = 500;
  }
  
  
  //ret.r = abs( float (t % 25) * 4095 / 25 );
  //ret.g = 0;
  //ret.b = 0;
  
  return ret;
}

void loop()
{
  
  int comp_start = millis();
  
  if( Serial.available() > 2 ) {
    char action = Serial.read();
    if( action == 'A' ) { //animate
      int rock = Serial.read() - '0';
      char animation = Serial.read();
      Serial.println( rock );
      Serial.println( animation );
      rockArray[ rock ].animation = animation;
      for( int i=0; i<5; i++ )
        Serial.println( rockArray[ i ].animation );  
    }
  }
  
  if( t%100 == 0 ) {
   int val = analogRead( 0 );
   if( abs( val - prev ) > 15 ) {
     //Serial.print( "ET" ); //event, touch
     //Serial.println( val ); //which one
     prev = val;
   }
  }
  
  for( int r=0; r<NUM_TLCS; r++ ) {
    for( int light=0; light<5; light++ ) {
      struct rgbvalue calculateanimation = animate( t, rockArray[ r*5 + light ].animation, light );
      Tlc.set( r * 16 + light * 3, calculateanimation.r );
      Tlc.set( r * 16 + light * 3 + 1, calculateanimation.g );
      Tlc.set( r * 16 + light * 3 + 2, calculateanimation.b );
    }
  }
  
  t++;
  
  Tlc.update();
  
  int comp_end = millis();
  
  delay( 50 - (comp_end - comp_start) );
}
