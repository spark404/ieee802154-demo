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

#include <esp_log.h>
#include <esp_ieee802154.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "ieee802154_statics.h"

void sender_task(void *pvParameters) {
    uint8_t frame[128]; // IEEE802.15.4 max is 127

    if (sender_queue == NULL) {
        ESP_LOGE("sender_task", "Failed to start, sender_queue == NULL");
        return;
    }

    while (true) {
        BaseType_t result = xQueueReceive(sender_queue, frame, pdMS_TO_TICKS(500));
        if (result)
            esp_ieee802154_transmit(frame, false);
    }
}