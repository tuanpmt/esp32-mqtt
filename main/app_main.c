#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "soc/rtc_cntl_reg.h"

#include "esp_wifi.h"
#include "esp_system.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "user_config.h"
#include "esp_log.h"

#include "mqtt.h"

#define MQTT_APP_TAG "MQTT APP"

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void connected_cb(void *self, void *params)
{
    mqtt_client *client = (mqtt_client *)self;
    mqtt_subscribe(client, "/test", 0);
    mqtt_publish(client, "/test", "howdy!", 6, 0, 0);
}
void disconnected_cb(void *self, void *params)
{

}
void reconnect_cb(void *self, void *params)
{

}
void subscribe_cb(void *self, void *params)
{
    mqtt_client *client = (mqtt_client *)self;
    ESP_LOGI(MQTT_APP_TAG, "Subscribe ok, test publish msg\n");
    mqtt_publish(client, "/test", "abcde", 5, 0, 0);
}

void publish_cb(void *self, void *params)
{

}
void data_cb(void *self, void *params)
{
    mqtt_client *client = (mqtt_client *)self;
    mqtt_event_data_t *event_data = (mqtt_event_data_t *)params;

    if (event_data->data_offset == 0) {

        char *topic = malloc(event_data->topic_length + 1);
        memcpy(topic, event_data->topic, event_data->topic_length);
        topic[event_data->topic_length] = 0;
        ESP_LOGI(MQTT_APP_TAG, "Publish topic: %s\n", topic);
        free(topic);
    }

    // char *data = malloc(event_data->data_length + 1);
    // memcpy(data, event_data->data, event_data->data_length);
    // data[event_data->data_length] = 0;
    ESP_LOGI(MQTT_APP_TAG, "Publish data[%d/%d bytes]\n",
         event_data->data_length + event_data->data_offset,
         event_data->data_total_length);
    // data);

    // free(data);

}

mqtt_settings settings = {
    .host = "192.168.0.103",
    .port = 1883,
    .client_id = "mqtt_client_id",
    .username = "user",
    .password = "pass",
    .clean_session = 0,
    .keepalive = 120,
    .lwt_topic = "/lwt",
    .lwt_msg = "offline",
    .lwt_qos = 0,
    .lwt_retain = 0,
    .connected_cb = connected_cb,
    .disconnected_cb = disconnected_cb,
    .reconnect_cb = reconnect_cb,
    .subscribe_cb = subscribe_cb,
    .publish_cb = publish_cb,
    .data_cb = data_cb
};

void app_main()
{

    ESP_LOGI(MQTT_APP_TAG, "Startup..\n");
    ESP_LOGI(MQTT_APP_TAG, "Free memory: %d bytes\n", esp_get_free_heap_size());
    ESP_LOGI(MQTT_APP_TAG, "SDK version: %s, Build time: %s\n", system_get_sdk_version(), BUID_TIME);

    nvs_flash_init();
    system_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );

    wifi_config_t sta_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .bssid_set = false
        }
    };


    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );

    ESP_LOGI(MQTT_APP_TAG, "Start, connect to Wifi network: %s ..\n", WIFI_SSID);
    ESP_LOGI(MQTT_APP_TAG, "Initial MQTT task\r\n");

    // Notice that, all callback will called in mqtt_task
    mqtt_start(&settings);
}
