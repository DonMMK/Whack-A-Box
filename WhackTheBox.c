//n10496262 micro controller assignment
//References and Sources of code mentioned in report

//Standard library
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>

#define SET_BIT(reg, pin)     (reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin)     (reg) &= ~(1 << (pin))
#define WRITE_BIT(reg, pin, value)  (reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
#define BIT_VALUE(reg, pin)     (((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin)    (BIT_VALUE((reg),(pin))==1)

//  https://www.tinkercad.com/things/hGYL4k0Uoal

// --== WIRING ==--
// LCD GND  -> GND
// LCD VCC  -> 5V
// LCD V0   -> GND
// LCD RW   -> GND
// LCD LED Anode    -> 220 Ohm -> 5V
// LCD LED Cathode  -> GND

#define LCD_USING_4PIN_MODE (1)

#define LCD_DATA4_DDR (DDRD)
#define LCD_DATA5_DDR (DDRD)
#define LCD_DATA6_DDR (DDRD)
#define LCD_DATA7_DDR (DDRD)

#define LCD_DATA4_PORT (PORTD)
#define LCD_DATA5_PORT (PORTD)
#define LCD_DATA6_PORT (PORTD)
#define LCD_DATA7_PORT (PORTD)

#define LCD_DATA4_PIN (4)
#define LCD_DATA5_PIN (5)
#define LCD_DATA6_PIN (6)
#define LCD_DATA7_PIN (7)

#define LCD_RS_DDR (DDRB)
#define LCD_ENABLE_DDR (DDRB)

#define LCD_RS_PORT (PORTB)
#define LCD_ENABLE_PORT (PORTB)

#define LCD_RS_PIN (1)
#define LCD_ENABLE_PIN (0)

//DATASHEET: https://s3-us-west-1.amazonaws.com/123d-circuits-datasheets/uploads%2F1431564901240-mni4g6oo875bfbt9-6492779e35179defaf4482c7ac4f9915%2FLCD-WH1602B-TMI.pdf

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define BAUD 9600
#define F_CPU 16000000UL
#define MYUBRR (F_CPU/16/BAUD -1)
#define FREQ 16000000.0
#define PRESCALE 1024.0

void lcd_init(void);
void lcd_write_string(uint8_t x, uint8_t y, char string[]);
void lcd_write_char(uint8_t x, uint8_t y, char val);

void lcd_clear(void);
void lcd_home(void);

void lcd_createChar(uint8_t, uint8_t[]);
void lcd_setCursor(uint8_t, uint8_t);

void lcd_noDisplay(void);
void lcd_display(void);
void lcd_noBlink(void);
void lcd_blink(void);
void lcd_noCursor(void);
void lcd_cursor(void);
void lcd_leftToRight(void);
void lcd_rightToLeft(void);
void lcd_autoscroll(void);
void lcd_noAutoscroll(void);
void scrollDisplayLeft(void);
void scrollDisplayRight(void);

size_t lcd_write(uint8_t);
void lcd_command(uint8_t);

void lcd_send(uint8_t, uint8_t);
void lcd_write4bits(uint8_t);
void lcd_write8bits(uint8_t);
void lcd_pulseEnable(void);

void timer0(void);
void timer1(void);
void timer2(void);

uint8_t _lcd_displayfunction;
uint8_t _lcd_displaycontrol;
uint8_t _lcd_displaymode;

// END Definitions


#define BTNDDR      DDRC
#define BTNPIN      PINC
#define A_BTN    2
#define B_BTN    3
#define C_BTN    4
#define D_BTN    5

#define LEDDDR      DDRB
#define LEDPORT     PORTB
#define LED_A     2
#define LED_B     3
#define LED_C     4
#define LED_D     5

#define SCREEN_HEIGHT 2
#define SCREEN_WIDTH 16

#define BOXA    0b11111100
#define BOXB    0b11111100
#define BOXC    0b11111100
#define BOXD    0b11111100
#define ARROWPR 0b11000111 //>>>>
#define ARROWPL 0b11001000 //<<<<
#define A       0b01000001
#define B       0b01000010
#define C       0b01000011
#define D       0b01000100

#define FAST   20
#define MEDIUM 50
#define SLOW   80


void setup_lcd(void);
void setup_IO(void);
void process(void);
void move_character(uint8_t x, uint8_t y, uint8_t character);

void game(void);

//global variables
static uint8_t BOX1x = 4;
static uint8_t BOX1y = 0;

static uint8_t BOX2x = 11;
static uint8_t BOX2y = 0;

static uint8_t BOX3x = 4;
static uint8_t BOX3y = 1;

static uint8_t BOX4x = 11;
static uint8_t BOX4y = 1;

static uint8_t arrowplx = 6;
static uint8_t arrowply = 0;

static uint8_t arrowprx = 9;
static uint8_t arrowpry = 0;

static uint8_t Ax = 3;
static uint8_t Ay = 0;

static uint8_t Bx = 12;
static uint8_t By = 0;

static uint8_t Cx = 3;
static uint8_t Cy = 1;

static uint8_t Dx = 12;
static uint8_t Dy = 1;

int whack_score = 0;
int reaction_time = 0;


int main(void){
  setup_lcd();
  setup_IO();
  //timer0();
  //timer1();
  //timer2();

  while(1){
    process();
    _delay_ms(500);
    lcd_clear();
  }
}


void setup_lcd(void) {
    lcd_init();
/*
      lcd_write_string(0,0, "  Welcome to Whack the Box");
      for(int i = 0; i< 9; i++){
        scrollDisplayLeft();
        _delay_ms(200);
      }
      lcd_clear();
      _delay_ms(2000);
      lcd_clear();

      lcd_write_string(0,0,"Please Select");
      lcd_write_string(0,1,"Speed");
      _delay_ms(2000);
      lcd_clear();
      _delay_ms(1000);

      lcd_write_string(0,0,"Start Wacking!!!");

      _delay_ms(2000);
      lcd_clear();
      _delay_ms(1000);
*/
      SET_BIT(LEDPORT,LED_D);
}


void setup_IO(void){

    CLEAR_BIT(BTNDDR,A_BTN);
    CLEAR_BIT(BTNDDR,B_BTN);
    CLEAR_BIT(BTNDDR,C_BTN);
    CLEAR_BIT(BTNDDR,D_BTN);

    SET_BIT(LEDDDR,LED_A);
    SET_BIT(LEDDDR,LED_B);
    SET_BIT(LEDDDR,LED_C);
    SET_BIT(LEDDDR,LED_D);

}

void process(void) {

if ((BIT_IS_SET(LEDPORT,LED_D)) == 1) {

    move_character(BOX1x,BOX1y, BOXA);
    move_character(BOX2x,BOX2y, BOXB);
    move_character(BOX3x,BOX3y, BOXC);
    move_character(BOX4x,BOX4y, BOXD);

    move_character(Ax,Ay,A);
    move_character(Bx,By,B);
    move_character(Cx,Cy,C);
    move_character(Dx,Dy,D);

    if (arrowply == 0) {

      if (arrowplx == 6) {
        move_character(arrowplx,arrowply, ARROWPL);
        _delay_ms(1000);

      if (!BIT_VALUE(BTNPIN,A_BTN) == 0) {
        CLEAR_BIT(LEDPORT,LED_D);
        lcd_write_string(arrowplx,arrowply," ");
        arrowply = arrowply + 1;
        whack_score += 10;
        SET_BIT(LEDPORT,LED_A);
        _delay_ms(100);
        CLEAR_BIT(LEDPORT,LED_A);
      }

      if (!BIT_VALUE(BTNPIN,A_BTN) == 1) {
        _delay_ms(300);
        lcd_clear();
        lcd_write_string(0,0,"You missed!!!");
        _delay_ms(2000);
      }
  }
}

  if (arrowply == 1 && arrowplx == 6) {
      move_character(arrowplx,arrowply, ARROWPL);
      _delay_ms(1000);

    if (!BIT_VALUE(BTNPIN,C_BTN) == 0) {
      lcd_write_string(arrowplx,arrowply," ");
      arrowply = arrowply - 1;
      whack_score += 10;
      SET_BIT(LEDPORT,LED_C);
      _delay_ms(100);
      CLEAR_BIT(LEDPORT,LED_C);
    }

    if (!BIT_VALUE(BTNPIN,C_BTN) == 1) {
      _delay_ms(300);
      lcd_clear();
      lcd_write_string(0,0,"You missed!!!");
      _delay_ms(2000);
    }
}
}
}
  //_delay_ms(1000);
/*
  if (arrowpry == 0 && arrowplx == 11) {
      move_character(arrowprx,arrowpry, ARROWPR);
      _delay_ms(1000);

    if (BIT_VALUE(BTNPIN,B_BTN) == 1) {
      lcd_write_string(arrowprx,arrowpry," ");
      arrowpry = arrowpry + 1;
      whack_score += 10;
      SET_BIT(LEDPORT,LED_B);
      _delay_ms(100);
      CLEAR_BIT(LEDPORT,LED_B);
    }

    if (BIT_VALUE(BTNPIN,B_BTN) == 0) {
      _delay_ms(300);
      lcd_clear();
      lcd_write_string(0,0,"You missed!!!");
      _delay_ms(2000);
    }
}

_delay_ms(1000);

if (arrowpry == 1 && arrowplx == 11) {
    move_character(arrowprx,arrowpry, ARROWPR);
    _delay_ms(1000);

  if (BIT_VALUE(BTNPIN,D_BTN) == 1) {
    lcd_write_string(arrowprx,arrowpry," ");
    arrowpry = arrowpry - 1;
    whack_score += 10;
    SET_BIT(LEDPORT,LED_D);
    _delay_ms(100);
    CLEAR_BIT(LEDPORT,LED_D);
  }

  if (BIT_VALUE(BTNPIN,D_BTN) == 0) {
    _delay_ms(300);
    lcd_clear();
    lcd_write_string(0,0,"You missed!!!");
    _delay_ms(2000);
  }
}

_delay_ms(1000);
*/




 /*
void timer0(void){ //PWM
   DDRD |= (1 << PD3);
   // PD3 is now an output

    OCR0A = 128;
    // set PWM for 50% duty cycle

    TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    // TinkerCAD Errata: timer clocking must be enabled before WGM
    // set prescaler to 8 and starts PWM
    TCCR0B = (1 << CS01);

    TCCR0A |= (1 << WGM02) | (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

}
*/
//void timer1(void){ //debouncing
//}

/*
volatile uint8_t bit_counter = 0;
volatile uint8_t pressed = 0;

ISR(TIMER1_OVF_vect){

    bit_counter = (bit_counter << 1);
    uint8_t mask = 0b00011111;
    bit_counter &= mask;
  WRITE_BIT(bit_counter, 0, BIT_VALUE(BTNPIN,A_BTN));

  if (bit_counter == mask){
    pressed = 1;
  }

  else if (bit_counter == 0){
    pressed = 0;
  }
}

*/
void timer2(void){ //reaction time
  TCCR2A = 0;

  //prescaler of 8
  SET_BIT(TCCR2B,CS21);

  //timer overflow
  SET_BIT(TIMSK2,TOIE2);

  //global interrupts
  sei();
}

/* ********************************************/
// START LIBRARY FUNCTIOMNS

void move_character(uint8_t x, uint8_t y, uint8_t character){
    lcd_setCursor(x,y);
    lcd_write(character);
}

void lcd_init(void){
  //dotsize
  if (LCD_USING_4PIN_MODE){
    _lcd_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  } else {
    _lcd_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
  }

  _lcd_displayfunction |= LCD_2LINE;

  // RS Pin
  LCD_RS_DDR |= (1 << LCD_RS_PIN);
  // Enable Pin
  LCD_ENABLE_DDR |= (1 << LCD_ENABLE_PIN);

  #if LCD_USING_4PIN_MODE
    //Set DDR for all the data pins
    LCD_DATA4_DDR |= (1 << 4);
    LCD_DATA5_DDR |= (1 << 5);
    LCD_DATA6_DDR |= (1 << 6);
    LCD_DATA7_DDR |= (1 << 7);

  #else
    //Set DDR for all the data pins
    LCD_DATA0_DDR |= (1 << LCD_DATA0_PIN);
    LCD_DATA1_DDR |= (1 << LCD_DATA1_PIN);
    LCD_DATA2_DDR |= (1 << LCD_DATA2_PIN);
    LCD_DATA3_DDR |= (1 << LCD_DATA3_PIN);
    LCD_DATA4_DDR |= (1 << LCD_DATA4_PIN);
    LCD_DATA5_DDR |= (1 << LCD_DATA5_PIN);
    LCD_DATA6_DDR |= (1 << LCD_DATA6_PIN);
    LCD_DATA7_DDR |= (1 << LCD_DATA7_PIN);
  #endif

  // SEE PAGE 45/46 OF Hitachi HD44780 DATASHEET FOR INITIALIZATION SPECIFICATION!

  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  _delay_us(50000);
  // Now we pull both RS and Enable low to begin commands (R/W is wired to ground)
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);

  //put the LCD into 4 bit or 8 bit mode
  if (LCD_USING_4PIN_MODE) {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    lcd_write4bits(0b0111);
    _delay_us(4500); // wait min 4.1ms

    // second try
    lcd_write4bits(0b0111);
    _delay_us(4500); // wait min 4.1ms

    // third go!
    lcd_write4bits(0b0111);
    _delay_us(150);

    // finally, set to 4-bit interface
    lcd_write4bits(0b0010);
  } else {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
    _delay_us(4500);  // wait more than 4.1ms

    // second try
    lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
    _delay_us(150);

    // third go
    lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
  }

  // finally, set # lines, font size, etc.
  lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);

  // turn the display on with no cursor or blinking default
  _lcd_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  lcd_display();

  // clear it off
  lcd_clear();

  // Initialize to default text direction (for romance languages)
  _lcd_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}


/********** high level commands, for the user! */
void lcd_write_string(uint8_t x, uint8_t y, char string[]){
  lcd_setCursor(x,y);
  for(int i=0; string[i]!='\0'; ++i){
    lcd_write(string[i]);
  }
}

void lcd_write_char(uint8_t x, uint8_t y, char val){
  lcd_setCursor(x,y);
  lcd_write(val);
}

void lcd_clear(void){
  lcd_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  _delay_us(2000);  // this command takes a long time!
}

void lcd_home(void){
  lcd_command(LCD_RETURNHOME);  // set cursor position to zero
  _delay_us(2000);  // this command takes a long time!
}


// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  lcd_command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    lcd_write(charmap[i]);
  }
}


void lcd_setCursor(uint8_t col, uint8_t row){
  if ( row >= 2 ) {
    row = 1;
  }

  lcd_command(LCD_SETDDRAMADDR | (col + row*0x40));
}

// Turn the display on/off (quickly)
void lcd_noDisplay(void) {
  _lcd_displaycontrol &= ~LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_display(void) {
  _lcd_displaycontrol |= LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// Turns the underline cursor on/off
void lcd_noCursor(void) {
  _lcd_displaycontrol &= ~LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_cursor(void) {
  _lcd_displaycontrol |= LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// Turn on and off the blinking cursor
void lcd_noBlink(void) {
  _lcd_displaycontrol &= ~LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_blink(void) {
  _lcd_displaycontrol |= LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcd_leftToRight(void) {
  _lcd_displaymode |= LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This is for text that flows Right to Left
void lcd_rightToLeft(void) {
  _lcd_displaymode &= ~LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This will 'right justify' text from the cursor
void lcd_autoscroll(void) {
  _lcd_displaymode |= LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This will 'left justify' text from the cursor
void lcd_noAutoscroll(void) {
  _lcd_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

/*********** mid level commands, for sending data/cmds */

inline void lcd_command(uint8_t value) {
  //
  lcd_send(value, 0);
}

inline size_t lcd_write(uint8_t value) {
  lcd_send(value, 1);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void lcd_send(uint8_t value, uint8_t mode) {
  //RS Pin
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  LCD_RS_PORT |= (!!mode << LCD_RS_PIN);

  if (LCD_USING_4PIN_MODE) {
    lcd_write4bits(value>>4);
    lcd_write4bits(value);
  } else {
    lcd_write8bits(value);
  }
}

void lcd_pulseEnable(void) {
  //Enable Pin
  LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);
  _delay_us(1);
  LCD_ENABLE_PORT |= (1 << LCD_ENABLE_PIN);
  _delay_us(1);    // enable pulse must be >450ns
  LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);
  _delay_us(100);   // commands need > 37us to settle
}

void lcd_write4bits(uint8_t value) {
  //Set each wire one at a time

  LCD_DATA4_PORT &= ~(1 << LCD_DATA4_PIN);
  LCD_DATA4_PORT |= ((value & 1) << LCD_DATA4_PIN);
  value >>= 1;

  LCD_DATA5_PORT &= ~(1 << LCD_DATA5_PIN);
  LCD_DATA5_PORT |= ((value & 1) << LCD_DATA5_PIN);
  value >>= 1;

  LCD_DATA6_PORT &= ~(1 << LCD_DATA6_PIN);
  LCD_DATA6_PORT |= ((value & 1) << LCD_DATA6_PIN);
  value >>= 1;

  LCD_DATA7_PORT &= ~(1 << LCD_DATA7_PIN);
  LCD_DATA7_PORT |= ((value & 1) << LCD_DATA7_PIN);

  lcd_pulseEnable();
}

void lcd_write8bits(uint8_t value) {
  //Set each wire one at a time

  #if !LCD_USING_4PIN_MODE
    LCD_DATA0_PORT &= ~(1 << LCD_DATA0_PIN);
    LCD_DATA0_PORT |= ((value & 1) << LCD_DATA0_PIN);
    value >>= 1;

    LCD_DATA1_PORT &= ~(1 << LCD_DATA1_PIN);
    LCD_DATA1_PORT |= ((value & 1) << LCD_DATA1_PIN);
    value >>= 1;

    LCD_DATA2_PORT &= ~(1 << LCD_DATA2_PIN);
    LCD_DATA2_PORT |= ((value & 1) << LCD_DATA2_PIN);
    value >>= 1;

    LCD_DATA3_PORT &= ~(1 << LCD_DATA3_PIN);
    LCD_DATA3_PORT |= ((value & 1) << LCD_DATA3_PIN);
    value >>= 1;

    LCD_DATA4_PORT &= ~(1 << LCD_DATA4_PIN);
    LCD_DATA4_PORT |= ((value & 1) << LCD_DATA4_PIN);
    value >>= 1;

    LCD_DATA5_PORT &= ~(1 << LCD_DATA5_PIN);
    LCD_DATA5_PORT |= ((value & 1) << LCD_DATA5_PIN);
    value >>= 1;

    LCD_DATA6_PORT &= ~(1 << LCD_DATA6_PIN);
    LCD_DATA6_PORT |= ((value & 1) << LCD_DATA6_PIN);
    value >>= 1;

    LCD_DATA7_PORT &= ~(1 << LCD_DATA7_PIN);
    LCD_DATA7_PORT |= ((value & 1) << LCD_DATA7_PIN);

    lcd_pulseEnable();
  #endif
}
