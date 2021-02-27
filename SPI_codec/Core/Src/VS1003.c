/*
 * VS1003.c
 *
 *  Created on: 2021. 2. 24.
 *      Author: pc1
 */
void VS1003_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;

	/*GPIO clock enable*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*/CS */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*XDCS*/
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*XRESET*/
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*DREQ*/
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*SPI2 GPIO CONFIGURATION
	 * PB10 -> SPI2_SCK
	 * PB14 -> SPI2_MISO
	 * PB15 -> SPI2_MOSI
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AFS_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	MP3_RE

}

