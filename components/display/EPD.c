
#include "img_EPD.h"
#include "fsdf.h"

#define EPD_NUM_MISO 2
#define EPD_NUM_MOSI 7
#define EPD_NUM_CLK 6
#define EPD_NUM_CS 8
#define EPD_NUM_DC 9
#define EPD_NUM_RES 5
#define EPD_NUM_BUSY 4

#define EPD_W21_RST_0 gpio_set_level(EPD_NUM_RES, 0)

#define EPD_W21_RST_1 gpio_set_level(EPD_NUM_RES, 1)

#define isEPD_W21_BUSY gpio_get_level(EPD_NUM_BUSY)

spi_device_handle_t spi;



static const char *TAG = "img_EPD";


// uint8_t gImage[2756]={0};

void EPD_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc = (int)t->user;
    gpio_set_level(EPD_NUM_DC, dc);
}
void EPD_cmd(spi_device_handle_t spi, const uint8_t cmd)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));                   // Zero out the transaction
    t.length = 8;                               // Command is 8 bits
    t.tx_buffer = &cmd;                         // The data is the cmd itself
    t.user = (void *)0;                         // D/C needs to be set to 0
    ret = spi_device_polling_transmit(spi, &t); // Transmit!
    assert(ret == ESP_OK);                      // Should have had no issues.
}

void EPD_data(spi_device_handle_t spi, const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t;
    if (len == 0)
        return;                                 // no need to send anything
    memset(&t, 0, sizeof(t));                   // Zero out the transaction
    t.length = len * 8;                         // Len is in bytes, transaction length is in bits.
    t.tx_buffer = data;                         // Data
    t.user = (void *)1;                         // D/C needs to be set to 1
    ret = spi_device_polling_transmit(spi, &t); // Transmit!
    assert(ret == ESP_OK);                      // Should have had no issues.
}

void driver_delay_xms(uint16_t ms)
{
    vTaskDelay(ms / portTICK_RATE_MS);
}

void EPD_W21_Init(void)
{
    EPD_W21_RST_0;         // Module reset
    driver_delay_xms(200); // At least 10ms delay
    EPD_W21_RST_1;
    driver_delay_xms(200); // At least 10ms delay
}
void EPD_W21_WriteCMD(uint8_t CMD)
{
    EPD_cmd(spi, CMD);
}
void EPD_W21_WriteDATA(uint8_t DATE)
{
    EPD_data(spi, &DATE, 1);
}
void Wirte_EPD(uint8_t *picData, uint8_t x, uint8_t y, uint8_t nec)//��д��
{
    if (nec)
    {
        picData[(211 - x) * 13 + (uint8_t)(y / 8)] &= ~(1 << (7-(y % 8))); //��X�ĵ�Yλ��0
    }
    else
    {
        picData[(211 - x) * 13 + (uint8_t)(y / 8)] |= 0x01 << (7-(y % 8)); //��X�ĵ�Yλ��1
    }

    //printf("(211 - x) * 13=%d,(uint8_t)(y / 8)=%d,(211 - x) * 13 + (uint8_t)(y / 8)=%d\n",(211 - x) * 13 ,(uint8_t)(y / 8),(211 - x) * 13 + (uint8_t)(y / 8));
}
void Wirte_EPD_pieco(uint8_t *picData, uint8_t x, uint8_t y, uint8_t nec)//��д��
{
    picData[(211 - x) * 13 + (uint8_t)(y / 8)] = nec;
}
void lcd_chkstatus(void)//�ȴ�
{
    unsigned char busy;
    do
    {
        EPD_W21_WriteCMD(0x71);
        busy = isEPD_W21_BUSY;
        busy = !(busy & 0x01);
        driver_delay_xms(1);
    } while (busy);
    driver_delay_xms(200);
}

void PIC_display_Clean(void)//����
{
    unsigned int i;
    EPD_W21_WriteCMD(0x10); // Transfer old data
    for (i = 0; i < 2888; i++)
    {
        EPD_W21_WriteDATA(0xff);
    }

    EPD_W21_WriteCMD(0x13); // Transfer new data
    for (i = 0; i < 2888; i++)
    {
        EPD_W21_WriteDATA(0xff);
    }
}
void PIC_display_Clean_White(uint8_t *date)
{
    for (int i = 0; i < 2888; i++)
    {
        date[i] = 0xFF;
    }
}
void PIC_display(uint8_t *picData_old,uint8_t *picData_new)//�׺�ɫ����
{
    EPD_W21_WriteCMD(0x10); // Transfer old data
    EPD_data(spi, picData_old, 2756);
    EPD_W21_WriteCMD(0x13); // Transfer new data
    EPD_data(spi, picData_new, 2756);
}
void PIC_display_White(uint8_t *picData)//��ɫ����
{
    EPD_W21_WriteCMD(0x10); // Transfer old data
    EPD_data(spi, picData, 2756);

    EPD_W21_WriteCMD(0x13); // Transfer new data
    for (int i = 0; i < 2888; i++)
    {
        EPD_W21_WriteDATA(0xff);
    }
}
void PIC_display_Red(uint8_t *picData)//��ɫ����
{    
    EPD_W21_WriteCMD(0x10); // Transfer old data
    for (int i = 0; i < 2888; i++)
    {
        EPD_W21_WriteDATA(0xff);
    }
    EPD_W21_WriteCMD(0x13); // Transfer new data
    EPD_data(spi, picData, 2756);
}
void EPD_sleep(void)//����
{
    EPD_W21_WriteCMD(0X02); // power off
    lcd_chkstatus();        // waiting for the electronic paper IC to release the idle signal
    EPD_W21_WriteCMD(0X07); // deep sleep
    EPD_W21_WriteDATA(0xA5);
}

void EPD_refresh(void)//ˢ��
{
    EPD_W21_WriteCMD(0x12); // DISPLAY REFRESH
    driver_delay_xms(1000); //!!!The delay here is necessary, 200uS at least!!!
    lcd_chkstatus();
    // printf("EPD_refresh");
}
void EPD_spi_init(void)//spi��ʼ��
{
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .miso_io_num = EPD_NUM_MISO,
        .mosi_io_num = EPD_NUM_MOSI,
        .sclk_io_num = EPD_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1 * 10 * 1000 * 1000,  // Clock out at 10 MHz
        .mode = 0,                               // SPI mode 0
        .spics_io_num = EPD_NUM_CS,              // CS pin
        .queue_size = 7,                         // We want to be able to queue 7 transactions at a time
        .pre_cb = EPD_spi_pre_transfer_callback, // Specify pre-transfer callback to handle D/C line
    };
    // Initialize the SPI bus
    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spi);

    gpio_reset_pin(EPD_NUM_DC);
    gpio_reset_pin(EPD_NUM_RES);
    gpio_reset_pin(EPD_NUM_BUSY);

    gpio_set_direction(EPD_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(EPD_NUM_RES, GPIO_MODE_OUTPUT);
    gpio_set_direction(EPD_NUM_BUSY, GPIO_MODE_INPUT);
}
void EPD_init(void)//��ʼ��
{
    uint8_t HRES = 0x68;       // 104
    uint8_t VRES_byte1 = 0x00; // 212
    uint8_t VRES_byte2 = 0xd4;

    EPD_spi_init();
    EPD_W21_Init(); // Electronic paper IC reset

    EPD_W21_WriteCMD(0x06);  // boost soft start
    EPD_W21_WriteDATA(0x17); // A
    EPD_W21_WriteDATA(0x17); // B
    EPD_W21_WriteDATA(0x17); // C

    EPD_W21_WriteCMD(0x04);
    lcd_chkstatus();

    EPD_W21_WriteCMD(0x00);  // panel setting
    EPD_W21_WriteDATA(0x0f); // LUT from OTP??128x296
    EPD_W21_WriteDATA(0x0d); // waiting for the electronic paper IC to release the idle signal

    EPD_W21_WriteCMD(0x61); // resolution setting
    EPD_W21_WriteDATA(HRES);
    EPD_W21_WriteDATA(VRES_byte1);
    EPD_W21_WriteDATA(VRES_byte2);

    EPD_W21_WriteCMD(0X50);  // VCOM AND DATA INTERVAL SETTING
    EPD_W21_WriteDATA(0x77); // WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7  ��߿���ɫ
}
void EPD_Printf(uint8_t *picData, uint8_t x, uint8_t y,char* mumber)
{

    for (uint8_t i = 0;  mumber[i]!='\r'; i++)
    {
        EPD_Write(picData,x+i*6,y,mumber[i]);
    }
}
void EPD_Write(uint8_t *picData, uint8_t x, uint8_t y, char zi)
{
    // uint8_t* pp=chF6x8[(int)zi-32];
    // EPD_Write(picData,x,y,pp);
    for (uint8_t i = 0; i < 6; i++)
    {
    uint8_t ss= ~chF6x8[zi-32][i];
     ss = ((ss * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
        Wirte_EPD_pieco(picData, x + i, y,ss);
    }
}

void EPD_Write_16(uint8_t *picData, uint8_t x, uint8_t y, char* zi)
{
     for (uint8_t i = 0; i < 16; i++)
     {
        for(uint8_t bit=0;bit<8;bit++)
        {
            uint8_t a=zi[i];
            uint8_t b =  ( a>> (7-bit)) & 1;
            Wirte_EPD(picData,x+i,y+bit,b);
        }
    }

    for(uint8_t i = 0; i < 16; i++) 
    {
        for(uint8_t bit=0;bit<8;bit++)
        {
            uint8_t a=zi[i+16];
            uint8_t b =  (a>> (7-bit)) & 1;
            Wirte_EPD(picData,x+i,y+bit+8,b);
        }
    }

}
/*
https://www.qqxiuzi.cn/zh/hanzi-gb2312-bianma.php GB2312�ַ�����
*/
void EPD_printf_16(uint8_t *picData, uint8_t x, uint8_t y, char* zi)
{
    for(u_int8_t i=0;zi[i]!='\0';i=i+2)
    {
        uint16_t zi_bit=((uint8_t)(zi[i]-0xb0)*94)+((uint8_t)(zi[i+1]-0xa0-1));
        printf("%d \n",zi_bit);
        EPD_Write_16(picData,x+i/2*16,y,&bin_data[(zi_bit)*32]);
    }
}

void EPD_main(void)
{

    // EPD_init();
    // PIC_display_Clean_White(gImage);
    // PIC_display_White(gImage);
    // EPD_Printf(gImage,50,50,"12345678\r");
    // PIC_display_White(gImage);
    // EPD_refresh();
    // EPD_sleep();
}