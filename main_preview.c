/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @description 	: A demo of driving Mifare RC522 module using MFRC522 library
  * @author			: Hamid Reza Tanhaei
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include "lcd.h"
#include "stdio.h"
#include "delay.h"
#include "MFRC522.h"
//
uint8_t value = 0;
char str1[17]={'\0'};
char str2[17]={'\0'};
char str3[17]={'\0'};
char str4[17]={'\0'};
char tmp_str[65]={'\0'};
//
// Delay function:
#define	Precise_Delay(x) {\
	uint32_t x1 = x * 72;\
	DWT->CYCCNT = 0;\
	while (DWT->CYCCNT < x1);\
}
////
SPI_HandleTypeDef hspi2;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
char Read_KeyPad(void);
////
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  while(DWT_Delay_Init());
  lcd_init();
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, RESET);
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, RESET);
  lcd_clear();
  //
  for (int i = 0; i < 16; i++){
	  tmp_str[i] = 'A';
  }
  for (int i = 16; i < 32; i++){
	  tmp_str[i] = 'B';
  }
  for (int i = 32; i < 48; i++){
	  tmp_str[i] = 'C';
  }
  for (int i = 48; i < 64; i++){
	  tmp_str[i] = 'D';
  }
  lcd_puts_long(tmp_str);
  DWT_Delay_ms(2000);
  lcd_clear();
  DWT_Delay_ms(1000);
  HAL_GPIO_WritePin(RC522_Rst_GPIO_Port, RC522_Rst_Pin, GPIO_PIN_SET);
  HAL_Delay(100);
  MFRC522_Init();
  DWT_Delay_ms(1000);
  u_char status, cardstr[MAX_LEN+1];
  u_char card_data[17];
  uint32_t delay_val = 1000; //ms
  uint16_t result = 0;
  u_char UID[5];
  // a private key to scramble data writing/reading to/from RFID card:
  u_char Mx1[7][5]={{0x12,0x45,0xF2,0xA8},{0xB2,0x6C,0x39,0x83},{0x55,0xE5,0xDA,0x18},
		  	  	  	{0x1F,0x09,0xCA,0x75},{0x99,0xA2,0x50,0xEC},{0x2C,0x88,0x7F,0x3D}};
  u_char SectorKey[7];
//
  status = Read_MFRC522(VersionReg);
  sprintf(str1,"Running RC522");
  sprintf(str2,"ver:%x", status);
  lcd_clear();
  lcd_gotoxy(0,0);
  lcd_puts(str1);
  lcd_gotoxy(0,1);
  lcd_puts(str2);
  DWT_Delay_ms(1000);
  //
  while (1)
  {
	  LED1_GPIO_Port -> BSRR = LED1_Pin;
	  DWT_Delay_ms(4);
	  LED1_GPIO_Port -> BRR = LED1_Pin;
	  for (int i = 0; i < 16; i++) {
		  cardstr[i] = 0;
	  }
	  status = 0;
	  // Find cards
	  status = MFRC522_Request(PICC_REQIDL, cardstr);
	  if(status == MI_OK) {
		  LED2_GPIO_Port -> BSRR = LED2_Pin;
		  result = 0;
		  result++;
		  sprintf(str1,"Card:%x,%x,%x", cardstr[0], cardstr[1], cardstr[2]);
		  //
		  // Anti-collision, return card serial number == 4 bytes
		  DWT_Delay_ms(1);
		  status = MFRC522_Anticoll(cardstr);
		  if(status == MI_OK) {
			  result++;
			  sprintf(str2,"UID:%x %x %x %x", cardstr[0], cardstr[1], cardstr[2], cardstr[3]);
			  UID[0] = cardstr[0];
			  UID[1] = cardstr[1];
			  UID[2] = cardstr[2];
			  UID[3] = cardstr[3];
			  UID[4] = cardstr[4];
			  //
			  DWT_Delay_ms(1);
			  status = MFRC522_SelectTag(cardstr);
			  if (status > 0){
				  result++;
				  //
				  SectorKey[0] = ((Mx1[0][0])^(UID[0])) + ((Mx1[0][1])^(UID[1])) + ((Mx1[0][2])^(UID[2])) + ((Mx1[0][3])^(UID[3]));// 0x11; //KeyA[0]
				  SectorKey[1] = ((Mx1[1][0])^(UID[0])) + ((Mx1[1][1])^(UID[1])) + ((Mx1[1][2])^(UID[2])) + ((Mx1[1][3])^(UID[3]));// 0x11; //KeyA[0]
				  SectorKey[2] = ((Mx1[2][0])^(UID[0])) + ((Mx1[2][1])^(UID[1])) + ((Mx1[2][2])^(UID[2])) + ((Mx1[2][3])^(UID[3]));// 0x11; //KeyA[0]
				  SectorKey[3] = ((Mx1[3][0])^(UID[0])) + ((Mx1[3][1])^(UID[1])) + ((Mx1[3][2])^(UID[2])) + ((Mx1[3][3])^(UID[3]));// 0x11; //KeyA[0]
				  SectorKey[4] = ((Mx1[4][0])^(UID[0])) + ((Mx1[4][1])^(UID[1])) + ((Mx1[4][2])^(UID[2])) + ((Mx1[4][3])^(UID[3]));// 0x11; //KeyA[0]
				  SectorKey[5] = ((Mx1[5][0])^(UID[0])) + ((Mx1[5][1])^(UID[1])) + ((Mx1[5][2])^(UID[2])) + ((Mx1[5][3])^(UID[3]));// 0x11; //KeyA[0]
				  DWT_Delay_ms(1);
				  status = MFRC522_Auth(0x60, 3, SectorKey, cardstr);
				  if (status == MI_OK){
					  result++;
					  sprintf(str3, "Auth. OK");
					  if (HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin) == 0){
						  // Clean-Up the Card:
						  card_data[0] = 0xFF;
						  card_data[1] = 0xFF;
						  card_data[2] = 0xFF;
						  card_data[3] = 0xFF;
						  card_data[4] = 0xFF;
						  card_data[5] = 0xFF;
						  card_data[6] = 0xFF; //Access_bits[6]
						  card_data[7] = 0x07; //Access_bits[7]
						  card_data[8] = 0x80; //Access_bits[8]
						  card_data[9] = 0x88; //user_byte[9]
						  card_data[10] = 0x88; //user_byte[10]
						  card_data[11] = 0x88; //user_byte[11]
						  card_data[12] = 0x88; //user_byte[12]
						  card_data[13] = 0x88; //user_byte[13]
						  card_data[14] = 0x88; //user_byte[14]
						  card_data[15] = 0x88; //user_byte[15]
						  DWT_Delay_ms(1);
						  status = MFRC522_Write(3, card_data);
						  if(status == MI_OK) {
							  result++;
							  sprintf(str3, "                ");
							  sprintf(str4, "Card Cleared!");
							  delay_val = 2000;
						  }

					  }
				  }
				  else{
					  for (int i = 0; i < 16; i++) {cardstr[i] = 0;}
					  status = 0;
					  // Find cards
					  DWT_Delay_ms(1);
					  status = MFRC522_Request(PICC_REQIDL, cardstr);
					  DWT_Delay_ms(1);
					  status = MFRC522_Anticoll(cardstr);
					  DWT_Delay_ms(1);
					  status = MFRC522_SelectTag(cardstr);
					  SectorKey[0] = 0xFF;
					  SectorKey[1] = 0xFF;
					  SectorKey[2] = 0xFF;
					  SectorKey[3] = 0xFF;
					  SectorKey[4] = 0xFF;
					  SectorKey[5] = 0xFF;
					  DWT_Delay_ms(1);
					  status = MFRC522_Auth(0x60, 3, SectorKey, cardstr);
					  if (status == MI_OK){
						  if (HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin) == 0){
							  card_data[0] = ((Mx1[0][0])^(UID[0])) + ((Mx1[0][1])^(UID[1])) + ((Mx1[0][2])^(UID[2])) + ((Mx1[0][3])^(UID[3]));// 0x11; //KeyA[0]
							  card_data[1] = ((Mx1[1][0])^(UID[0])) + ((Mx1[1][1])^(UID[1])) + ((Mx1[1][2])^(UID[2])) + ((Mx1[1][3])^(UID[3]));// 0x11; //KeyA[0]
							  card_data[2] = ((Mx1[2][0])^(UID[0])) + ((Mx1[2][1])^(UID[1])) + ((Mx1[2][2])^(UID[2])) + ((Mx1[2][3])^(UID[3]));// 0x11; //KeyA[0]
							  card_data[3] = ((Mx1[3][0])^(UID[0])) + ((Mx1[3][1])^(UID[1])) + ((Mx1[3][2])^(UID[2])) + ((Mx1[3][3])^(UID[3]));// 0x11; //KeyA[0]
							  card_data[4] = ((Mx1[4][0])^(UID[0])) + ((Mx1[4][1])^(UID[1])) + ((Mx1[4][2])^(UID[2])) + ((Mx1[4][3])^(UID[3]));// 0x11; //KeyA[0]
							  card_data[5] = ((Mx1[5][0])^(UID[0])) + ((Mx1[5][1])^(UID[1])) + ((Mx1[5][2])^(UID[2])) + ((Mx1[5][3])^(UID[3]));// 0x11; //KeyA[0]
							  card_data[6] = 0xFF; //Access_bits[6]
							  card_data[7] = 0x07; //Access_bits[7]
							  card_data[8] = 0x80; //Access_bits[8]
							  card_data[9] = 0x88; //user_byte[9]
							  card_data[10] = 0x88; //user_byte[10]
							  card_data[11] = 0x88; //user_byte[11]
							  card_data[12] = 0x88; //user_byte[12]
							  card_data[13] = 0x88; //user_byte[13]
							  card_data[14] = 0x88; //user_byte[14]
							  card_data[15] = 0x88; //user_byte[15]
							  DWT_Delay_ms(1);
							  status = MFRC522_Write(3, card_data);
							  if(status == MI_OK) {
								  result++;
								  sprintf(str3, "Card Set!");
								  delay_val = 2000;
							  }
						  }
						  else{

							  sprintf(str4, "New Card!");
						  }
					  }
					  else if (status != MI_OK){
						  sprintf(str3, "Auth. Error");
					  }
				  }
				  DWT_Delay_ms(1);
				  MFRC522_StopCrypto1();
			  }
		  }
		  DWT_Delay_ms(1);
		  MFRC522_Halt();
		  LED2_GPIO_Port -> BRR = LED2_Pin;
		  lcd_clear();
		  lcd_gotoxy(0,0);
		  lcd_puts(str1);
		  lcd_gotoxy(0,1);
		  lcd_puts(str2);
		  lcd_gotoxy(0,2);
		  lcd_puts(str3);
		  lcd_gotoxy(0,3);
		  lcd_puts(str4);
		  DWT_Delay_ms(delay_val);
		  delay_val = 1000;
		  sprintf(str1, "                ");
		  sprintf(str2, "                ");
		  sprintf(str3, "                ");
		  sprintf(str4, "                ");
	  }
	  else{
		  sprintf(str1, "Waiting for Card");
		  lcd_gotoxy(0,0);
		  lcd_puts(str1);
		  sprintf(str2, "                ");
		  lcd_gotoxy(0,1);
		  lcd_puts(str2);
		  sprintf(str3, "                ");
		  lcd_gotoxy(0,2);
		  lcd_puts(str3);
		  sprintf(str4, "                ");
		  lcd_gotoxy(0,3);
		  lcd_puts(str4);
	  }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SIM_RST_GPIO_Port, SIM_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, RC522_Rst_Pin|RC522_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, KpadRow1_Pin|KpadRow2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, KpadRow3_Pin|KpadRow4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PF0 PF1 PF2 PF3 
                           PF4 PF5 PF6 PF7 
                           PF8 PF9 PF10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : SIM_RST_Pin */
  GPIO_InitStruct.Pin = SIM_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SIM_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RC522_Rst_Pin RC522_CS_Pin */
  GPIO_InitStruct.Pin = RC522_Rst_Pin|RC522_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : Key1_Pin Key2_Pin */
  GPIO_InitStruct.Pin = Key1_Pin|Key2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : KpadRow1_Pin KpadRow2_Pin */
  GPIO_InitStruct.Pin = KpadRow1_Pin|KpadRow2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : KpadRow3_Pin KpadRow4_Pin */
  GPIO_InitStruct.Pin = KpadRow3_Pin|KpadRow4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : KpadCol1_Pin KpadCol2_Pin KpadCol3_Pin KpadCol4_Pin */
  GPIO_InitStruct.Pin = KpadCol1_Pin|KpadCol2_Pin|KpadCol3_Pin|KpadCol4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2_Pin */
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DipSw5_Pin DipSw4_Pin DipSw3_Pin */
  GPIO_InitStruct.Pin = DipSw5_Pin|DipSw4_Pin|DipSw3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : DipSw2_Pin */
  GPIO_InitStruct.Pin = DipSw2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DipSw2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
char Read_KeyPad(void){
	char key_value = ' ';
	//
	KpadRow1_GPIO_Port -> BRR = KpadRow1_Pin;
	HAL_Delay(1);
	if((KpadCol1_GPIO_Port->IDR & KpadCol1_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '1';
	if((KpadCol2_GPIO_Port->IDR & KpadCol2_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '2';
	if((KpadCol3_GPIO_Port->IDR & KpadCol3_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '3';
	KpadRow1_GPIO_Port -> BSRR = KpadRow1_Pin;
	//
	KpadRow2_GPIO_Port -> BRR = KpadRow2_Pin;
	HAL_Delay(1);
	if((KpadCol1_GPIO_Port->IDR & KpadCol1_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '4';
	if((KpadCol2_GPIO_Port->IDR & KpadCol2_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '5';
	if((KpadCol3_GPIO_Port->IDR & KpadCol3_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '6';
	KpadRow2_GPIO_Port -> BSRR = KpadRow2_Pin;
	//
	KpadRow3_GPIO_Port -> BRR = KpadRow3_Pin;
	HAL_Delay(1);
	if((KpadCol1_GPIO_Port->IDR & KpadCol1_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '7';
	if((KpadCol2_GPIO_Port->IDR & KpadCol2_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '8';
	if((KpadCol3_GPIO_Port->IDR & KpadCol3_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '9';
	KpadRow3_GPIO_Port -> BSRR = KpadRow3_Pin;
	//
	KpadRow4_GPIO_Port -> BRR = KpadRow4_Pin;
	HAL_Delay(1);
	if((KpadCol1_GPIO_Port->IDR & KpadCol1_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '*';
	if((KpadCol2_GPIO_Port->IDR & KpadCol2_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '0';
	if((KpadCol3_GPIO_Port->IDR & KpadCol3_Pin) == (uint32_t)GPIO_PIN_RESET)
		key_value = '#';
	KpadRow4_GPIO_Port -> BSRR = KpadRow4_Pin;
	//
	return	key_value;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
