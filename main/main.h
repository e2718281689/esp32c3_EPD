#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/param.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_spiffs.h"
#include "esp_netif.h"
#include "esp_err.h"
#include "esp_vfs_fat.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#include "soc/soc_caps.h"
#include "esp_vfs.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_err.h"
#include "esp_log.h"

#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_http_server.h"


static const char *TAG = "pdm_rec_example";

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)

#define EXAMPLE_ESP_WIFI_SSID "cjia"
#define EXAMPLE_ESP_WIFI_PASS "ufo31415962"
#define EXAMPLE_ESP_MAXIMUM_RETRY 5
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

sdmmc_card_t* card;
//sdmmc_host_t host = SDSPI_HOST_DEFAULT();
#define SD_MOUNT_POINT      "/sdcard"

#define MOUNT_POINT "/sdcard"
#define CONFIG_EXAMPLE_SPI_MOSI_GPIO 7
#define CONFIG_EXAMPLE_SPI_MISO_GPIO 2
#define CONFIG_EXAMPLE_SPI_SCLK_GPIO 6
#define CONFIG_EXAMPLE_SPI_CS_GPIO 1

static int s_retry_num = 0;

static EventGroupHandle_t s_wifi_event_group;

#define SCRATCH_BUFSIZE  8192