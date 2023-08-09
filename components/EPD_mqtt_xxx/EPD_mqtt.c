#include "EPD_mqtt.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/event_groups.h"
//#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"


static const char *TAG = "mqtt";

#define MQTT_EVENT_CONNECTED_group        BIT0
#define MQTT_EVENT_DATA_group           BIT1

static EventGroupHandle_t s_mqtt_event_group;
static SemaphoreHandle_t MQTT_EVENT_CONNECTED_task;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    int msg_id;
    switch((esp_mqtt_event_id_t)event_id)
    {
        case MQTT_EVENT_CONNECTED:
        {
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);/*  */
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            //xSemaphoreGive(pQueueDefinition);
            break;
        }
        case MQTT_EVENT_DATA:
        {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            xSemaphoreGive(MQTT_EVENT_CONNECTED_task);
            break;
        }
        default:
            ESP_LOGI(TAG, "Other event id:");
            break;
    }
}
void MQTT_EVENT_DATA_group_f(void *pvParam)
{
    while(1)
    {

        if(xSemaphoreTake(MQTT_EVENT_CONNECTED_task, 10) == pdTRUE)
        {
            printf("xfgdtgdfg\n");
            //xSemaphoreGive( pQueueDefinition );
        }
        vTaskDelay( 1 / portTICK_PERIOD_MS);
    }
}
void mqtt_app_start()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
    };
    MQTT_EVENT_CONNECTED_task=xSemaphoreCreateBinary();

    s_mqtt_event_group = xEventGroupCreate();
    xTaskCreate(MQTT_EVENT_DATA_group_f,"sdfgdgfgfd",2048,NULL,1,NULL);

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}