//
// Created by LMF on 2024/1/13.
//

#ifndef SGP30_DHT11_H
#define SGP30_DHT11_H
#include "stm32f1xx.h"
#define DHT11_OUT GPIO_PIN_11
#define DHT11_IN  GPIO_PIN_11
#define DHT11_PIN GPIO_PIN_11
#define DHT11_PORT GPIOG//GPIOG端口
#define DHT11_CLK() __HAL_RCC_GPIOG_CLK_ENABLE() //GPIOG端口时钟
uint8_t DHT11_Init(void );//初始化DHT11
uint8_t Read_DHT_Data(uint8_t *temp,uint8_t *hum);//读取温湿度
uint8_t DHT11_Read_Byte(void );//读出一个字节
uint8_t DHT11_Read_Bit(void );//读出一个位
uint8_t DHT11_Check(void );//检测是否存在DHT11
void DHT11_ReSet(void );//复位DHT11
#endif //SGP30_DHT11_H
