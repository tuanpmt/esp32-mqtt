# ESP32 MQTT Library sample application

- Dependency library: https://github.com/tuanpmt/espmqtt

## Prerequire

- ESP32 compiler: 
- ESP-IDF: https://github.com/espressif/esp-idf

## Compile

`git clone --recursive https://github.com/tuanpmt/esp32-mqtt`

Run command `make menuconfig` to change SERIAL PORT configuration and the options at `Component config -> MQTT `:

- `CONFIG_MQTT_PROTOCOL_311`: default is yes
- `CONFIG_MQTT_SECURITY_ON`: default is yes 
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
    .connected_cb = connected_cb, // trigger when client connected to broker with valid infomations
    .disconnected_cb = disconnected_cb, //trigger when client disconnect from broker
    .reconnect_cb = reconnect_cb, //trigger when client reconnect - not implement yet
    .subscribe_cb = subscribe_cb, //trigger when client subscribe a topic successful 
    .publish_cb = publish_cb, //trigger when client publish data to channel successful 
    .data_cb = data_cb //trigger when client receive data from channel has subscribed
};

mqtt_client *client = mqtt_start(&settings);
```

### Data receive from channel subscribed:

```c
void data_cb(void *self, void *params)
{
    mqtt_client *client = (mqtt_client *)self;
    mqtt_event_data_t *event_data = (mqtt_event_data_t *)params;
}

typedef struct mqtt_event_data_t
{
  char* topic;  //point to topic 
  char* data; //point to data
  uint16_t topic_length;
  uint16_t data_length; //current packet length
  uint16_t data_offset; //current packet offset
  uint16_t data_total_length; //total packet length
} mqtt_event_data_t;
```

## Todo list

- [x] Create MQTT task server all protocol defined - Support subscribing, publishing, authentication, will messages, keep alive pings and all 3 QoS levels (it should be a fully functional client).
- [ ] Support mbedtls for SSL connection
- [ ] Write document

## Progress:

- [x] Can connect, subscribe, support queue for subscribe, support lwt msg
- [x] Direct send keepalive packet 
- [x] Can publish 
- [x] Can receive and process publish qos 0
- [x] Can receive and process publish qos 1 and 2 
- [ ] Implement all events

## License

Apache 2.0
