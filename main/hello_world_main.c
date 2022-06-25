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

#include "esp_console.h"
#include <dirent.h>
#include "argtable3/argtable3.h"

#define  path "/sdcard"

static struct {
    struct arg_int *port;
    struct arg_end *end;
} file_ls_args;

static int file_ls_cmd(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **)&file_ls_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, file_ls_args.end, argv[0]);
        return 0;
    }
    int chip_addr = file_ls_args.port->ival[0];
    printf("%d \n",chip_addr);

    return 0;
}
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
//
//    DIR *d; //声明一个句柄
//    struct dirent *file; //readdir函数的返回值就存放在这个结构体中
//    struct stat sb;
//    if(!(d = opendir(path)))
//    {
//      printf("error opendir %s!!!\n",path);
//    }
//    while((file = readdir(d)) != NULL)
//   {
//       printf("%s \n",file->d_name);
//   }

    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.prompt = "EPD_sss>";

//    i2cconfig_args.port = arg_int0(NULL, "port", "<0|1>", "Set the I2C bus port number");
//    i2cconfig_args.freq = arg_int0(NULL, "freq", "<Hz>", "Set the frequency(Hz) of I2C bus");
//    i2cconfig_args.sda = arg_int1(NULL, "sda", "<gpio>", "Set the gpio for I2C SDA");
//    i2cconfig_args.scl = arg_int1(NULL, "scl", "<gpio>", "Set the gpio for I2C SCL");
//    i2cconfig_args.end = arg_end(2);

    file_ls_args.port= arg_int1("c", "chip", "<chip_addr>", "Specify the address of the chip on that bus");
    file_ls_args.end = arg_end(2);
    const esp_console_cmd_t file_cd_cmd = {
            .command = "ls",
            .help = "Config I2C bus",
            .hint = NULL,
            .func = &file_ls_cmd,
            .argtable = &file_ls_args
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&file_cd_cmd));

    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}
