#ifndef _GD32V_PJT_INCLUDE_
#define _GD32V_PJT_INCLUDE_

#include "gd32vf103_libopt.h"
#include <stdio.h>

#define PA_OUT(n,s)                                         \
({  if (s%2)        gpio_bit_set(GPIOA, GPIO_PIN_##n);      \
    else            gpio_bit_reset(GPIOA, GPIO_PIN_##n);   })
#define PA_IN(n)    gpio_input_bit_get(GPIOA, GPIO_PIN_##n)

#define PB_OUT(n,s)                                         \
({  if (s%2)        gpio_bit_set(GPIOB, GPIO_PIN_##n);      \
    else            gpio_bit_reset(GPIOB, GPIO_PIN_##n);   })
#define PB_IN(n)    gpio_input_bit_get(GPIOB, GPIO_PIN_##n)

#define PC_OUT(n,s)                                         \
({  if (s%2)        gpio_bit_set(GPIOC, GPIO_PIN_##n);      \
    else            gpio_bit_reset(GPIOC, GPIO_PIN_##n);   })
#define PC_IN(n)    gpio_input_bit_get(GPIOC, GPIO_PIN_##n)

#define LEDR_TOG    gpio_bit_write(GPIOC, GPIO_PIN_13, (bit_status)(1-gpio_input_bit_get(GPIOC, GPIO_PIN_13)))
#define LEDR(s)     PC_OUT(13, s)
#define LEDG_TOG    gpio_bit_write(GPIOA, GPIO_PIN_1, (bit_status)(1-gpio_input_bit_get(GPIOA, GPIO_PIN_1)))
#define LEDG(s)     PA_OUT(1, s)
#define LEDB_TOG    gpio_bit_write(GPIOA, GPIO_PIN_2, (bit_status)(1-gpio_input_bit_get(GPIOA, GPIO_PIN_2)))
#define LEDB(s)     PA_OUT(2, s)

#endif /* _GD32V_PJT_INCLUDE_ */
