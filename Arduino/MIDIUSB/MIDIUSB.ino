#include "MIDIUSB.h"

struct Command {
  uint8_t channel = 0;
  uint8_t note = 0;
  uint8_t velocity = 0;
};

void setup() {
  Serial.begin(57600);
  Serial1.begin(31250);
}

midiEventPacket_t rx;

void loop() {
  if (Serial1.available() > 2) {
    midiEventPacket_t cmd;
    cmd.byte1 = Serial1.read();
    cmd.byte2 = Serial1.read();
    cmd.byte3 = Serial1.read();
    cmd.header = cmd.byte1 >> 4;
    /* DEBUG
    Serial.print(cmd.header, HEX);
    Serial.print(" ");
    Serial.print(cmd.byte1, HEX);
    Serial.print(" ");
    Serial.print(cmd.byte2, HEX);
    Serial.print(" ");
    Serial.print(cmd.byte3, HEX);
    Serial.println();
    /**/
    MidiUSB.sendMIDI(cmd);
    MidiUSB.flush();
  }
  /*
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      Serial1.write(rx.byte1);
      Serial1.write(rx.byte2);
      Serial1.write(rx.byte3);
      /* DEBUG
      Serial.print(rx.byte1, DEC);
      Serial.print(" ");
      Serial.print(rx.byte2, DEC);
      Serial.print(" ");
      Serial.print(rx.byte3, DEC);
      Serial.println();
    }
  } while (rx.header != 0);
      /**/
}
