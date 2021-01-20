# cortex_STM32F103_Nucleo
> for cortex, stm32, nucleo study



#### 1. __attribute__와 weak

 - __attribute__((att1, att2, ...__))로 변수, 구조체, 함수의 선언시 특성 속성을 부여할 수 있다.
  - 이중 weak의 특성은 컴파일 시 같은 이름의 weak 속성이 아닌 심볼(strong symbol이라 함)이 있을때 weak속성의 심볼을 제거하고 weak가 아닌 것을 사용하는 것이다.
  - 예를 들면 weak속성의 심볼은 정의 되지 않으면 컴파일 시 제거되어 오류를 검출하지 않으므로 소프트웨어 개발용 코드를 작성시에 함수를 weak 속성으로 선언하고 프로그래머가 직접 다른 파일에서 정의하도록 할 떄 사용할 수 있다.
  - 가장 큰 특징은 동일한 strong 심볼이 여러 개면 컴파일 오류가 발생하지만 weak심볼이 여러개면 랜덤으로 선택된다.

  #### 2. Hal라이브러리에서 인터럽트의 발생 코드는 다음과 같다
  > EXT15_10_IRQHandler() -> HAL_GPIO_EXTI_IRQHandler() -> HAL_GPIO_EXTI_Callback()


```c
void EXTI15_10_IRQHandler(void)
  {
    	/* USER CODE BEGIN EXTI15_10_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    /* USER CODE BEGIN EXTI15_10_IRQn 1 */  
    }
```


  ```c
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
  /* EXTI line interrupt detected */
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != 0x00u)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
    HAL_GPIO_EXTI_Callback(GPIO_Pin);
  }
}
  ```

```c
__weak void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}
```

```
 - 보이는 것처럼 call_back함수가 weak로 선언되어 있기 때문에 main에서 다시 정의해주면 된다
```

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	switch(GPIO_Pin){
		case B1_Pin:
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			break;

		default:
			;
	}
}
```

