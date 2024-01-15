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

#include <esp_ieee802154.h>
#include <esp_log.h>

#include "ieee802154_statics.h"

#define RADIO_TAG "ieee802154_callbacks"

void dummy_function() {
    //dummy
}

void esp_ieee802154_receive_done(uint8_t* frame, esp_ieee802154_frame_info_t* frame_info) {
    ESP_EARLY_LOGI(RADIO_TAG, "rx OK, received %d bytes", frame[0]);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (receiver_queue != NULL)
        xQueueSendToBackFromISR(receiver_queue, frame, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken)
        taskYIELD();
}

void esp_ieee802154_receive_failed(uint16_t error) {
    ESP_EARLY_LOGI(RADIO_TAG, "rx failed, error %d", error);
}

void esp_ieee802154_receive_sfd_done(void) {
    ESP_EARLY_LOGI(RADIO_TAG, "rx sfd done, Radio state: %d", esp_ieee802154_get_state());
}

void __attribute__((used)) esp_ieee802154_transmit_done(const uint8_t *frame, const uint8_t *ack, esp_ieee802154_frame_info_t *ack_frame_info) {
    ESP_EARLY_LOGI(RADIO_TAG, "tx OK, sent %d bytes, ack %d", frame[0], ack != NULL);
}

void esp_ieee802154_transmit_failed(const uint8_t *frame, esp_ieee802154_tx_error_t error) {
    ESP_EARLY_LOGI(RADIO_TAG, "tx failed, error %d", error);
}

void esp_ieee802154_transmit_sfd_done(uint8_t *frame) {
    ESP_EARLY_LOGI(RADIO_TAG, "tx sfd done");
}