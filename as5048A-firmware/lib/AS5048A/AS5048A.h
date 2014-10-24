#ifndef as5048_h
#define as5048_h
#define LIBRARY_VERSION 1.0.0

class AS5048A{

  bool errorFlag;
  byte cs;
  byte dout;
  byte din;
  byte clk;
  word position;
  word transaction(word data);

  public:

  /**
   *
   */
  AS5048A(byte arg_cs, byte arg_dout, byte arg_din, byte arg_clk);

  //most important!
  int getRotation();

  void pause();

  void restart();

  //return the raw position
  word getRawRotation();

  //just for diagnostics
  word getState();

  //print the state, over the serial line
  void printState();

  //like get start, but just returns the gain
  byte getGain();

  //get the error flags and reset the error register
  word getErrors();

  //set what value is the zero position, returned
  //values will be relative to this
  void setZeroPosition(word arg_position);

  word getZeroPosition();

  //returns true if there's an error
  bool error();
};
#endif
