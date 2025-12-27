/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "math.h"
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
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_i2s2_ext_rx;
DMA_HandleTypeDef hdma_spi2_tx;

/* USER CODE BEGIN PV */
// 0 = no gain reduction
// 1 = attack
// 2 = gain reduction
// 3 = release
enum CompStates {
	S_NoOperation, S_Attack, S_GainReduction, S_Release
};
enum CompStates State = S_NoOperation;

int32_t Attack, Release, Hold, TimeOut;
float gainreduce, gain_step_attack, gain_step_release, gain, threshold;

uint16_t rxBuf[8];
uint16_t txBuf[8];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_DAC_Init(void);
static void MX_I2S2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */
	// 1 sample = 1/96kHz = ~10us
	//Attack -> 30 ms -> 3000
	//Release -> 20 ms -> 2000
	//Hold -> 10ms -> 1000
	Attack = 3000;
	Release = 2000;
	Hold = 1000;

	//threshold -20dB below limit -> 0.1 * 2^31
	threshold = 0.1f * 2147483648;

	//compression ratio: 6:1 -> -6dB = 0.5
	gainreduce = 0.5f;

	gain_step_attack = (1.0f - gainreduce) / Attack;
	gain_step_release = (1.0f - gainreduce) / Release;

	//initial gain = 1.0 -> no compression
	gain = 1.0f;
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
	MX_DMA_Init();
	MX_DAC_Init();
	MX_I2S2_Init();
	/* USER CODE BEGIN 2 */
	HAL_I2SEx_TransmitReceive_DMA(&hi2s2, txBuf, rxBuf, 4);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief DAC Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC_Init(void) {

	/* USER CODE BEGIN DAC_Init 0 */

	/* USER CODE END DAC_Init 0 */

	DAC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN DAC_Init 1 */

	/* USER CODE END DAC_Init 1 */

	/** DAC Initialization
	 */
	hdac.Instance = DAC;
	if (HAL_DAC_Init(&hdac) != HAL_OK) {
		Error_Handler();
	}

	/** DAC channel OUT1 config
	 */
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN DAC_Init 2 */

	/* USER CODE END DAC_Init 2 */

}

/**
 * @brief I2S2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2S2_Init(void) {

	/* USER CODE BEGIN I2S2_Init 0 */

	/* USER CODE END I2S2_Init 0 */

	/* USER CODE BEGIN I2S2_Init 1 */

	/* USER CODE END I2S2_Init 1 */
	hi2s2.Instance = SPI2;
	hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
	hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B_EXTENDED;
	hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
	hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_96K;
	hi2s2.Init.CPOL = I2S_CPOL_LOW;
	hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
	hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
	if (HAL_I2S_Init(&hi2s2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2S2_Init 2 */

	/* USER CODE END I2S2_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	/* DMA1_Stream4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
	/* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int DoCompressor(int inSample) {
	float inSampleF = (float) inSample;

	if (fabs(inSampleF) > threshold) {
		if (gain >= gainreduce) {
			if (State == S_NoOperation) {
				State = S_Attack;
				TimeOut = Attack;
			} else if (State == S_Release) {
				State = S_Attack;
				TimeOut = Attack;
			}
		}
		if (State == S_GainReduction)
			TimeOut = Hold;

	}

	if (fabs(inSampleF) < threshold && gain <= 1.0f) {
		if (TimeOut == 0 && State == S_GainReduction) {
			State = S_Release;
			TimeOut = Release;
		}
	}
	switch (State) {

	case S_Attack:
		if (TimeOut > 0 && gain > gainreduce) {
			gain -= gain_step_attack;
			TimeOut--;
		} else {
			State = S_GainReduction;
			TimeOut = Hold;
		}
		break;

	case S_GainReduction:
		if (TimeOut > 0)
			TimeOut--;
		else {
			State = S_Release;
			TimeOut = Release;
		}
		break;

	case S_Release:
		if (TimeOut > 0 && gain < 1.0f) {
			TimeOut--;
			gain += gain_step_release;
		} else {
			State = S_NoOperation;
		}
		break;

	case S_NoOperation:
		if (gain < 1.0f)
			gain = 1.0F;
		break;

	default:

		break;

	}
	float outSampleF = inSample * gain;

	return (int) outSampleF;
}

void HAL_I2SEx_TxRxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {

	//restore signed 24 bit sample from 16-bit buffers
	int lSample = (int) (rxBuf[0] << 16) | rxBuf[1];
	int rSample = (int) (rxBuf[2] << 16) | rxBuf[3];

	// divide by 2 (rightshift) -> -3dB per sample
	lSample = lSample >> 1;
	rSample = rSample >> 1;

	//sum to mono
	lSample = rSample + lSample;
	rSample = lSample;

	//run HP on left channel and LP on right channel
	lSample = DoCompressor(lSample);
	//rSample = DoCompressor(rSample);

	//restore to buffer
	txBuf[0] = (lSample >> 16) & 0xFFFF;
	txBuf[1] = lSample & 0xFFFF;
	txBuf[2] = (rSample >> 16) & 0xFFFF;
	txBuf[3] = rSample & 0xFFFF;
}

void HAL_I2SEx_TxRxCpltCallback(I2S_HandleTypeDef *hi2s) {

	//restore signed 24 bit sample from 16-bit buffers
	int lSample = (int) (rxBuf[4] << 16) | rxBuf[5];
	int rSample = (int) (rxBuf[6] << 16) | rxBuf[7];

	// divide by 2 (rightshift) -> -3dB per sample
	lSample = lSample >> 1;
	rSample = rSample >> 1;

	//sum to mono
	lSample = rSample + lSample;
	rSample = lSample;

	//run HP on left channel and LP on right channel
	lSample = DoCompressor(lSample);
	//rSample = DoCompressor(rSample);

	//restore to buffer
	txBuf[4] = (lSample >> 16) & 0xFFFF;
	txBuf[5] = lSample & 0xFFFF;
	txBuf[6] = (rSample >> 16) & 0xFFFF;
	txBuf[7] = rSample & 0xFFFF;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
