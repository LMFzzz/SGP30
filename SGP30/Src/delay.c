//
// Created by LMF on 2024/1/8.
//
#include "delay.h"

static uint8_t fac_us = 0;//us��ʱ������
static uint16_t fac_ms = 0;//ms��ʱ������
void delay_init() {
    SysTick_Config(SYSTICK_CLKSOURCE_HCLK_DIV8);//ѡ���ⲿʱ��
    fac_us = SystemCoreClock / 8000000;//ϵͳʱ�ӵ�1/8
    fac_ms = (uint16_t) fac_us * 1000;//����ÿ��ms��Ҫ��systickʱ����
}

//��ʱus
void delay_us(uint32_t nus) {
    uint32_t temp;
    SysTick->LOAD = nus * fac_us;//ʱ�����
    SysTick->VAL = 0x00;//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//��ʼ����
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));//�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//�رռ�����
    SysTick->VAL = 0x00;//��ռ�����
}

void delay_ms(uint16_t nms) {
    uint32_t temp;
    SysTick->LOAD = (uint32_t) nms * fac_ms;//ʱ�����(SysTick->LOAD Ϊ24bit)
    SysTick->VAL = 0x00;//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//��ʼ����
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));//�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//�رռ�����
    SysTick->VAL = 0x00;//��ռ�����
}