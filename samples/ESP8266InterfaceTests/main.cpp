/* NetworkSocketAPI Example Program
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "ESP8266Interface.h"
#include "NSAPITests.h"

ESP8266Interface iface(D1, D0);

int main()
{
    int32_t result = iface.connect("ssid", "password");

    if (result) {
        printf("Interface failed to connect with code %d\r\n", result);
    } else {
        nsapi_tests("ESP8266Interface Tests", &iface, "0.0.0.0", 4000);
        iface.disconnect();
    }
    while(1);
}
