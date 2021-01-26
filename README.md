# cortex_STM32F103_Nucleo
> for cortex, stm32, nucleo study



#### 1. __attribute__와 weak

 - __attribute__((att1, att2, ...__))로 변수, 구조체, 함수의 선언시 특성 속성을 부여할 수 있다.
  - 이 중 weak의 특성은 컴파일 시 같은 이름의 weak 속성이 아닌 심볼(strong symbol이라 함)이 있을때 weak 속성의 심볼을 제거하고 weak가 아닌 것을 사용하는 것이다.
  - 예를 들면 weak 속성의 심볼은 정의 되지 않으면 컴파일 시 제거되어 오류를 검출하지 않으므로 소프트웨어 개발용 코드를 작성시에 함수를 weak 속성으로 선언하고 프로그래머가 직접 다른 파일에서 정의하도록 할 떄 사용할 수 있다.
  - 가장 큰 특징은 동일한 strong 심볼이 여러 개면 컴파일 오류가 발생하지만 weak심볼이 여러 개면 랜덤으로 선택된다.

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

- 보이는 것처럼 call_back함수가 weak로 선언되어 있기 때문에 main에서 다시 정의해주면 된다

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



#### 3. EXTI(External Interrupt)

- 정확히 인터럽트는 외부와 내부 인터럽트로 구성되어 있다.
- 외부 인터럽트란 물리적인 인터럽트로 스위치를 예로 들 수 있고, 내부 인터럽트는 atmega실습에서 수행한 타이머 인터럽트를 들 수 있다.



#### 4. Watchdog Timer란

- 시스템이 무한루프나 비정상적인 동작을 하는 경우 자동으로 시스템을 리셋시키는 기능을 말한다.

- 여기에서 무한루프나 오류 등의 상황에 대하여 모니터링하는데 사용하는 타이머를 Watchdog 타이머라 한다.

  > 타이머를 맞추어 놓고, 계속 동작하면서 와치도그 타이머를 리셋시켜 와치도그 인터럽트가 발생되지 않도록 한다.
  >
  > 만약 시스템 내부적의 오류로 인해 와치도그 타이머를 리셋시키지 못해 인터럽트가 발생되면 프로그램에 문제가 생긴 것으로 간주하여 하드웨어적으로 리셋하게 되는 원리이다.



#### 5. RCC란 Reset Control Clock의 약자이다



#### 6. Auto-reload preload enable은 반복적인 작업이 발생하도록 하는 설정으로 일단 이해



#### 7. NVIC 

> Nested Vectored Interrupt Controller의 약자로, 중첩된 인터럽트를 제어하는 기능이다.

- 모든 exception에 대해서 우선순위가 설정되어 있고, 이 우선순위에 따라 interrupt를 처리하는데 의의가 있다.



#### 8. 다음과 같은 타이머 초기화 코드를 작성하면 이와 같은 코드가 main.c에 작성된다.

```c
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 63; //분주비
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP; //timer 카운터 값이 올라감
  htim3.Init.Period = 999; //기간이지용
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
```



#### 9. Timer 중 LP가 붙은건 저전력 모드(low power)



#### 10. 마찬가지로 타이머 인터럽트가 발생하면 TIM3_IRQHandler() -> HAL_TIM_IRQHandler() -> HAL_TIM_PeriodElapsedCallback() 순서로 호출된다.



#### 11. volatile 변수로 선언한 변수는 사용할 때 항상 메모리에 접근하게 된다.



#### 12. ADC conversion에는 interrupt없이 하는 것이 regular conversion이고, conversion 도중에 다른 conversion이 생겨서 interrupt와 비슷하게 중간에 끼어 들어서 conversion이 가능한 것이 injected conversion이라고 생각하면 된다



#### 13. DMA(Direct Memory Access)란 cpu를 대신하여 I/O장치와 Memory사이의 데이터 전송을 담당하는 장치를 지칭한다.

- CPU의 개입 없이 주변장치와 주기억장치와의 데이터 전송이 이루어지는 방법을 통칭
- 프로그램 수행 중 입출력을 위한 인터럽트의 발생 횟수를 최소화하여 컴퓨터 시스템의 효율을 높인다.
- CPU는 DMA와 상태정보 및 제어정보만을 주고 받는다.



#### 14. ADC의 최대 16개의 채널에서 데이터를 읽어 올 수 있는데, 각각의 채널에서 데이터를 모두 읽어 오려면 상당히 귀찮은 작업을 해줘야한다.

- 하지만 DMA를 이용하면 CPU는 미리 설정된 주소의 메모리만 읽으면 DMA가 전달해준 새로운 값을 가져올 수 있게 된다.



#### 15. Continuous conversion을 Enable하면 ADC값을 계속해서 읽어 들일 수 있다.



#### 16. [MUX](https://m.blog.naver.com/asd7979/30109725011)

- 멀티플렉서 또는 mux는 여러 아날로그 또는 디지털 입력 신호 중 하나를 선택하여 선택된 입력을 하나의 라인에 전달하는 장치이다.
- 전자 멀티플렉서는 여러 신호가 이를테면 입력 신호 당 하나의 장치를 보유하지 않고 아날로그 - 디지털 변환회로나 하나의 통신선 등 하나의 장치나 자원을 공유할 수 있게 만들어준다.
- n개의 선택선의 조합에 의해 선택된 2^n개의 입력선 중에서 하나를 선택하여 출력선에 연결시켜 주는 회로이며, 여러 개의 회로가 단일 회선을 공동으로 이용하여 신호를 전송하는데 사용한다.

![제목 없음](https://user-images.githubusercontent.com/18729679/105497220-e697f180-5d01-11eb-8b1f-74e62f3395f3.png)

- s1, s0는 Y출력을 결정하게 할 결정자 또는 선택자가 된다.
- s1과 s0의 값에 따라 어떤 입력이 출력되는 가를 선택하는 것이라고 할 수 있다.
- 입력된 4개의 값 중 하나를 선택해 출력한다.



![image-20210122223623985](README.assets/image-20210122223623985.png)



#### 17. ADC 초기화 과정 코드 내용

```c
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
```



#### 18. printf 포팅 코드 붙여넣기

```c
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	if(ch == '\n')
		HAL_UART_Transmit(&huart2, (uint8_t*) "\r", 1, 0xFFFF);
	HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, 0xFFFF);

	return ch;
}
```



#### 19. [PLL(Phase Locked Loop)](http://www.rfdh.com/bas_rf/begin/pll.php3)

![image](https://user-images.githubusercontent.com/18729679/105498687-d4b74e00-5d03-11eb-9ee6-738596a17715.png)

> 디지털 클럭을 이용하는 디지털 신호전송에 있어서.. 들어오는 신호가 0인지 1인지 명확히 구분하려면, 적어도 1과 0을 판별해야 하는 range를 먼저 정확히 정의해야 할 것입니다. 이것은 결국 하나하나의 클럭이 시작되고 끝나는 지점을 명확히 알아야 한다는 것을 의미하겠죠. 하지만 무선 혹은 유선상으로 신호를 보내면 신호경로에 따라 신호 delay가 생기고 당연히 위상이 변하기 때문에, 수신측에서 보기엔 어느 point를 시작과 끝점으로 0과 1을 판단해야 하는 것인지 모호하게 되어 버립니다.
>
> 그럴 때 무엇이 필요할까요? 바로 클럭의 시작과 끝을 정확히 맞추어주는, 즉 동기(synchronization)시켜줄 무엇인가가 필요합니다. 그리고 이렇게 한 주기의 시작(0도)과 끝(360도)을 맞추어준다는 것은, 들어오는 신호가 어떻게 들어오던 간에 결국 특정 위상점에서 들어오는 것처럼 고정(lock)하는 것입니다.
>
> 바로 이렇게 주기적 신호의 위상을 원하는대로, 흔들리지 않는 정확한 고정점으로 잡아주기 위해 만들어진 회로가 바로 PLL인 것이죠.



#### 20. [ADC에서 self calibration을 하지 않을 때 문제점](https://jeonhj.tistory.com/22)

- STM32 일부 디바이스는 전원 인가 또는 리셋 후 ADC Self Calibration을 해줘야 정상적인 ADC 결과값을 얻을 수 있다.

  

  ![image](https://user-images.githubusercontent.com/18729679/105499348-b00fa600-5d04-11eb-9703-a959d19df126.png)

  

#### 21. [printf에서 float를 출력하기 위해서는 따로 설정을 해줘야한다.][https://blog.naver.com/chandong83/221390413346]

- Project -> Properties -> C/C++ Build > Settings > Tool Settings > MCU Settings를 선택한 다음 "Use float with printf fromt newlib - nano (-u_printf_float)" 항목을 체크하면 된다.



#### 22. STM32 시리즈의 MCU는 시간 정밀도와 결합 복원도에서 서로 다른 특성을 갖는 두 개의 IWDG(Independent WDG)와 WWDG(Window WDG)를 내장하고 있다. => WDG란 Window Watch Dog의 약자이다.



#### 23.  [와치독 시간은 아래 공식으로 구해진다.][https://m.blog.naver.com/PostView.nhn?blogId=hms4913&logNo=30150058967&proxyReferer=https:%2F%2Fwww.google.com%2F] 

> t~WWDG~ = t~PCLK~ * 4096 *  2^WDGTB^ * * ( t[5:0] + 1)     (ms)
>
> - t~PCLK~: PCLK1 clock time(본 실습에서는 32MHz이므로 31.25ns)
> - 2^WTGTB^ : WWDG counter clock prescaler
> - W[6:0] : WWDG window value, 63(0x3F)보다 커야 함.
> - T[6:0] : WWDG free - runnign downcounter value
> - t[5:0] : T[6:0] - W[6:0]

- 본 실습에서 적용 되는 값들을 넣어서 계산해 보면 아래와 같다

  > t~WWDG~ = 31.25 * 10^-9^ *4096*8*(127-80+1) = 49.152 ms

- 와치독 카운터를 갱신하려면 49.152~66.56ms(0x3F time) 구간(Window)에서 갱신(Refresh)를 해야한다. 너무 빠르거나 늦으면 MCU가 리셋된다.

![image-20210124212737923](README.assets/image-20210124212737923.png)

- ==즉, W[6:0] 과 0x3F 사이의 시간에서 와치독 타이머의 downcounter를 갱신해 주지 않으면 MCU는 리셋될 것이다. 따라서 반드시 downcounter를 갱신해주는 HAL_WWDG_Refresh함수를 49.152~66.56ms(0x3F time) 에서 갱신해야함으로 delay(50) 후 실행하는 것이 필요하다==

```c
  while (1)
  {
	  if(i==20)
	  {
		  i=0;
		  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  }
	  
	  HAL_Delay(50);
	  i++;
	  /* Refresh WWDH*/
	  //와치독 함수의 downcounter를 갱신해줌
	  if(HAL_WWDG_Refresh(&hwwdg) != HAL_OK){
		  Error_Handler();
	  }
	  
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
```

#### 24. Watchdog flag

- watchdog에 의해서 counter값이 초기화 되지 않아 reset되어 실행되었는지 알 수 있도록 flag함수를 제공해 준다.

  ```c
   //와치독의 flag가 true라면 => 즉 MCU가 reset되었다면
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) == SET){
  	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  	  HAL_Delay(4000);
  	  __HAL_RCC_CLEAR_RESET_FLAGS();
    }else{
  	  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    }
  ```

  



#### 25. printf() 터미널 디버깅 방법을 다룰 때 USART 를 사용하였다. 

- 하지만  Tx 출력 기능만을 이용하였기 떄문에 Polling 방식을 이용하더라도 큰 문제없이 USART 디버깅 출력이 가능하였다. 하지만 통신으로 Rx 입력을 받게 된다면 Polling 방식으로는 수신 데이터 유실 및 프로그램 구현에 불편함이 많다.

  

#### 26. GIE(global interrupt enable) 

> 이 비트는 모든 인터럽트에 대해 on/off 스위치와 같습니다. GIE 비트를 0으로 셋팅하면 모든 인터럽트들이 각각의 활성화 설정 비트와 상관없이 비활성화 됩니다. GIE = 1으로 셋팅하면 인터럽트는 활성화 됩니다. 단 각각의 인터럽트는 각각의 활성화 비트가 설정되어 있어야 합니다. 인터럽트가 요청되면 GIE 비트는 자동으로 0으로 셋팅이 되어 더 이상의 인터럽트 요청을 비활성화 시켜버립니다. 요청된 인터럽트의 서비스 루틴이 실행이 마무리되어 return 명령이 실행될 때 GIE 비트는 다시 1로 설정이 되어 전체 인터럽트 시스템을 활성화 시킵니다.



#### 27. PEIE(peripheral interrupt enable)

> 비트는 peripheral interrupt 그룹에 대한 미니 마스터 스위치입니다. 이 비트는 peripheral itnerrupt들을 활성화 시키기 위해선 반드시 셋팅되어야 합니다. 참고로, peripheral interrupt들은 PIE1 레지스터에 각각의 활성화 비트를 가지고 있습니다.



#### 28. USART2 수신 인터럽트 발생 시 콜백 함수를 호출하고 플래그를 클리어 해주는 코드

``` c
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	if((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE) != RESET)){
		HAL_UART_RxCpltCallback(&huart2);
		__HAL_UART_CLEAR_PEFLAG(&huart2);
	}
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}
```



#### 29.  uart 수신 인터럽트를 발생시키기 위해서는 `__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);`를 통해 초기화를 해줘야한다.

```

```

