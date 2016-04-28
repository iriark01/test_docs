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
#include "rtos.h"
#include "LWIPInterface.h"
#include "TCPSocket.h"

LWIPInterface eth;
Semaphore network_sem(1);

DigitalOut blink_led(LED_GREEN);
DigitalOut network_led(LED_BLUE);

void blink()
{
    blink_led = !blink_led;
}

void network_callback()
{
    network_led = !network_led;
    network_sem.release();
}

int main()
{
    Ticker blinky;
    blinky.attach(blink, 0.4f);

    printf("NetworkSocketAPI Example\r\n");

    eth.connect();
    const char *ip = eth.get_ip_address();
    const char *mac = eth.get_mac_address();
    printf("IP address is: %s\r\n", ip ? ip : "No IP");
    printf("MAC address is: %s\r\n", mac ? mac : "No MAC");
    
    SocketAddress addr(&eth, "mbed.org");
    printf("mbed.org resolved to: %s\r\n", addr.get_ip_address());

    TCPSocket ipv4socket(&eth);
    ipv4socket.connect("4.ifcfg.me", 23);
    ipv4socket.set_blocking(false);
    ipv4socket.attach(network_callback);

    TCPSocket ipv6socket(&eth);
    ipv6socket.connect("6.ifcfg.me", 23);
    ipv6socket.set_blocking(false);
    ipv6socket.attach(network_callback);

    // Tries to get both an IPv4 and IPv6 address
    while (network_sem.wait(2500) > 0) {
        int count;
        char buffer[64];

        count = ipv4socket.recv(buffer, sizeof buffer);
        if (count >= 0) {
            printf("public IPv4 address is: %.15s\r\n", &buffer[15]);
        }

        count = ipv6socket.recv(buffer, sizeof buffer);
        if (count >= 0) {
            printf("public IPv6 address is: %.39s\r\n", &buffer[15]);
        }
    }
    
    ipv4socket.close();
    ipv6socket.close();
    eth.disconnect();

    printf("Done\r\n");
}
