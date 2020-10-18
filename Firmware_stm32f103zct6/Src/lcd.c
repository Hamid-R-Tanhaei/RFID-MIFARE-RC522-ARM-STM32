#include "lcd.h"
void send_command(unsigned char data)
{
  HAL_Delay(2);
  HAL_GPIO_WritePin(CTRL_PORT,(1<<RS_PIN),GPIO_PIN_RESET);
  HAL_GPIO_WritePin(CTRL_PORT,(1<<RW_PIN),GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DATA_PORT,(0xFF<<D0_PIN_Start),GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DATA_PORT,(data<<D0_PIN_Start),GPIO_PIN_SET);
  HAL_GPIO_WritePin(CTRL_PORT,(1<<EN_PIN),GPIO_PIN_SET);
  HAL_GPIO_WritePin(CTRL_PORT,(1<<EN_PIN),GPIO_PIN_RESET);
}

void lcd_putchar(unsigned char data)
{
  HAL_Delay(1);
  HAL_GPIO_WritePin(CTRL_PORT,(1<<RS_PIN),GPIO_PIN_SET);
  HAL_GPIO_WritePin(CTRL_PORT,(1<<RW_PIN),GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DATA_PORT,(0xFF<<D0_PIN_Start),GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DATA_PORT,(data<<D0_PIN_Start),GPIO_PIN_SET);
  HAL_GPIO_WritePin(CTRL_PORT,(1<<EN_PIN),GPIO_PIN_SET);
  HAL_GPIO_WritePin(CTRL_PORT,(1<<EN_PIN),GPIO_PIN_RESET);
}

void lcd_init(void)
{
  HAL_Delay(2);
//  send_command(0x38);
  send_command(0x3C);
  send_command(0x06);
  send_command(0x0c);
}

void lcd_puts(char *str)
{
  HAL_Delay(1);
  while(*str != 0)
  {
	lcd_putchar(*str);
    str++;
  }
}
//
void lcd_puts_long(char *str)
{
	uint16_t cntr = 0;
  HAL_Delay(1);
  lcd_gotoxy(0, 0);
  while((*str != 0) && (cntr < 16)){
	cntr++;
    lcd_putchar(*str);
    str++;
  }
  //
  if(cntr >= 16){
	  lcd_gotoxy(0, 1);
	  while((*str != 0) && (cntr < 32)){
		cntr++;
	    lcd_putchar(*str);
	    str++;
	  }
  }
  if(cntr >= 32){
	  lcd_gotoxy(0, 2);
	  while((*str != 0) && (cntr < 48)){
		  cntr++;
		  lcd_putchar(*str);
		  str++;
	  }
  }
  //
  if(cntr >= 48){
	  lcd_gotoxy(0, 3);
	  while((*str != 0) && (cntr < 64)){
		  cntr++;
		  lcd_putchar(*str);
		  str++;
	  }
  }
}
//
void lcd_gotoxy(unsigned char x, unsigned char y)
{
  HAL_Delay(1);
 
    switch(y){
    case 0:
      send_command( 0x80 + x );
    break;
    case 1:
      send_command( 0xC0 + x );
      break;
    case 2:
      send_command( 0x90 + x );
      break;
    case 3:
      send_command( 0xD0 + x );
  }
}

void lcd_clear(void)
{
  HAL_Delay(1);
  send_command(0x01);
  send_command(0x02);
}

