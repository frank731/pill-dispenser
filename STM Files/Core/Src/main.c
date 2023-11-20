/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
struct Button {
	GPIO_TypeDef* port;
	uint16_t pin;
	uint32_t time_pressed;
	int state;
};

struct Motor {
	GPIO_TypeDef* IN1_PORT;
	GPIO_TypeDef* IN2_PORT;
	GPIO_TypeDef* IN3_PORT;
	GPIO_TypeDef* IN4_PORT;
	uint16_t IN1_PIN;
	uint16_t IN2_PIN;
	uint16_t IN3_PIN;
	uint16_t IN4_PIN;
};

// Function to convert minutes to 24-hour time format and return as a string
char* convertTo24HourFormat(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;

    // Allocate memory for the result string
    char* result = (char*)malloc(6); // HH:MM\0

    // Check if memory allocation was successful
    if (result == NULL) {
        printf("Memory allocation failed.\n");
        exit(1); // Exit with an error code
    }

    // Format the result string
    sprintf(result, "%02d:%02d", hours, mins);

    return result;
}

void microDelay (uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  while (__HAL_TIM_GET_COUNTER(&htim2) < delay);
}

void stepCCV (int steps, uint16_t delay, struct Motor m) // CCV - Counter Clockwise
{
  for(int x=0; x<steps; x=x+1)
  {
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_SET);   // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_SET);   // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_SET);   // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_SET);   // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_SET);   // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_SET);   // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_SET);   // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_SET);   // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_SET);   // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_SET);   // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_SET);   // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_SET);   // IN4
	  microDelay(delay);
  }
}

void stepCV (int steps, uint16_t delay, struct Motor m) // CV - Clockwise
{
  for(int x=0; x<steps; x=x+1)
  {
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_SET);   // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_SET);   // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_SET);   // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_SET);   // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_SET);   // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_SET);   // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_SET);   // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_SET);   // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_RESET); // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_SET);   // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_SET);   // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_SET);   // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);
	  HAL_GPIO_WritePin(m.IN1_PORT, m.IN1_PIN, GPIO_PIN_SET);   // IN1
	  HAL_GPIO_WritePin(m.IN2_PORT, m.IN2_PIN, GPIO_PIN_RESET); // IN2
	  HAL_GPIO_WritePin(m.IN3_PORT, m.IN3_PIN, GPIO_PIN_RESET); // IN3
	  HAL_GPIO_WritePin(m.IN4_PORT, m.IN4_PIN, GPIO_PIN_RESET); // IN4
	  microDelay(delay);

  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim2);

  struct Motor m1, m2;
  m1.IN1_PIN = GPIO_PIN_0;
  m1.IN1_PORT = GPIOA;
  m1.IN2_PIN = GPIO_PIN_1;
  m1.IN2_PORT = GPIOA;
  m1.IN3_PIN = GPIO_PIN_4;
  m1.IN3_PORT = GPIOA;
  m1.IN4_PIN = GPIO_PIN_0;
  m1.IN4_PORT = GPIOB;
  m2.IN1_PIN = GPIO_PIN_8;
  m2.IN1_PORT = GPIOB;
  m2.IN2_PIN = GPIO_PIN_9;
  m2.IN2_PORT = GPIOB;
  m2.IN3_PIN = GPIO_PIN_6;
  m2.IN3_PORT = GPIOA;
  m2.IN4_PIN = GPIO_PIN_7;
  m2.IN4_PORT = GPIOA;
  unsigned int pill_count = 0;
  unsigned int dispense_time = 0;
  unsigned int input_state = 0;

  Lcd_PortType ports[] = { D4_GPIO_Port, D5_GPIO_Port, D6_GPIO_Port, D7_GPIO_Port };
  Lcd_PinType pins[] = {D4_Pin, D5_Pin, D6_Pin, D7_Pin};
  Lcd_HandleTypeDef lcd;
  lcd = Lcd_create(ports, pins, RS_GPIO_Port, RS_Pin, EN_GPIO_Port, EN_Pin, LCD_4_BIT_MODE);
  Lcd_cursor(&lcd, 0, 1);
  Lcd_string(&lcd, "# to dispense:");
  Lcd_cursor(&lcd, 1, 7);
  Lcd_int(&lcd, pill_count);

  struct Button buttons[3];
  buttons[0].port = Left_Button_GPIO_Port;
  buttons[0].pin = Left_Button_Pin;
  buttons[1].port = Right_Button_GPIO_Port;
  buttons[1].pin = Right_Button_Pin;
  buttons[2].port = Rightmost_Button_GPIO_Port;
  buttons[2].pin = Rightmost_Button_Pin;
  for (int i = 0; i < 3; i++){
	  buttons[i].time_pressed = 0;
	  buttons[i].state = GPIO_PIN_SET;
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  for (int i = 0; i < 3; i++){
		  if(HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin) == GPIO_PIN_RESET){
			  if(buttons[i].state == GPIO_PIN_SET){
				  buttons[i].time_pressed = HAL_GetTick();
				  buttons[i].state = GPIO_PIN_RESET;
			  }
		  }
		  else{
			  if(buttons[i].state == GPIO_PIN_RESET){
				  if(HAL_GetTick() - buttons[i].time_pressed < 750){ //held less than 3 seconds
					  if(input_state == 0){
						  if(i == 0){
							  if(pill_count == 10){
								  Lcd_cursor(&lcd, 1, 8);
								  Lcd_string(&lcd, " ");
							  }
							  if(pill_count > 0) pill_count--;
							  Lcd_cursor(&lcd, 1, 7);
							  Lcd_int(&lcd, pill_count);
						  }
						  else if(i == 1){
							  if(pill_count < 10) pill_count++;
							  Lcd_cursor(&lcd, 1, 7);
							  Lcd_int(&lcd, pill_count);
						  }
						  else{
							  for(int i = 0; i < pill_count; i++){
								  stepCCV(256, 1000, m1);  // 256 half revolution
								  stepCCV(256, 1000, m2);
								  HAL_Delay(100);
								  stepCV(256, 1000, m2);
								  stepCV(256, 1000, m1);
							  }
						  }
					  }
					  else if(input_state == 1){
						  if(i == 0){
							  if(dispense_time > 0) dispense_time -= 30;
							  Lcd_cursor(&lcd, 1, 5);
							  Lcd_string(&lcd, convertTo24HourFormat(dispense_time));
						  }
						  else if(i == 1){
							  if(dispense_time < 1440) dispense_time += 30;
							  Lcd_cursor(&lcd, 1, 5);
							  Lcd_string(&lcd, convertTo24HourFormat(dispense_time));
						  }
					  }
				  }
				  else{
					  if(input_state == 0 && i == 2){
						  input_state++;
						  Lcd_clear(&lcd);
						  Lcd_cursor(&lcd, 0, 1);
						  Lcd_string(&lcd, "Dispense time:");
						  Lcd_cursor(&lcd, 1, 5);
						  Lcd_string(&lcd, convertTo24HourFormat(dispense_time));
					  }
				  }
				  buttons[i].state = GPIO_PIN_SET;
			  }
		  }
	  }
	  /*
	  if((HAL_GPIO_ReadPin(Rightmost_Button_GPIO_Port, Rightmost_Button_Pin) == GPIO_PIN_RESET) ||
			  (HAL_GPIO_ReadPin(Right_Button_GPIO_Port, Right_Button_Pin) == GPIO_PIN_RESET) ||
			  (HAL_GPIO_ReadPin(Left_Button_GPIO_Port, Left_Button_Pin) == GPIO_PIN_RESET)){
		  //HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	  }
	  else{
		  //HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	  }
	  */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
