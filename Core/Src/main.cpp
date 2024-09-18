/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "rosserial.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Propulsion_Sys/propulsion_sys.h"
#include "Datatype/dynamics.h"
#include "robot_arm.h"
#include "Sensor/mpu9250.h"
#include <stdio.h>
#include "dvl_reader.h"
#include "read_data.h"
#include "controller.h"
#include "Sensor/bar02.h"
/* USER CODE END Includes */


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Interrupt_Handle();

// robot arm
extern int Servo_State;
extern int CurrentMotor_State;
int AdjustSpeed = 30;  // PWM(microsecond): AdjustSpeed+1500 for current motor

// KillSwitch operate variable
int operate = 1;

//  error data for position: x,y; velocity: x, y, z; 
extern geometry::Vector ex;     // position error
extern geometry::Vector ev;     // velocity error
extern geometry::Vector eR;     // angle    error  
extern geometry::Vector eOmega; // angular velocity error  

double depth = 0;
extern float desired_depth;  //desired depth

//  coefficient K for controller 
geometry::Vector Kx = {0.6, 0.6, 1};
geometry::Vector Kv = {0, 0, 0};
geometry::Vector KR = {0.004, 0.0028, 0.014};
geometry::Vector KOmega = {0, 0, 0};

// other variables
int done = 0;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /**********************/
    /**  Initialization  **/
    /**********************/
  
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();
    /* Initialize all configured peripherals */
    MX_DMA_Init();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_SPI2_Init();
    MX_TIM2_Init();
    MX_TIM4_Init();
    MX_TIM8_Init();
    MX_USART3_UART_Init();
    MX_UART5_Init();
    // rosserial communication 
    rosserial_init();
    rosserial_subscribe(); 
    

    // Sensor Initialization
    Mpu9250 imu_no_use;   // the old imu no use 
    imu_no_use.set(&hspi2, GPIOB, GPIO_PIN_12);

    Bar02 depth_sensor;   // pressure senser
    if (!depth_sensor.set(&hi2c1))
        Interrupt_Handle();
    
    // Controller Initialization 
    Controller controller(Kx, Kv, KR, KOmega); 

    // Propulsion system Initialization
    //For Motor Output
    Propulsion_Sys propulsion_sys(&htim2, &htim8);
    
    //Robot Arm
    Robot_Arm arm(&htim4, AdjustSpeed+1500); // unit: PWM (ms)
    //Wait for motor to setup
    HAL_Delay(3000);


    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (!done)
    {
      
      //Depth Sensor
      depth_sensor.read_value(); // calculate the pressure internally
      depth = depth_sensor.depth(); // calculate the depth via offset
      ex.z = desired_depth - depth; // the error to the desired depth
      //ex.z = 0; // (Use when depth sensor stop working)
      
      //Controller
      // input ex, ev, eR, eOmega; output control_input
      // Control inputs for propulsion system
      // force: x, y, z; moment: x, y, z
      Kinematics control_input = controller.update(ex, ev, eR, eOmega);
      
      // KillSwitch read the "operate" value
      // operate 0 stop the motor
      //         1 run  the motor
      operate = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);

      /**  Underwater Motors control  **/ 
      // Allocate and Output
      propulsion_sys.allocate(control_input);  //T200 Motor Output

      /**  Servo control  **/
      /*if(Servo_State){
        arm.Servo_SetAngle(1,0);   // (number of Servo, unit: degree)
      }
      else{
        arm.Servo_SetAngle(1, 85); // (number of Servo, unit: degree)
      }*/

      /**  CurrentMotor contrl  **/
      //CurrentMotor_State = 1;
      //arm.CurrentMotor_SetMode(CurrentMotor_State);
      arm.CurrentMotor_Test();

      // publisher topic: stm32_to_rpi 
      rosserial_publish(control_input.angular.x, control_input.angular.y, 0, depth);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}


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

void Interrupt_Handle()
{
  done = 1;
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
