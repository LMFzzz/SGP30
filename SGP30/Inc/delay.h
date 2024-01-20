//
// Created by LMF on 2024/1/8.
//

#ifndef SGP30_DELAY_H
#define SGP30_DELAY_H
#include "stm32f1xx.h"
void delay_init(void );
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
#endif //SGP30_DELAY_H
