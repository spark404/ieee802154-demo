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

#include "ieee802154_support.h"

void reverse_memcpy(uint8_t *restrict dst, const uint8_t *restrict src, size_t n)
{
    size_t i;

    for (i=0; i < n; ++i) {
        dst[n - 1 - i] = src[i];
    }
}