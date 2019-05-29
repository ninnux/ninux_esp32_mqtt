#ifndef NINUX_MQTT
#define NINUX_MQTT
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "mqtt_client.h"

static const char *TAG2 = "ninux_mqtt";

char mqtt_data[512];
char mqtt_topic[512];
char input_mqtt_topic[512];

EventGroupHandle_t mqtt_event_group;
//extern const int CONNECTED_BIT = BIT0;
extern const int CONNECTED_BIT;

static esp_mqtt_client_handle_t mqtt_client = NULL;

void ninux_mqtt_init(void);
void ninux_mqtt_subscribe_topic(char *);
void ninux_mqtt_publish(char* topic, char* data);
static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);
static void mqtt_app_start(void);
#endif
