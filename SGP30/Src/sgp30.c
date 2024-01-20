//
// Created by LMF on 2024/1/13.
//
#include "stm32f1xx.h"
#include "sgp30.h"
#include "delay.h"
void SGP30_GPIO_Init(void ){
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStructure.Pin=SGP30_SCL;
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SGP30_PORT,&GPIO_InitStructure);
    GPIO_InitStructure.Pin=SGP30_SDA;
    HAL_GPIO_Init(SGP30_PORT,&GPIO_InitStructure);
}
void SDA_OUT(void ){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin=SGP30_SDA;
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SGP30_PORT,&GPIO_InitStructure);
}
void SDA_IN(void ){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin=SGP30_SDA;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode=GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(SGP30_PORT,&GPIO_InitStructure);
}
//产生IIC起始信号
void SGP30_IIC_Start(void ){
    SDA_OUT();
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SDA,GPIO_PIN_SET);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_SET);
    delay_us(20);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SDA,GPIO_PIN_RESET);
    delay_us(20);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
}
//产生IIC停止信号
void SGP30_IIC_Stop(void ){
    SDA_OUT();
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SDA,GPIO_PIN_RESET);
    delay_us(20);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_SET);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SDA,GPIO_PIN_SET);
    delay_us(20);
}
//等待应答信号
uint8_t SGP30_IIC_Wait_Ack(void ){
    uint8_t ucErrTime=0;
    SDA_IN();
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SDA,GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_SET);
    delay_us(10);
    while (SGP30_SDA_READ())
    {
        ucErrTime++;
        if (ucErrTime>250)
        {
            SGP30_IIC_Stop();
            return 1;
        }
    }
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
    return 0;
}
//产生ACK信号
void SGP30_IIC_Ack(void ){
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
    SDA_OUT();
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SDA,GPIO_PIN_RESET);
    delay_us(20);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_SET);
    delay_us(20);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
}
//不产生ACK应答
void SGP30_IIC_NAck(void ){
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
    SDA_OUT();
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SDA,GPIO_PIN_SET);
    delay_us(20);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_SET);
    delay_us(20);
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
}
//IIC发送一个字节
void SGP30_IIC_Send_Byte(uint8_t txd){
    uint8_t t;
    SDA_OUT();
    HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
    for (t=0;t<8;t++)
    {
        if ((txd&0x80)>>7)
            HAL_GPIO_WritePin(SGP30_PORT,SGP30_SDA,GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(SGP30_PORT,SGP30_SDA,GPIO_PIN_RESET);
        txd<<=1;
        delay_us(20);
        HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_SET);
        delay_us(20);
        HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
        delay_us(20);
    }
    delay_us(20);
}
//读一个字节
uint16_t SGP30_IIC_Read_Byte(uint8_t ack){
    uint8_t i;
    uint16_t receive=0;
    SDA_IN();
    for (i=0;i<8;i++){
        HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_RESET);
        delay_us(20);
        HAL_GPIO_WritePin(SGP30_PORT,SGP30_SCL,GPIO_PIN_SET);
        receive<<=1;
        if (SGP30_SDA_READ())
            receive++;
        delay_us(20);
    }
    if (!ack)
        SGP30_IIC_NAck();
    else
        SGP30_IIC_Ack();
    return receive;
}
//初始化IIC接口
void SGP30_Init(void ){
    SGP30_GPIO_Init();
    SGP30_Write(0x20,0x03);
}
void SGP30_Write(uint8_t a,uint8_t b){
    SGP30_IIC_Start();
    SGP30_IIC_Send_Byte(SGP30_write);
    SGP30_IIC_Wait_Ack();
    SGP30_IIC_Send_Byte(a);
    SGP30_IIC_Wait_Ack();
    SGP30_IIC_Send_Byte(b);
    SGP30_IIC_Wait_Ack();
    SGP30_IIC_Stop();
    delay_ms(100);

}
uint32_t SGP30_Read(void ){
    uint32_t dat;
    uint8_t crc;
    SGP30_IIC_Start();
    SGP30_IIC_Send_Byte(SGP30_read);
    SGP30_IIC_Wait_Ack();
    dat= SGP30_IIC_Read_Byte(1);
    dat<<=0;
    dat+= SGP30_IIC_Read_Byte(1);
    crc= SGP30_IIC_Read_Byte(1);
    crc=crc;
    dat<<=8;
    dat+= SGP30_IIC_Read_Byte(1);
    dat<<=8;
    dat+= SGP30_IIC_Read_Byte(0);
    SGP30_IIC_Stop();
    return (dat);
}
