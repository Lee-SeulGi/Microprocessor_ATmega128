// LCD를 이용한 스톱워치 디지털 시계

#define CMD_CREAR_DISPLAY     0x01 //1. Clear Display
#define CMD_RETURN_HOME      0x02 //2. Return Home
#define CMD_ENTRY_MODE_SET_OPTION1   0x04 // cursor [left], Display shift[none]
#define CMD_ENTRY_MODE_SET_OPTION2   0x05 // cursor [left], Display shift[]
#define CMD_ENTRY_MODE_SET_OPTION3   0x06 // cursor [right], Display shift[none]
#define CMD_ENTRY_MODE_SET_OPTION4   0x07 // cursor [right], Display shift[]
#define CMD_DISPLAY_OPTION1     0x08 // Display [Off], Cursor [Off], Cursor Blink [Off]
#define CMD_DISPLAY_OPTION2     0x09 // Display [Off], Cursor [Off], Cursor Blink [On]
#define CMD_DISPLAY_OPTION3     0x0A // Display [Off], Cursor [On], Cursor Blink [Off]
#define CMD_DISPLAY_OPTION4     0x0B // Display [Off], Cursor [On], Cursor Blink [On]
#define CMD_DISPLAY_OPTION5     0x0C // Display [On], Cursor [Off], Cursor Blink [Off]
#define CMD_DISPLAY_OPTION6     0x0D // Display [On], Cursor [Off], Cursor Blink [On]
#define CMD_DISPLAY_OPTION7     0x0E // Display [On], Cursor [On], Cursor Blink [Off]
#define CMD_DISPLAY_OPTION8     0x0F // Display [On], Cursor [On], Cursor Blink [On]
#define CMD_CURSOR_DISPLAY_SHIFT_OPTION1 0x10 // [Cursor] Select, Cursor Shift [Left]
#define CMD_CURSOR_DISPLAY_SHIFT_OPTION2 0x14 // [Cursor] Select, Cursor Shift [Right]
#define CMD_CURSOR_DISPLAY_SHIFT_OPTION3 0x18 // [Display] Select, Display Shift [Left]
#define CMD_CURSOR_DISPLAY_SHIFT_OPTION4 0x1C // [Display] Select, Display Shift [Right]
#define CMD_FUNCTION_SET_OPTION1   0x20 // [4]bit mode, [1] line, [5x8] Font
#define CMD_FUNCTION_SET_OPTION2   0x24 // [4]bit mode, [1] line, [5x11] Font
#define CMD_FUNCTION_SET_OPTION3   0x28 // [4]bit mode, [2] line, [5x8] Font
#define CMD_FUNCTION_SET_OPTION4   0x2C // [4]bit mode, [2] line, [5x11] Font
#define CMD_FUNCTION_SET_OPTION5   0x30 // [8]bit mode, [1] line, [5x8] Font
#define CMD_FUNCTION_SET_OPTION6   0x3C // [8]bit mode, [1] line, [5x11] Font
#define CMD_FUNCTION_SET_OPTION7   0x34 // [8]bit mode, [2] line, [5x8] Font
#define CMD_FUNCTION_SET_OPTION8   0x38 // [8]bit mode, [2] line, [5x11] Font

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define RS 0
#define RW 1
#define E 2
#define DATA PORTA

unsigned int time_count = 0; // 타이머 카운트
unsigned char sec, min, hour = 0; // 시,분,초 변수 설정

void LCD_INS (unsigned char com) // LCD Instruction 함수
{
  PORTC&=~(1<<RS); // RS bit을 0으로 SET
  PORTC&=~(1<<RW); // RW bit을 0으로 SET
  PORTC|=(1<<E); // E bit을 1으로 SET
  _delay_ms(50);
  DATA=com;
  _delay_ms(50);
  PORTC&=~(1<<E); // E bit을 1으로 SET
}

void LCD_WRITE_DATA (unsigned char com) // LCD DATA 쓰기 함수
{
  PORTC|=(1<<RS); // RS bit을 1으로 SET
  PORTC&=~(1<<RW); // RW bit을 0으로 SET
  PORTC|=(1<<E); // E bit을 1으로 SET
  _delay_ms(10);
  DATA=com;
  _delay_ms(10);
  PORTC&=~(1<<E); // E bit을 0으로 SET
}

void LCD_PST(unsigned char col, unsigned char row) // 문자열 디스플레이 행, 열 지정 함수
{
  LCD_INS(0x80|(row+col*0x40));
}

void str(unsigned char* com) // 문자열 함수
{
  while(*com!=0)
  {
    LCD_WRITE_DATA(*com);
    com++;
  }
}

void init_buffer(void) // LCD 버퍼 초기화 함수
{
  PORTC|=(1<<RS);
  PORTC|=(1<<RW);
  PORTC|=(1<<E);
  _delay_ms(10);
  
  while(DATA!=0)
  {
    LCD_WRITE_DATA(0);
    _delay_ms(10);
  }
}

int main(void) {

  TCNT0 = 0x06; // ---------------------> 1ms 만들기 위한 시스템 타이머 레지스터 설정
  TCCR0 = 0x04; // ----------------> freescale 64배수
  TIMSK = 0x01; // --------------> 오버플로우 인터럽트 사용
  SREG = 0x80; // --------------> 인터럽트 허용 레지스터

  DDRA=0xff; // PORTA 출력 설정
  DDRC=0xff; // PORTC 출력 설정

  PORTA=0x00; // PORTA 값 초기화
  PORTB=0x00; // PORTC 값 초기화

  LCD_INS(CMD_CREAR_DISPLAY); // Clear Display
  _delay_ms(2);

  LCD_INS(CMD_RETURN_HOME); // Return Home
  _delay_ms(2);

  LCD_INS(CMD_FUNCTION_SET_OPTION8); // [8]bit mode, [2] line, [5x8] Font
  _delay_ms(2);

  LCD_INS(CMD_DISPLAY_OPTION5); // Display [On], Cursor [Off], Cursor Blink [Off]
  _delay_ms(2);

  LCD_INS(CMD_CURSOR_DISPLAY_SHIFT_OPTION2); // [Cursor] Select, Cursor Shift [Right]
  _delay_ms(2);

  LCD_INS(CMD_ENTRY_MODE_SET_OPTION3); // cursor [right], Display shift[none] (커서 오른쪽증가, shift Off)
  _delay_ms(2);

  // 문자열 선언
  unsigned char str1[] = "Digital Clock";
  unsigned char time[] = "TIME   ";

  // 문자열 출력
  LCD_PST(0,0); // 1행 1열에 문자열 출력
  str(str1);

  LCD_PST(1,0); // 2행 1열에 문자열 출력
  str(time);

  // 00:00:00 출력 초기화
  LCD_PST(1,8);
  LCD_WRITE_DATA(0x30);
  LCD_PST(1,9);
  LCD_WRITE_DATA(0x30);
  LCD_PST(1,10);
  LCD_WRITE_DATA(0x3A);
  LCD_PST(1,11);
  LCD_WRITE_DATA(0x30);
  LCD_PST(1,12);
  LCD_WRITE_DATA(0x30);
  LCD_PST(1,13);
  LCD_WRITE_DATA(0x3A);
  LCD_PST(1,14);
  LCD_WRITE_DATA(0x30);
  LCD_PST(1,15);
  LCD_WRITE_DATA(0x30);

  while(1) {

  switch(sec/(60)) {

  case 0:    // sec != 60
  if (sec%10 != 0) // sec가 10의 배수가 아님
  {
  LCD_PST(1,15);
  LCD_WRITE_DATA(0x30 | (0x01*sec%10)); // 0x30에서 lower bit를 sec의 1의자리 숫자로 SET
  break;
  }
  else if (sec%10 == 0) // sec가 10의 배수임
  {
  LCD_PST(1,15);
  LCD_WRITE_DATA(0x30 | (0x01*sec%10)); // 0x30에서 lower bit를 sec의 1의자리 숫자로 SET

  LCD_PST(1,14);
  LCD_WRITE_DATA(0x30 | (0x01*sec/10)); // 0x30에서 lower bit를 sec의 10의자리 숫자로 SET
  }
  break;

  case 1: // sec == 60
  sec = 0; // sec는 0으로 리셋, xx:00 으로 초기화
  LCD_PST(1,15);
  LCD_WRITE_DATA(0x30);
  LCD_PST(1,14);
  LCD_WRITE_DATA(0x30);
  break;
  }

  switch(min/(60)) {

  case 0:  // min != 60
  if (min%10 != 0) // min가 10의 배수가 아님
  {
  LCD_PST(1,12);
  LCD_WRITE_DATA(0x30 | (0x01*min%10)); // 0x30에서 lower bit를 min의 1의자리 숫자로 SET
  break;
  }
  else if (sec%10 == 0) // min가 10의 배수임
  {
  LCD_PST(1,12);
  LCD_WRITE_DATA(0x30 | (0x01*min%10)); // 0x30에서 lower bit를 min의 1의자리 숫자로 SET

  LCD_PST(1,11);
  LCD_WRITE_DATA(0x30 | (0x01*min/10)); // 0x30에서 lower bit를 min의 10의자리 숫자로 SET
  }
  break;

  case 1: // min == 60
  min = 0; // min는 0으로 리셋, 00:xx 으로 초기화
  LCD_PST(1,12);
  LCD_WRITE_DATA(0x30);
  LCD_PST(1,11);
  LCD_WRITE_DATA(0x30);
  break;
  }

  switch(hour/(12)) {

  case 0:  // hour != 60
  if (hour%10 != 0) // hour가 10의 배수가 아님
  {
  LCD_PST(1,9);
  LCD_WRITE_DATA(0x30 | (0x01*hour%10)); // 0x30에서 lower bit를 hour의 1의자리 숫자로 SET
  break;
  }

  else if (sec%10 == 0) // hour가 10의 배수임
  {
  LCD_PST(1,9);
  LCD_WRITE_DATA(0x30 | (0x01*hour%10)); // 0x30에서 lower bit를 hour의 1의자리 숫자로 SET

  LCD_PST(1,8);
  LCD_WRITE_DATA(0x30 | (0x01*hour/10)); // 0x30에서 lower bit를 hour의 01의자리 숫자로 SET
  }
  break;

  case 1:  // hour != 60
  hour = 0;
  LCD_PST(1,9);
  LCD_WRITE_DATA(0x30);
  LCD_PST(1,8);
  LCD_WRITE_DATA(0x30);
  break;
  }
  }
  }

  ISR(TIMER0_OVF_vect) {
  TCNT0 = 6; // 256-6 = 250 count = 1ms
  if(++time_count == 1000) { // at 1s
  sec++; // 타이머 카운트 1000이 되면 sec 1증가
  time_count = 0; // 타이머 카운트 0으로 리셋
  if(sec==60) { // at 1min
  min++; // min 1 증가
  sec = 0; // sec 0으로 리셋
  if(min==60) { // at 1 hour
  hour++; // hour 1증가
  min = 0; // min 0으로 리셋
  if(hour==12) { // change to P.M or A.M
  hour = 0; // hour 0으로 리셋
  }
  }
  }
  }
}
