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


static TaskHandle_t xMqttTask = NULL;

int resolev_dns(const char *host, struct sockaddr_in *ip) {
    struct hostent *he;
    struct in_addr **addr_list;
    he = gethostbyname(host);
    if (he == NULL) return 0;
    addr_list = (struct in_addr **)he->h_addr_list;
    if (addr_list[0] == NULL) return 0;
    ip->sin_family = AF_INET;
    memcpy(&ip->sin_addr, addr_list[0], sizeof(ip->sin_addr));
    return 1;
}

int client_connect(const char *stream_host, const char *stream_path, int stream_port)
{
    while (1) {

        struct sockaddr_in remote_ip;
        bzero(&remote_ip, sizeof(struct sockaddr_in));

        //if stream_host is not ip address, resolve it
        if (inet_pton(AF_INET, stream_host, &(remote_ip.sin_addr)) != 1) {
            if (!resolev_dns(stream_host, &remote_ip)) {
                vTaskDelay(1000 / portTICK_RATE_MS);
                continue;
            }
        }
        int sock = socket(PF_INET, SOCK_STREAM, 0);

        if (sock == -1) {
            continue;
        }

        remote_ip.sin_port = htons(stream_port);
        INFO("[READER] Connecting to server %s...:%d,%d\n", ipaddr_ntoa((const ip_addr_t*)&remote_ip.sin_addr.s_addr), stream_port, remote_ip.sin_port);

        if (connect(sock, (struct sockaddr *)(&remote_ip), sizeof(struct sockaddr)) != 00) {
            close(sock);
            INFO("[READER] Conn err.\n");
            vTaskDelay(1000 / portTICK_RATE_MS);
            continue;
        }
        //Cobble together HTTP request
        write(sock, "GET ", 4);
        write(sock, stream_path, strlen(stream_path));
        write(sock, " HTTP/1.0\r\nHost: ", 17);
        write(sock, stream_host, strlen(stream_host));
        write(sock, "\r\n\r\n", 4);
        //We ignore the headers that the server sends back... it's pretty dirty in general to do that,
        //but it works here because the aac decoder skips it because it isn't valid aac data.
        return sock;
    }

}

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
    INFO("[APP] Start ..\n");

    strcpy(cfg.sta.ssid, WIFI_SSID);
    strcpy(cfg.sta.password, WIFI_PASS);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &cfg);
    esp_wifi_start();
    esp_wifi_connect();
    INFO("[APP] Create MQTT task\r\n");

    mqtt_settings mqtt_info = {
        .host = "192.168.0.1",
        .port = 1883,
        .client_id = "mqtt_client_id",
        .username = "user",
        .password = "pass",
        .clean_session = 0,
        .keepalive = 120,
        .lwt_topic = "/lwt",
        .lwt_msg = "offline",
        .connected_cb = mqtt_connected,
        .disconnected_cb = mqtt_disconnected,
        .reconnect_cb = mqtt_reconnect,
        .subscribe_cb = mqtt_subscribe,
        .publish_cb = mqtt_publish,
        .data_cb = mqtt_data
    };
    // Notice that, all callback will called in mqtt_task
    // All function publish, subscribe
    xTaskCreate(&mqtt_task, "mqtt_task", 2048, &mqtt_info, MQTT_PRIORITY, &xMqttTask);
    //xTaskCreatePinnedToCore(&download_task, "cpu2", 2048, buf2, 6, NULL, 1);
}
