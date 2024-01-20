//
// Created by LMF on 2024/1/13.
//
#include "dht11.h"
#include "delay.h"
void DHT11_IO_OUT(void ){
    GPIO_InitTypeDef GPIO_InitStructure;
    DHT11_CLK();
    GPIO_InitStructure.Pin=DHT11_PIN;
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PORT,&GPIO_InitStructure);
}
void DHT11_IO_IN(void ){
    GPIO_InitTypeDef GPIO_InitStructure;
    DHT11_CLK();
    GPIO_InitStructure.Pin=DHT11_PIN;
    GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
    HAL_GPIO_Init(DHT11_PORT,&GPIO_InitStructure);
}
void DHT11_ReSet(void ){
    DHT11_IO_OUT();
    HAL_GPIO_WritePin(DHT11_PORT,DHT11_OUT,GPIO_PIN_RESET);
    delay_ms(20);
    HAL_GPIO_WritePin(DHT11_PORT,DHT11_OUT,GPIO_PIN_SET);
    delay_us(30);
}
uint8_t DHT11_Check(void ){
    uint8_t retry=0;
    DHT11_IO_IN();
    while (DHT11_IN && retry<100)
    {
        retry++;
        delay_us(1);
    }
    if (retry>=100) return 1;
    else retry=0;
    while (!DHT11_IN && retry<100)
    {
        retry++;
        delay_us(1);
    }
    if (retry>=100) return 1;
    return 0;
}
uint8_t DHT11_Read_Bit(void )
{
    uint8_t retry=0;
    while (DHT11_IN && retry<100)
    {
        retry++;
        delay_us(1);
    }
    retry=0;
    while (!DHT11_IN && retry<100)
    {
        retry++;
        delay_us(1);
    }
    delay_us(40);
    if (DHT11_IN) return 1;
    else return 0;
}
uint8_t DHT11_Read_Byte(void )
{
    uint8_t i,dat;
    dat=0;
    for (i=0;i<8;i++)
    {
        dat<<=1;
        dat|=DHT11_Read_Bit();
    }
    return dat;
}
uint8_t Read_DHT_Data(uint8_t *temp,uint8_t *hum)
{
    uint8_t buf[5];
    uint8_t i;
    DHT11_ReSet();
    if (DHT11_Check()==0)
    {
        for (i=0;i<5;i++)
        {
            buf[i]=DHT11_Read_Byte();
        }
        if ((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
        {
            *hum=buf[0];
            *(hum+1)=buf[1];
            *temp=buf[2];
            *(temp+1)=buf[3];
        }
    } else{
        return 1;
    }
    return 0;
}
uint8_t DHT11_Init(void ){
    DHT11_ReSet();
    return DHT11_Check();
}
