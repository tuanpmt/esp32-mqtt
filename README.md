# ESP32 MQTT Library sample application

## Prerequire

- ESP32 compiler: 
- ESP-IDF: 

## Compile

`git clone --recursive https://github.com/tuanpmt/esp32-mqtt`

Run command `make menuconfig` change SERIAL PORT configuration and change the options at `Component config -> MQTT `

`make && make flash`

## Usage 

```
 mqtt_settings mqtt_info = {
    .host = "192.168.0.1",  // or domain, ex: "google.com",
    .port = 1883,
    .client_id = "mqtt_client_id", 
    .username = "user",
    .password = "pass",
    .clean_session = 0, 
    .keepalive = 120, //second
    .lwt_topic = "/lwt",    // = "" for disable lwt, will don't care other options
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
// Notice that, all callback will called in mqtt_task
// All function publish, subscribe
xTaskCreate(&mqtt_task, "mqtt_task", 2048, &mqtt_info, MQTT_PRIORITY, &xMqttTask);
```
