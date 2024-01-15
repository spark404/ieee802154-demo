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

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "ieee802154_statics.h"
#include "ieee802154_tasks.h"

#include <esp_err.h>
#include <esp_ieee802154.h>
#include <esp_mac.h>

QueueHandle_t receiver_queue;
QueueHandle_t sender_queue;
TaskHandle_t sender_task_handle;

extern void dummy_function();

esp_err_t ieee802154_init(uint16_t pan_id, uint8_t channel, QueueHandle_t receiver, QueueHandle_t sender) {
    dummy_function();

    esp_err_t err = esp_ieee802154_enable();
    if (err != ESP_OK)
        return err;

    err = esp_ieee802154_set_coordinator(false);
    if (err != ESP_OK)
        return err;

    err = esp_ieee802154_set_promiscuous(false);
    if (err != ESP_OK)
        return err;

    err = esp_ieee802154_set_rx_when_idle(true);
    if (err != ESP_OK)
        return err;

    err = esp_ieee802154_set_panid(pan_id);
    if (err != ESP_OK)
        return err;

    err = esp_ieee802154_set_channel(channel);
    if (err != ESP_OK)
        return err;

    uint8_t eui64[8] = {0};
    esp_read_mac(eui64, ESP_MAC_IEEE802154);
    esp_ieee802154_set_extended_address(eui64);

    receiver_queue = receiver;
    sender_queue = sender;

    BaseType_t res = xTaskCreate(sender_task, "ieee802154_sender", 2048, NULL, 10, &sender_task_handle);
    if (!res) {
        return ESP_FAIL;
    }

    // Enable the receiver
    return esp_ieee802154_receive();
}