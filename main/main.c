// Copyright [2024] [Hugo Trippaers]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_ieee802154.h>
#include <esp_log.h>
#include <esp_phy_init.h>
#include <esp_mac.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "ieee802154.h"

#define TAG "app_main"

#define SHORT_BROADCAST 0xFFFF

static QueueHandle_t radio_receiver;
static QueueHandle_t radio_sender;

void send_broadcast(uint16_t pan_id) {
    ESP_LOGI(TAG, "Send broadcast from pan %04x", pan_id);
    uint8_t buffer[256];

    uint8_t eui64[8];
    esp_ieee802154_get_extended_address(eui64);

    ieee802154_address_t src = {
            .mode = ADDR_MODE_LONG,
            .long_address = { eui64[0], eui64[1], eui64[2], eui64[3], eui64[4], eui64[5], eui64[6], eui64[7]}
    };

    ieee802154_address_t dst = {
            .mode = ADDR_MODE_SHORT,
            .short_address = SHORT_BROADCAST
    };

    uint8_t hdr_len = ieee802154_header(&pan_id, &src, &pan_id, &dst, false, &buffer[1], sizeof(buffer) - 1);

    // Add the local eui64 as payload
    memcpy(&buffer[1 + hdr_len], eui64, 8);

    // packet length
    buffer[0] = hdr_len + 8;

    xQueueSend(radio_sender, buffer, buffer[0]);
}
void app_main() {
    ESP_LOGI(TAG, "Initializing NVS from flash...");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    radio_receiver = xQueueCreate(5, 128);
    radio_sender = xQueueCreate(5, 128);

    if (radio_receiver == 0) {
        ESP_LOGE("app_main", "Failed to create receiver queue");
        return;
    }

    if (radio_sender == 0) {
        ESP_LOGE("app_main", "Failed to create sender queue");
        return;
    }

    ESP_ERROR_CHECK(ieee802154_init(0x4242, 13, radio_receiver, radio_sender));

    uint8_t frame[128];
    while (true) {
        send_broadcast(0x4242);

        BaseType_t res = xQueueReceive(radio_receiver, frame, 0);
        if (res) {
            ESP_LOGI("app_main", "Got %d bytes on the radio", frame[0]);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}