#include <Wire.h>
#include <AM2321.h>
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 

#define buzzer_pin 6 

float Warning_Temperature = 30;

int song[] = {
  330,0,392,587,0,0,532,0,392,349,0,0,330,0,330,330,349,392,
  440,0,0,392,0,0,330,0,392,587,0,0,532,0,392,349,0,0
};

int noteDurations[] = {
   4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
   4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
};

void setup() {
  Serial.begin(115200);
  while(!Serial){;
  }
  pinMode(buzzer_pin, OUTPUT);    
}

void loop() {
  AM2321 am2321;
  am2321.read(); 
   
  float temp = am2321.temperature/10.0;  
  float humidity = am2321.humidity/10.0;
  Serial.println(temp);
  Serial.println(humidity);
 
  
  u8g.firstPage();  
  do {
  u8g.setFont(u8g_font_unifont); 
  u8g.drawStr( 0, 20, "temp");
  u8g.setPrintPos(70, 20);   
  u8g.print(temp);
  u8g.print(" ");
  u8g.print("c");
  
  u8g.drawStr( 0, 42, "Humidity");
  u8g.setPrintPos(70,42); 
  u8g.print(humidity);
  u8g.print(" ");
  u8g.print("%"); 
  
 if(temp>30)
  {
  u8g.drawStr( 0,63, "Open air conditioning");   
  }
  else 
  if(temp<Warning_Temperature);
  {
  u8g.drawStr( 0,63, "");  
  }   
  } while( u8g.nextPage() ); 

  if(temp>Warning_Temperature)
  {
  song_play();
  }
  else 
  if(temp<Warning_Temperature);
  {
  noTone(buzzer_pin);
  }
   delay(500);  //600000 mills = 10 min  
}

void song_play()
{
  for (int thisNote = 0; thisNote < 25; thisNote++)
  {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer_pin, song[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.20;
    delay(pauseBetweenNotes);    
  }
}

