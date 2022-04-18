#pragma once

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_vfs_fat.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#include "sdkconfig.h"



void EPD_init(void);
void EPD_refresh(void);
void EPD_sleep(void);
void PIC_display_Red(uint8_t *picData);
void PIC_display_White(uint8_t *picData);
void PIC_display(uint8_t *picData_old, uint8_t *picData_new);
void PIC_display_Clean(void);
void Wirte_EPD(uint8_t* picData,uint8_t x,uint8_t y,uint8_t nec);
void PIC_display_Clean_White(uint8_t* date);
void EPD_Write(uint8_t *picData, uint8_t x, uint8_t y, char zi);
void EPD_Printf(uint8_t *picData, uint8_t x, uint8_t y,char* mumber);
void Wirte_EPD_pieco(uint8_t* picData,uint8_t x,uint8_t y,uint8_t nec);
void EPD_Write_16(uint8_t *picData, uint8_t x, uint8_t y, char* zi);
void EPD_printf_16(uint8_t *picData, uint8_t x, uint8_t y, char* zi);