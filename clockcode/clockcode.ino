/*
    Fades a line down the channels, with max value and duration based on
    the voltage of analog pin 0.
    Try grounding analog 0: everything should turn off.
    Try putting +5V into analog 0: everything should turn on.

    See the BasicUse example for hardware setup.

    Alex Leone <acleone ~AT~ gmail.com>, 2009-02-03 */

#include <Time.h>
#include <Servo.h> 
#include "Tlc5940.h"
#include "tlc_fades.h"

long rise, set, curtime, start;
long night_1_mag, night_2_mag, night_mag, day_mag;

float needle_pos;
int ones_pos, tenths_pos, hundredths_pos;
int ones_pos_old, tenths_pos_old, hundredths_pos_old;

int LED_map[ 10 ];

TLC_CHANNEL_TYPE channel;

Servo myservo;

void setup()
{
  LED_map[0] = 0;
  LED_map[1] = 3;
  LED_map[2] = 6;
  LED_map[3] = 9;
  LED_map[4] = 12;
  LED_map[5] = 15;
  LED_map[6] = 18;
  LED_map[7] = 21;
  LED_map[8] = 24;
  LED_map[9] = 27;
  
  Tlc.init();
  rise = long(7)*3600 + 58*60;
  set = long(12+5)*3600 + 29*60;
  start = long( (12+8) ) *3600  + 17*60 + 0*1;
  
  night_1_mag = rise;
  night_2_mag = long(24) * 3600 - set;
  night_mag = night_1_mag + night_2_mag;
  day_mag = set - rise;
  
  myservo.attach(12);
  
  Serial.begin( 9600 );
  
  delay( 2000 );
}

void loop()
{
  curtime = start;
  //Serial.println( curtime );
  curtime += now();
  //Serial.println( curtime );
  
  float percent;
  if( curtime < rise ) {
   percent = float( curtime + night_2_mag ) / float( night_mag );
  } else if ( set < curtime ) {
    percent = float( curtime - set ) / float( night_mag );
  } else if (rise < curtime && curtime < set) {
    percent = float( curtime - rise ) / float( day_mag );
  }
  
  needle_pos = floor( float( floor( percent * 10 ) ) * 180 / 10 );
  ones_pos = floor( 10*(percent / 10 - floor( percent / 10 )) );
  tenths_pos =  floor( 10*(percent - floor( percent ) ) );
  hundredths_pos = floor( 10*(percent*10 - floor( percent*10 ) ) );
  
  uint16_t duration = 500;
  uint32_t startMillis = millis() + 1000;
  uint32_t endMillis = startMillis + duration;
  
  if( ones_pos != ones_pos_old ) {
    tlc_addFade(LED_map[ones_pos], 0, 800, startMillis, endMillis);
    Serial.println( "ones change" );
  }
  if( tenths_pos != tenths_pos_old ) {
    tlc_addFade(LED_map[tenths_pos]+1, 0, 800, startMillis, endMillis);
    Serial.println( "ten change" );
  }
  if( hundredths_pos != hundredths_pos_old ) {
    tlc_addFade(LED_map[hundredths_pos]+2, 0, 800, startMillis, endMillis);
    Serial.println( "hundo change" );
  }

/*
  Serial.print( percent );
  Serial.print( "," );
  Serial.print( needle_pos );
  Serial.print( "," );
  Serial.print( ones_pos );
  Serial.print( "," );
  Serial.print( tenths_pos );
  Serial.print( "," );
  Serial.println( hundredths_pos );
*/

  ones_pos_old = ones_pos;
  tenths_pos_old = tenths_pos;
  hundredths_pos_old = hundredths_pos;
  
  tlc_updateFades();
  
  myservo.write(0);
}

