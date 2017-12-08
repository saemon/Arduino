#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "AqmonCommons.h"
#include "AqmonSipo595.h"
#include "AqmonLCD.h"

void AqmonLCD::init(
  byte lcdCols,  // no of columns supported by LCD
  byte lcdRows, // no of rows supported by LCD
  byte lcdMode, //4 ot 8 bit lcd data bus
  byte lcdBusOffset, //bitmask for which 5 bits of sipo bus drive the LCD, must be contiguos and in LCD pin order
  AqmonSipo595 *sipoObj

){
  _cols = lcdCols;
  _rows = lcdRows;
  _mode = lcdMode;
  _busOffset = lcdBusOffset;
  sipo=sipoObj;
  _displayfunction = (_mode>4?LCD_8BITMODE:LCD_4BITMODE) | (_rows>1?LCD_2LINE:LCD_1LINE) | LCD_5x8DOTS;
  begin(_cols, _rows, LCD_5x8DOTS);
}

void AqmonLCD::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }
  delayMicroseconds(50000);
  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    command(LCD_FUNCTIONSET | _displayfunction|LCD_8BITMODE , true);
    delayMicroseconds(4500); // wait min 4.1ms
    command(LCD_FUNCTIONSET | _displayfunction|LCD_8BITMODE , true);
    delayMicroseconds(4500); // wait min 4.1ms
    command(LCD_FUNCTIONSET | _displayfunction|LCD_8BITMODE , true);
    delayMicroseconds(150);
    command(LCD_FUNCTIONSET | _displayfunction , true);
  } else {
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(4500);  // wait more than 4.1ms
    command(LCD_FUNCTIONSET | _displayfunction);
    delayMicroseconds(150);
    command(LCD_FUNCTIONSET | _displayfunction);
  }

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

}

void AqmonLCD::setRowOffsets(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void AqmonLCD::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void AqmonLCD::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void AqmonLCD::setCursor(uint8_t col, uint8_t row)
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
void AqmonLCD::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void AqmonLCD::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void AqmonLCD::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void AqmonLCD::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void AqmonLCD::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void AqmonLCD::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void AqmonLCD::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void AqmonLCD::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void AqmonLCD::leftToRight(void) {  delayMicroseconds(2000);  // this command takes a long time!

  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void AqmonLCD::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void AqmonLCD::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void AqmonLCD::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters  delayMicroseconds(2000);  // this command takes a long time!

void AqmonLCD::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */
inline void AqmonLCD::command(uint8_t value) {
  command(value,false);
}

inline void AqmonLCD::command(uint8_t value, boolean isNibble) {
  send(value,true,isNibble);
}

inline size_t AqmonLCD::write(uint8_t value) {
  send(value,false,false);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/
void AqmonLCD::send(uint8_t value, boolean isCommand) {
  send(value,isCommand,false);
}

// write either command or data, with automatic 4/8-bit selection
void AqmonLCD::send(uint8_t value, boolean isCommand, boolean isNibble) {
  byte out, instr;
  if(_displayfunction&LCD_8BITMODE){
    debug("8-bit Send "+String(value,HEX));
    instr = sipo->getCurOut()&!(LCD_8BITMASK>>_busOffset);
    out = (((value)<<5)|(isCommand?LCD_COMMANDWORD:LCD_DATAWORD));
    //Pulse Enable

    sipo->write1(instr|((out)>>_busOffset)); //Enable LOW
    sipo->write1(instr|((out|LCD_ENABLEUP)>>_busOffset)); //Enable HIGH
    sipo->write1(instr|((out)>>_busOffset)); //Enable LOW
    delayMicroseconds(100);
  }else{
    debug("4-bit Send "+String(value,HEX));
    instr = sipo->getCurOut()&!(LCD_4BITMASK>>_busOffset);
    out = (((value&0x0F)<<9)|(isCommand?LCD_COMMANDWORD:LCD_DATAWORD));
    //Pulse Enable
    sipo->write1(instr|((out)>>_busOffset)); //Enable LOW
    sipo->write1(instr|((out|LCD_ENABLEUP)>>_busOffset)); //Enable HIGH
    sipo->write1(instr|((out)>>_busOffset)); //Enable LOW
    delayMicroseconds(100);
    if(!isNibble){ //if !nibble write the next nibble also
      out = (((value&0xF0)<<5)|(isCommand?LCD_COMMANDWORD:LCD_DATAWORD)); //
      //Pulse Enable
      sipo->write1(instr|((out)>>_busOffset)); //Enable LOW
      sipo->write1(instr|((out|LCD_ENABLEUP)>>_busOffset)); //Enable HIGH
      sipo->write1(instr|((out)>>_busOffset)); //Enable LOW
      delayMicroseconds(100);
    }
  }
}
