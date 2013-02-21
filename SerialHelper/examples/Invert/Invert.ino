/*
	SerialHelper - Invert
   
	Demonstrates the use of SerialHelper library. 

	Created by Lukasz Kujawa <lukasz@php.net>, http://systemsarchitect.net
*/

#include <SerialHelper.h>

SerialHelper serialHelper(128, 0);

void setup() {
  Serial.begin(9600);
}

void invert(char *buff, int len) {
  int i, tmp;

  for( i = 0 ; i < len / 2 ; i++ ) {
      tmp = buff[len - i - 1];
      buff[len - i - 1] = buff[i];
      buff[i] = tmp;
    }
}

void loop() {
  char *buff;
  int len;
  if( Serial.available() > 0 ) {
    serialHelper.read();
    
    buff = serialHelper.getMessage();
    len = serialHelper.getMessageLength();
    invert(buff, len);
    
    serialHelper.write(buff, len);
  }
}
