#include "Lcd.h"

Lcd::Lcd(void) {
  line = 0;
  max_line = 6; 

  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);
  write(LCD_C, 0x21 );  // LCD Extended Commands.
  write(LCD_C, 0xBf );  // Set LCD Vop (Contrast). 
  write(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
  write(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  write(LCD_C, 0x0C );  // LCD in normal mode.
  write(LCD_C, 0x20 );
  write(LCD_C, 0x0C );
}


void Lcd::gotoXY(int x, int y)
{
  write( 0, 0x80 | x);  // Column.
  write( 0, 0x40 | y);  // Row.  
}


void Lcd::drawBox(void)
{
  int j;
  for(j = 0; j < 84; j++) // top
  {
    gotoXY(j, 0);
    write(LCD_D, 0x01);
  }
 
  for(j = 0; j < 84; j++) //Bottom
  {
    gotoXY(j, 5);
    write(LCD_D, 0x80);
  }
 
  for(j = 0; j < 6; j++) // Right
  {
    gotoXY(83, j);
    write(LCD_D, 0xff);
  }
 
  for(j = 0; j < 6; j++) // Left
  {
    gotoXY(0, j);
    write(LCD_D, 0xff);
  }
}
 
void Lcd::scroll(String message)
{
  int scrollPosition = -10;

  for (int i = scrollPosition; i < scrollPosition + 11; i++)
  {
    if ((i >= message.length()) || (i < 0))
    {
      character(' ');
    }
    else
    {
      character(message.charAt(i));
    }
  }
  scrollPosition++;
  if ((scrollPosition >= message.length()) && (scrollPosition > 0))
  {
    scrollPosition = -10;
  }
}
void Lcd::print(char *characters)
{
  while (*characters)
  {
    character(*characters++);
  }
}

void Lcd::println(char *characters)
{
  print(characters);
  newline();
}


void Lcd::print(int characters) {
  char value[10];
  itoa (characters, value, 10);

  print(value);

}


void Lcd::println(int characters) {
  print(characters);
  newline();
}


void Lcd::newline() {

  if(line >= max_line) {
    line = 0;
  }else{
    line++;
  } 

  gotoXY(0,line);

}
void Lcd::write(byte dc, byte data)
{
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}


void Lcd::character(char character)
{
  write(LCD_D, 0x00);
  for (int index = 0; index < 5; index++)
  {
    write(LCD_D, ASCII[character - 0x20][index]);
  }
  write(LCD_D, 0x00);
}

void Lcd::clear(void)
{
  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    write(LCD_D, 0x00);
  }
  gotoXY(0,0);
  line = 0;
}