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
#include "mqtt.h"
#include "EPD_console.h"

#define  path "/sdcard"

static struct {
     u8_t wav_id[4];
     u8_t wav_Size[4];
     u8_t wav_Type[4];

     u8_t wav_fmt[4];
     u8_t wav_Size1[16];
     u8_t wav_AudioFormat[2];
     u8_t wav_NumChannels[2];
     u8_t wav_SampleRate[4];
     u8_t wav_ByteRate[4];
     u8_t wav_BlockAlign[2];
     u8_t wav_BitsPerSample[2];

} wav_date;

void app_main(void)
{
//    ESP_ERROR_CHECK(nvs_flash_init());
//    ESP_ERROR_CHECK(esp_netif_init());
//    ESP_ERROR_CHECK(esp_event_loop_create_default());
//    init_spiffs();
//    wifi_init_sta();
//    html_int();
//    mqtt_app_start();
    mount_sdcard();

    DIR *d; //声明一个句柄
    struct dirent *file; //readdir函数的返回值就存放在这个结构体中
    struct stat sb;
    if(!(d = opendir(path)))
    {
      printf("error opendir %s!!!\n",path);
    }
    while((file = readdir(d)) != NULL)
   {
       printf("%s \n",file->d_name);
   }
    FILE *file_111;
    char xxx[4];

    file_111=fopen("/sdcard/123QWE.WAV","r");
    for (int j = 0; j < 10; ++j)
    {
        fread(xxx,1,4,file_111);
        for (int i = 0; i < 4; ++i)
        {
            printf("%c ",xxx[i]);
        }
        printf("\n");
    }

}
