#ifndef _LCD_H
#define _LCD_H

#include "stm32f1xx_hal.h"
#define DATA_PORT  GPIOF
#define CTRL_PORT  GPIOF
#define D0_PIN_Start  0
#define RS_PIN  10
#define RW_PIN  9
#define EN_PIN   8


void lcd_init(void);
void lcd_clear(void);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_puts(char *str);
void lcd_puts_long(char *str);
void lcd_putchar(unsigned char data);

#endif
