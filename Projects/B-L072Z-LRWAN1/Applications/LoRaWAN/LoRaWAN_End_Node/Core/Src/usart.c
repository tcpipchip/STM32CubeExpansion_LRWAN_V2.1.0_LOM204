/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "dma.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;
UART_HandleTypeDef huart5;
DMA_HandleTypeDef hdma_usart5_tx;

/**
  * @brief USART5 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART5_UART_Init(void)
{
	huart5.Instance = USART5;
	huart5.Init.BaudRate = 115200;
	huart5.Init.WordLength = UART_WORDLENGTH_8B;
	huart5.Init.StopBits = UART_STOPBITS_1;
	huart5.Init.Parity = UART_PARITY_NONE;
	huart5.Init.Mode = UART_MODE_TX_RX;
	huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart5.Init.OverSampling = UART_OVERSAMPLING_16;
	huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart5) != HAL_OK)
	{
	Error_Handler();
	}

}

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance        = USART2;
  huart2.Init.BaudRate   = USART_BAUDRATE;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits   = UART_STOPBITS_1;
  huart2.Init.Parity     = UART_PARITY_NONE;
  huart2.Init.Mode       = UART_MODE_TX;
  huart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  if (uartHandle->Instance == USART2)
  {
    /* USER CODE BEGIN USART2_MspInit 0 */

    /* USER CODE END USART2_MspInit 0 */

    /* Enable peripherals and GPIO Clocks */
    /* Enable GPIO TX/RX clock */
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();

    /* Enable USARTx clock */
    __USART2_CLK_ENABLE();

    /* Enable DMA clock */
    DMAx_CLK_ENABLE();

    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    /* Select SYSTEM clock for USART2 commuincation TX/RX */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /**USART2 GPIO Configuration
    PA3     ------> USART2_RX
    PA2     ------> USART2_TX
      */
    GPIO_InitStruct.Pin = USARTx_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;
    HAL_GPIO_Init(USARTx_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USARTx_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;
    HAL_GPIO_Init(USARTx_RX_GPIO_Port, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_TX Init */
    /* Configure the DMA handler for Transmission process */
    hdma_usart2_tx.Instance                 = USARTx_TX_DMA_CHANNEL;
    hdma_usart2_tx.Init.Request             = USARTx_TX_DMA_REQUEST;
    hdma_usart2_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode                = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority            = DMA_PRIORITY_LOW;

    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    /* Associate the initialized DMA handle to the UART handle */
    __HAL_LINKDMA(uartHandle, hdmatx, hdma_usart2_tx);

    /* Configure the NVIC for DMA */
    /* NVIC configuration for DMA transfer complete interrupt (USART1_TX) */
    HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, USARTx_Priority, 1);
    HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);

    /* NVIC for USART, to catch the TX complete */
    HAL_NVIC_SetPriority(USARTx_IRQn, USARTx_DMA_Priority, 1);
    HAL_NVIC_EnableIRQ(USARTx_IRQn);

    /* USER CODE BEGIN USART2_MspInit 1 */

    /* USER CODE END USART2_MspInit 1 */
  }if(uartHandle->Instance==USART5){
  /* USER CODE BEGIN USART5_MspInit 0 */

  /* USER CODE END USART5_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART5_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART5 GPIO Configuration
    PB3     ------> USART5_TX
    PB4     ------> USART5_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_USART5;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART5 DMA Init */
    /* USART5_TX Init */
    hdma_usart5_tx.Instance = DMA1_Channel3;
    hdma_usart5_tx.Init.Request = DMA_REQUEST_13;
    hdma_usart5_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart5_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart5_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart5_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart5_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart5_tx.Init.Mode = DMA_NORMAL;
    hdma_usart5_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart5_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle, hdmatx, hdma_usart5_tx);

    /* USART5 interrupt Init */
    HAL_NVIC_SetPriority(USART4_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART4_5_IRQn);
  /* USER CODE BEGIN USART5_MspInit 1 */

  /* USER CODE END USART5_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

  if (uartHandle->Instance == USART2)
  {
    /* USER CODE BEGIN USART2_MspDeInit 0 */

    /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA3     ------> USART2_RX
    PA2     ------> USART2_TX
      */
    HAL_GPIO_DeInit(GPIOA, USARTx_RX_Pin | USARTx_TX_Pin);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
    /* USER CODE BEGIN USART2_MspDeInit 1 */

    /* USER CODE END USART2_MspDeInit 1 */
  } else if(uartHandle->Instance == USART5) {
	/* USER CODE BEGIN USART5_MspDeInit 0 */

	/* USER CODE END USART5_MspDeInit 0 */
	/* Peripheral clock disable */
	__HAL_RCC_USART5_CLK_DISABLE();

	/**USART5 GPIO Configuration
	PB3     ------> USART5_TX
	PB4     ------> USART5_RX
	*/
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_4);

	/* USART5 DMA DeInit */
	HAL_DMA_DeInit(uartHandle->hdmatx);

	/* USART5 interrupt DeInit */
	HAL_NVIC_DisableIRQ(USART4_5_IRQn);
	/* USER CODE BEGIN USART5_MspDeInit 1 */

	/* USER CODE END USART5_MspDeInit 1 */

  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
