
#include "ninux_esp32_mqtt.h"

void ninux_mqtt_init(){
	mqtt_app_start();
}

void ninux_mqtt_publish(char* topic, char* data){
	mqtt_app_start();
	bzero(mqtt_data,sizeof(mqtt_data));
	bzero(mqtt_topic,sizeof(mqtt_topic));
	sprintf(mqtt_data,"%s",data);
	sprintf(mqtt_topic,"%s",topic);
}

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    //char mqtt_topic[128];
    //extern char* mqtt_data;
    //extern char* mqtt_topic;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG2, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, mqtt_topic, mqtt_data, 0, 0, 0);
            ESP_LOGI(TAG2, "sent publish successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG2, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            ESP_LOGI(TAG2, "sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            ESP_LOGI(TAG2, "sent unsubscribe successful, msg_id=%d", msg_id);
	    //esp_mqtt_client_destroy(client);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG2, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG2, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAG2, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG2, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG2, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG2, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG2, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG2, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
        .event_handle = mqtt_event_handler,
        // .user_context = (void *)your_context
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
}
