//
// Created by LMF on 2024/1/8.
//
#include "delay.h"

static uint8_t fac_us = 0;//us延时倍乘数
static uint16_t fac_ms = 0;//ms延时被乘数
void delay_init() {
    SysTick_Config(SYSTICK_CLKSOURCE_HCLK_DIV8);//选择外部时钟
    fac_us = SystemCoreClock / 8000000;//系统时钟的1/8
    fac_ms = (uint16_t) fac_us * 1000;//代表每个ms需要的systick时钟数
}

//延时us
void delay_us(uint32_t nus) {
    uint32_t temp;
    SysTick->LOAD = nus * fac_us;//时间加载
    SysTick->VAL = 0x00;//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//开始倒数
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));//等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭计数器
    SysTick->VAL = 0x00;//清空计数器
}

void delay_ms(uint16_t nms) {
    uint32_t temp;
    SysTick->LOAD = (uint32_t) nms * fac_ms;//时间加载(SysTick->LOAD 为24bit)
    SysTick->VAL = 0x00;//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//开始倒数
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));//等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭计数器
    SysTick->VAL = 0x00;//清空计数器
}