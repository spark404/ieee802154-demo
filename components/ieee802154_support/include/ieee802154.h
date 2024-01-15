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

#ifndef EPD_STATION_IEEE802154_H
#define EPD_STATION_IEEE802154_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <esp_err.h>

#define ADDR_MODE_NONE     (0)  // PAN ID and address fields are not present
#define ADDR_MODE_RESERVED (1)  // Reserved
#define ADDR_MODE_SHORT    (2)  // Short address (16-bit)
#define ADDR_MODE_LONG     (3)  // Extended address (64-bit)

typedef struct {
    uint8_t mode; // ADDR_MODE_NONE || ADDR_MODE_SHORT || ADDR_MODE_LONG
    union {
        uint16_t short_address;
        uint8_t long_address[8];
    };
} ieee802154_address_t;

esp_err_t ieee802154_init(uint16_t pan_id, uint8_t channel, QueueHandle_t receiver, QueueHandle_t sender);
uint8_t ieee802154_header(const uint16_t *src_pan, ieee802154_address_t *src, const uint16_t *dst_pan,
                          ieee802154_address_t *dst, uint8_t ack, uint8_t *header, uint8_t header_length);

#endif  // EPD_STATION_IEEE802154_H
