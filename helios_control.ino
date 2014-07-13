// The checkMIDI function of this program is based heavily on one by By Amanda Ghassaei
// linked here: http://www.instructables.com/id/Send-and-Receive-MIDI-with-Arduino/

/* * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

*/


byte commandByte;
byte noteByte;
byte velocityByte;

byte noteOn = 144;
byte noteOff = 128;

int torches[3] = {0,0,0};

int max_time_on = 5000; //the maximum time, in MS, that a torch can stay on

void setup(){
  Serial.begin(31250);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void checkMIDI() {
  do {
    if (Serial.available()) {
      commandByte = Serial.read();
      noteByte = Serial.read();
      velocityByte = Serial.read();
      
      if (commandByte == noteOn) {
        switch(noteByte) {
          case 60:
            torches[0] = max_time_on;
            break;
          case 50:
            torches[1] = max_time_on;
            break;
        }
      }
      
      if (commandByte == noteOff) {
        switch(noteByte) {
          case 60:
            torches[0] = 0;
            break;
          case 50:
            torches[1] = 0;
            break;
        }
      }
    }
  } while (Serial.available() > 2);//when at least three bytes available
}

void torch_on(int torch) {
  //turn on the torch at address $foo
}

void torch_off(int torch) {
  //turn off the torch ad address $f00
}

void expire_torches() {
  
  //use an event loop to turn torches off after either the note has been released, or a timer of 5 seconds expires.
  
  for (int i = 0; i < 3; i++) {
    if (torches[i] == max_time_on) {
      //this torch has just been triggered.  Turn it on
    }
    if (torches[i] == 0) {
      //this torch has either expired or its key has been released; turn the torch off
    }
    torches[i] = torches[i] - 1;
  }
}

void loop() {
  checkMIDI();
  write_torches();
  delay(1);
}
