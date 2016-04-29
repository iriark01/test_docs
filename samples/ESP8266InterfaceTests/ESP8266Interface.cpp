/* ESP8266 implementation of NetworkInterfaceAPI
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

#include "ESP8266Interface.h"

// Various timeouts for different ESP8266 operations
#define ESP8266_CONNECT_TIMEOUT 15000
#define ESP8266_SEND_TIMEOUT    500
#define ESP8266_RECV_TIMEOUT    0
#define ESP8266_MISC_TIMEOUT    500


// ESP8266Interface implementation
ESP8266Interface::ESP8266Interface(PinName tx, PinName rx, bool debug)
    : _esp(tx, rx, debug)
{
    memset(_ids, 0, sizeof(_ids));
}

ESP8266Interface::~ESP8266Interface()
{
}

int32_t ESP8266Interface::connect(
    const char *ap, 
    const char *pass_phrase, 
    ns_security_t)
{
    _esp.setTimeout(ESP8266_CONNECT_TIMEOUT);

    if (!_esp.startup(3)) {
        return NS_ERROR_DEVICE_ERROR;
    }

    if (!_esp.dhcp(true, 1)) {
        return NS_ERROR_DHCP_FAILURE;
    }

    if (!_esp.connect(ap, pass_phrase)) {
        return NS_ERROR_NO_CONNECTION;
    }

    if (!_esp.getIPAddress()) {
        return NS_ERROR_DHCP_FAILURE;
    }

    return 0;
}

int32_t ESP8266Interface::disconnect()
{
    _esp.setTimeout(ESP8266_MISC_TIMEOUT);

    if (!_esp.disconnect()) {
        return NS_ERROR_DEVICE_ERROR;
    }

    return 0;
}

const char *ESP8266Interface::getIPAddress()
{
    return _esp.getIPAddress();
}

const char *ESP8266Interface::getMACAddress()
{
    return _esp.getMACAddress();
}

SocketInterface *ESP8266Interface::createSocket(ns_protocol_t proto)
{
    // Look for an unused socket
    int id = -1;

    for (int i = 0; i < ESP8266_SOCKET_COUNT; i++) {
        if (!_ids[i]) {
            id = i;
            _ids[i] = true;
            break;
        }
    }

    if (id == -1) {
        return 0;
    }

    return new ESP8266Socket(&_esp, proto, id);
}

void ESP8266Interface::destroySocket(SocketInterface *iface)
{
    ESP8266Socket *socket = (ESP8266Socket *)iface;
    _ids[socket->_id] = false;
    delete socket;
}


// ESP8266Socket implementation
int32_t ESP8266Interface::ESP8266Socket::open(const char *ip, uint16_t port)
{
    _esp->setTimeout(ESP8266_MISC_TIMEOUT);

    const char *proto = (_proto == NS_UDP) ? "UDP" : "TCP";

    if (!_esp->open(proto, _id, ip, port)) {
        return NS_ERROR_TIMEOUT;
    }

    return 0;
}

int32_t ESP8266Interface::ESP8266Interface::ESP8266Socket::close()
{
    _esp->setTimeout(ESP8266_MISC_TIMEOUT);

    if (!_esp->close(_id)) {
        return NS_ERROR_TIMEOUT;
    }

    return 0;
}

int32_t ESP8266Interface::ESP8266Socket::send(const void *data, uint32_t size)
{
    _esp->setTimeout(ESP8266_SEND_TIMEOUT);

    if (!_esp->send(_id, data, size)) {
        return NS_ERROR_TIMEOUT;
    }

    return size;
}

int32_t ESP8266Interface::ESP8266Socket::recv(void *data, uint32_t size)
{
    _esp->setTimeout(ESP8266_RECV_TIMEOUT);

    int32_t recv = _esp->recv(_id, data, size);

    if (recv < 0) {
        return NS_ERROR_WOULD_BLOCK;
    }

    return recv;
}

