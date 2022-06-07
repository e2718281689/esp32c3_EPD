/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "main.h"
#include "FATFS_sd.h"
#include "SPIFFS_flash.h"
#include "wifi_comm.h"
#include "web_serve.h"
#include "EPD.h"
//#include "Adafruit_MPU6050.h"

uint8_t gImage[2756]={0};


void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    init_spiffs();
    wifi_init_sta();
    html_int();
    mount_sdcard();
    EPD_init();

}
