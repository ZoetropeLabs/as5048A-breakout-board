#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <AS5048A.h>

/**
 * Constructor
 */
AS5048A::AS5048A(byte arg_cs, byte arg_dout, byte arg_din, byte arg_clk){
  cs = arg_cs;
  dout = arg_dout;
  din = arg_din;
  clk = arg_clk;
  errorFlag = false;
  position = 0;

  //setup pins
  pinMode(din, INPUT);
  pinMode(dout, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(cs,OUTPUT);

  digitalWrite(clk, LOW);
  digitalWrite(cs, HIGH);
}

void AS5048A::pause() {
	pinMode(dout, INPUT);
	pinMode(clk, INPUT);
	pinMode(cs, INPUT);
}

void AS5048A::restart() {
  pinMode(din, INPUT);
  pinMode(dout, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(cs,OUTPUT);
  errorFlag = false;
}

/**
 * Get the rotation of the sensor relative to the zero position.
 *
 * @return {int} between -2^13 and 2^13
 */
int AS5048A::getRotation(){
  word data;
  int rotation;

  data = AS5048A::getRawRotation();
  rotation = (int)data - (int)position;
  if(rotation > 8191) rotation = -((0x3FFF)-rotation); //more than -180
  //if(rotation < -0x1FFF) rotation = rotation+0x3FFF;

  return rotation;
}

word AS5048A::getRawRotation(){
  const word commandCode = 0xFFFF;
  return AS5048A::transaction(commandCode) & 0x3FFF;
}

/**
 * returns the value of the state register
 * @return 16 bit word containing flags
 */
word AS5048A::getState(){
  const word commandCode = 0x7FFD; // (precomputed parity and that)

  return AS5048A::transaction(commandCode);
}
/*
void AS5048A::printState(){
  word data;

  data = AS5048A::getState();
  if(AS5048A::error()){
    Serial.print("Error bit was set!");
  }
  Serial.println(data & ~0x4000, BIN);
}
*/
byte AS5048A::getGain(){
  word data = AS5048A::getState();
  return (byte) data & 0xFF;
}

word AS5048A::getErrors(){
  const word commandCode = 0x4001;

  return AS5048A::transaction(commandCode);
}

void AS5048A::setZeroPosition(word arg_position){
  position = arg_position % 0x3FFF;
}

word AS5048A::getZeroPosition(){
  return position;
}

/*
 * Send the 16 bits of data to the sensor, then read back 16 bits
 */
word AS5048A::transaction(word sendData){

  word data = 0;
  char shift;

  digitalWrite(cs, LOW);
  //delay(1);
  //send command
  for(shift=15; shift >= 0; shift--){
    digitalWrite(dout,(sendData >> shift) & 1);
    digitalWrite(clk, HIGH);
    //delay(1);
    digitalWrite(clk,LOW);
    //delay(1);
  }

  digitalWrite(cs, HIGH);
  //delay(1);
  digitalWrite(cs, LOW);
  //delay(1);

  //send a nop and get the data
  digitalWrite(dout, LOW);
  for(shift=15; shift >= 0; shift--){
    digitalWrite(clk, HIGH);
    //delay(1);
    digitalWrite(clk,LOW);
    //delay(1);
    data |= (digitalRead(din) << shift);
  }
  digitalWrite(cs, HIGH);

  //strip parity and return
  data = data & ~0x8000;

  //check for the error bit and set the flag
  if(data&~0x4000){
    errorFlag = true;
  }

  return data;
}

bool AS5048A::error(){
  return errorFlag;
}
