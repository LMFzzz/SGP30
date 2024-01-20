//
// Created by LMF on 2024/1/13.
//

#ifndef SGP30_SGP30_H
#define SGP30_SGP30_H
#include "stm32f1xx.h"
#define SGP30_SCL GPIO_PIN_5
#define SGP30_SDA GPIO_PIN_6
#define SGP30_PORT GPIOA
#define SGP30_SDA_READ() HAL_GPIO_ReadPin(SGP30_PORT,SGP30_SDA)
#define SGP30_read 0xb1 //SGP30的读地址
#define SGP30_write 0xb0 //SGP30的写地址
void SGP30_IIC_Start(void );//发送IIC信号
void SGP30_IIC_Stop(void );//停止IIC信号
void SGP30_IIC_Send_Byte(uint8_t txd);//IIC发送一个字节
uint16_t SGP30_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t SGP30_IIC_Wait_Ack(void );//IIC等待Ack信号
void SGP30_IIC_Ack(void );//IIC发送ACK信号
void SGP30_IIC_NAck(void );//IIC不发送ACK信号
void SGP30_Init(void );
void SGP30_Write(uint8_t a,uint8_t b);
uint32_t SGP30_Read(void );
#endif //SGP30_SGP30_H
