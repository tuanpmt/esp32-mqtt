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
#include "debug.h"

#include "mqtt.h"


void mqtt_connected(void *params)
{

}
void mqtt_disconnected(void *params)
{

}
void mqtt_reconnect(void *params)
{

}
void mqtt_subscribe(void *params)
{

}
void mqtt_publish(void *params)
{

}
void mqtt_data(void *params)
{

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
    .connected_cb = mqtt_connected,
    .disconnected_cb = mqtt_disconnected,
    .reconnect_cb = mqtt_reconnect,
    .subscribe_cb = mqtt_subscribe,
    .publish_cb = mqtt_publish,
    .data_cb = mqtt_data
};

void app_main()
{
    wifi_config_t cfg;
    INFO("[APP] Startup..\n");
    INFO("[APP] Free memory: %d bytes\n", system_get_free_heap_size());
    INFO("[APP] SDK version: %s, Build time: %s\n", system_get_sdk_version(), BUID_TIME);

#ifdef CPU_FREQ_160MHZ
    INFO("[APP] Setup CPU run as 160MHz\n");
    SET_PERI_REG_BITS(RTC_CLK_CONF, RTC_CNTL_SOC_CLK_SEL, 0x1, RTC_CNTL_SOC_CLK_SEL_S);
    WRITE_PERI_REG(CPU_PER_CONF_REG, 0x01);
    INFO("[APP] Setup CPU run as 160MHz - Done\n");
#endif
    INFO("[APP] Start, connect to Wifi network: %s ..\n", WIFI_SSID);

    strcpy(cfg.sta.ssid, WIFI_SSID);
    strcpy(cfg.sta.password, WIFI_PASS);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &cfg);
    esp_wifi_start();
    esp_wifi_connect();

    INFO("[APP] Initial MQTT task\r\n");

    // Notice that, all callback will called in mqtt_task
    // All function publish, subscribe
    mqtt_start(&settings);
    while (1);
}
