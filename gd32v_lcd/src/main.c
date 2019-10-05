#include "lcd/lcd.h"
#include "fatfs/tf_card.h"
#include <string.h>

unsigned char image[12800];
FATFS fs;

void init_uart0(void)
{	
	/* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	/* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);

    usart_interrupt_enable(USART0, USART_INT_RBNE);
}

int main(void)
{
    uint8_t mount_is_ok = 1; /* 0: mount successful ; 1: mount failed */
    int offset = 0;
    FIL fil;
    FRESULT fr;     /* FatFs return code */
    UINT br;

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1|GPIO_PIN_2);

    init_uart0();

    Lcd_Init();			// init OLED
    LCD_Clear(WHITE);
    BACK_COLOR=WHITE;

    LEDR(1);
    LEDG(1);
    LEDB(1);

    fr = f_mount(&fs, "", 1);
    if (fr == 0)
        mount_is_ok = 0;
    else
        mount_is_ok = 1;

    if (mount_is_ok == 0)
    {
        while(1)
        {
            offset = 0;
            fr = f_open(&fil, "logo.bin", FA_READ);
            if (fr) printf("open error: %d!\n\r", (int)fr);
            f_lseek(&fil, offset);
            fr = f_read(&fil, image, sizeof(image), &br);
            LCD_ShowPicture(0,0,159,39);
            offset += 12800;
            LEDB_TOG;
            f_lseek(&fil, offset);
            fr = f_read(&fil, image, sizeof(image), &br);
            LCD_ShowPicture(0,40,159,79);
            LEDB_TOG;
            delay_1ms(1500);
            f_close(&fil);

            fr = f_open(&fil, "bmp.bin", FA_READ);
            if (fr) printf("open error: %d!\n\r", (int)fr);
            offset = 0;

            for (int i=0; i<2189;i++)
            {
                fr = f_read(&fil, image, sizeof(image), &br);
                LCD_ShowPicture(0,0,159,39);
                offset += 12800;
                f_lseek(&fil, offset);
                LEDB_TOG;
                fr = f_read(&fil, image, sizeof(image), &br);
                LCD_ShowPicture(0,40,159,79);
                offset += 12800;
                f_lseek(&fil, offset);
                LEDB_TOG;
            }

            /* Close the file */
            f_close(&fil);
        }
    }
    else
    {
        LCD_ShowString(24,  0, (u8 *)("no card found!"), BLACK);
        LCD_ShowString(24, 16, (u8 *)("no card found!"), BLUE);
        LCD_ShowString(24, 32, (u8 *)("no card found!"), BRED);
        LCD_ShowString(24, 48, (u8 *)("no card found!"), GBLUE);
        LCD_ShowString(24, 64, (u8 *)("no card found!"), RED);
        while (1)
        {
            LEDR_TOG;
            delay_1ms(200);
            LEDG_TOG;
            delay_1ms(200);
            LEDB_TOG;
            delay_1ms(200);
        }
    }
}

int _put_char(int ch)
{
    usart_data_transmit(USART0, (uint8_t) ch );
    while ( usart_flag_get(USART0, USART_FLAG_TBE)== RESET){
    }

    return ch;
}
