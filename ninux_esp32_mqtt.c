
#include "ninux_esp32_mqtt.h"


void ninux_mqtt_init(void){
	mqtt_app_start();
}
void ninux_mqtt_publish(char* topic, char* data){
    	int msg_id;
	bzero(mqtt_data,sizeof(mqtt_data));
	bzero(mqtt_topic,sizeof(mqtt_topic));
	sprintf(mqtt_data,"%s",data);
	sprintf(mqtt_topic,"%s",topic);
        
        esp_mqtt_client_stop(mqtt_client);
        esp_mqtt_client_start(mqtt_client);
        //esp_mqtt_client_set_uri(mqtt_client, CONFIG_EXAMPLE_BROKER_TCP_URI);
        ESP_LOGI(TAG2, "Note free memory: %d bytes", esp_get_free_heap_size());
        xEventGroupWaitBits(mqtt_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);

        msg_id = esp_mqtt_client_publish(mqtt_client, mqtt_topic, mqtt_data, 0, 0, 0);
	xEventGroupClearBits(mqtt_event_group, CONNECTED_BIT);
}

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG2, "MQTT_EVENT_CONNECTED");
            xEventGroupSetBits(mqtt_event_group, CONNECTED_BIT);
            //msg_id = esp_mqtt_client_subscribe(mqtt_client, CONFIG_EXAMPLE_SUBSCIBE_TOPIC, qos_test);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG2, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG2, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            //msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            //ESP_LOGI(TAG2, "sent publish successful, msg_id=%d", msg_id);
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
    mqtt_event_group = xEventGroupCreate();
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
        .event_handle = mqtt_event_handler,
        // .user_context = (void *)your_context
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
}
