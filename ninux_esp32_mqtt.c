
#include "ninux_esp32_mqtt.h"


void ninux_mqtt_init(esp_err_t (*f)(esp_mqtt_event_handle_t)){
	mqtt_app_start((*f));
        xEventGroupWaitBits(mqtt_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}

void ninux_mqtt_publish(char* topic, char* data){
    	int msg_id;
	bzero(mqtt_data,sizeof(mqtt_data));
	bzero(mqtt_topic,sizeof(mqtt_topic));
	sprintf(mqtt_data,"%s",data);
	sprintf(mqtt_topic,"%s",topic);
        //esp_mqtt_client_start(mqtt_client);
        //xEventGroupWaitBits(mqtt_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
        msg_id = esp_mqtt_client_publish(mqtt_client, mqtt_topic, mqtt_data, 0, 0, 0);
}

void ninux_mqtt_set_topic(char* topic){
	sprintf(input_mqtt_topic,"%s",topic);
}

static void mqtt_app_start(esp_err_t (*f)(esp_mqtt_event_handle_t))
{
    mqtt_event_group = xEventGroupCreate();
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URL,
        .event_handle = (*f),
        // .user_context = (void *)your_context
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(mqtt_client);
}
