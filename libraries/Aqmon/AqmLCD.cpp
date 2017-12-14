#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "AqmCommons.h"
#include "AqmCommsMgr.h"
#include "AqmLCD.h"

void AqmLCD::init( AqmCommsMgr *commsMgr){
  debug("Starting LCD init()");
  comms = commsMgr;
  _cols = 16;
  _rows = 2;
  _mode = LCD_4BITMODE;
  _displayfunction = _mode|LCD_2LINE|LCD_5x8DOTS;
  begin(16, 2, LCD_5x8DOTS);
  debug("Ending LCD init()");
}

void AqmLCD::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  debug("Starting LCD begin()");
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  delayMicroseconds(50000);
  // Now we pull both RS and R/W low to begin commands
  debug("LCD begin() pulling all pins low");
  command(0x00,true);

  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    debug("LCD begin() 4bit mode handshake 1");
    command(LCD_FUNCTIONSET | _displayfunction|LCD_8BITMODE , true);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    debug("LCD begin() 4bit mode handshake 2");
    command(LCD_FUNCTIONSET | _displayfunction|LCD_8BITMODE , true);
    delayMicroseconds(4500); // wait min 4.1ms

    // third go!
    debug("LCD begin() 4bit mode handshake 3");
    command(LCD_FUNCTIONSET | _displayfunction|LCD_8BITMODE , true);
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    debug("LCD begin() set 4bit mode");
    command(LCD_FUNCTIONSET | _displayfunction , true);
  } else {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    debug("LCD begin() 8bit mode handshake 1");
    // Send function set command sequence
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    debug("LCD begin() 8bit mode handshake 2");
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(150);

    // third go
    debug("LCD begin() set 8bit mode");
    command(LCD_FUNCTIONSET | _displayfunction);
  }


  // finally, set # lines, font size, etc.
  debug("LCD begin() set display function");
  command(LCD_FUNCTIONSET | _displayfunction);

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  debug("LCD begin() set display control");
  display();
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  debug("LCD begin() set entry mode");
  command(LCD_ENTRYMODESET | _displaymode);
}

void AqmLCD::setRowOffsets(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void AqmLCD::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void AqmLCD::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void AqmLCD::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }

  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void AqmLCD::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void AqmLCD::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void AqmLCD::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void AqmLCD::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void AqmLCD::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void AqmLCD::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void AqmLCD::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void AqmLCD::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void AqmLCD::leftToRight(void) {  delayMicroseconds(2000);  // this command takes a long time!

  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void AqmLCD::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void AqmLCD::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void AqmLCD::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters  delayMicroseconds(2000);  // this command takes a long time!

void AqmLCD::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */
inline void AqmLCD::command(uint8_t value) {
  command(value,false);
}

inline void AqmLCD::command(uint8_t value, boolean isNibble) {
  send(value,true,isNibble);
}

inline size_t AqmLCD::write(uint8_t value) {
  send(value,false,false);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/
void AqmLCD::send(uint8_t value, boolean isCommand) {
  send(value,isCommand,false);
}

// write either command or data, with automatic 4/8-bit selection
void AqmLCD::send(uint8_t value, boolean isCommand, boolean isNibble) {
  if(_displayfunction&LCD_8BITMODE){
    debug("Unsupported LCD 8-bit Send "+String(value,HEX));
  }else{
    debug("LCD 4-bit Send "+String(value,HEX));
    byte out = (isCommand?0x00:0x40)|(value>>4);
    //Pulse Enable
    comms->sendByte(out); //Enable LOW
    comms->sendByte(out|0x10); //Enable HIGH
    comms->sendByte(out); //Enable LOW
    delayMicroseconds(100);
    if(!isNibble){ //if !nibble write the next nibble also
      byte out = (isCommand?0x00:0x40)|(value&0x0F);
      //Pulse Enable
      comms->sendByte(out); //Enable LOW
      comms->sendByte(out|0x10); //Enable HIGH
      comms->sendByte(out); //Enable LOW
      delayMicroseconds(100);
    }
  }
}
