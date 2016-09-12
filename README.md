# ESP32 MQTT Library sample application

- Dependency library: https://github.com/tuanpmt/espmqtt

## Prerequire

- ESP32 compiler: 
- ESP-IDF: https://github.com/espressif/esp-idf

## Compile

`git clone --recursive https://github.com/tuanpmt/esp32-mqtt`

Run command `make menuconfig` to change SERIAL PORT configuration and the options at `Component config -> MQTT `:

- `CONFIG_MQTT_PROTOCOL_311`: default is yes
- `CONFIG_MQTT_SECURITY_ON`: default is yes (but not support yet)
- `CONFIG_MQTT_PRIORITY`: default 5
- `CONFIG_MQTT_LOG_ERROR_ON`: defaut is yes
- `CONFIG_MQTT_LOG_WARN_ON`: defaut is yes
- `CONFIG_MQTT_LOG_INFO_ON`: defaut is yes
- `CONFIG_MQTT_RECONNECT_TIMEOUT`: defaut is 60 seconds
- `CONFIG_MQTT_QUEUE_BUFFER_SIZE_WORD`: default is 1024 (4096 bytes)
- `CONFIG_MQTT_BUFFER_SIZE_BYTE`: default is 1024 bytes
- `CONFIG_MQTT_MAX_HOST_LEN`: default is 64 bytes
- `CONFIG_MQTT_MAX_CLIENT_LEN`: default is 32 bytes
- `CONFIG_MQTT_MAX_USERNAME_LEN`: default is 32 bytes
- `CONFIG_MQTT_MAX_PASSWORD_LEN`: default is 32 bytes
- `CONFIG_MQTT_MAX_LWT_TOPIC`: default is 32 bytes
- `CONFIG_MQTT_MAX_LWT_MSG`: default is 32 bytes

Copy `main/include/user_config.sample.h` to `main/include/user_config.local.h` and provide `WIFI_SSID`, `WIFI_PASS`

`make && make flash`

## Usage 

```c
 mqtt_settings settings = {
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

mqtt_start(&settings);
```

## Todo list

- [ ] Create MQTT task server all protocol defined - Support subscribing, publishing, authentication, will messages, keep alive pings and all 3 QoS levels (it should be a fully functional client).
- [ ] Support mbedtls for SSL connection
- [ ] Write document

## Progress:

- [x] Can connect, subscribe, support queue for subscribe, support lwt msg
- [x] Direct send keepalive packet 
- [ ] Can publish 
- [ ] Can receive and process publish qos 0
- [ ] Can receive and process publish qos 1 and 2 

## License

Apache 2.0
