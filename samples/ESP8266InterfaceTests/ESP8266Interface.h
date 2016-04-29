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

#ifndef ESP8266_INTERFACE_H
#define ESP8266_INTERFACE_H

#include "WiFiInterface.h"
#include "ESP8266.h"

#define ESP8266_SOCKET_COUNT 5

/** ESP8266Interface class
 *  Implementation of the NetworkInterface for the ESP8266
 */
class ESP8266Interface : public WiFiInterface
{
public:

    ESP8266Interface(PinName tx, PinName rx, bool debug = false);
    virtual ~ESP8266Interface();

    // Implementation of WiFiInterface
    virtual int32_t connect(
        const char *ssid,
        const char *pass,
        ns_security_t security = NS_SECURITY_NONE);

    virtual int32_t disconnect();

    // Implementation of NetworkInterface
    virtual const char *getIPAddress();
    virtual const char *getMACAddress();

    virtual SocketInterface *createSocket(ns_protocol_t proto);
    virtual void destroySocket(SocketInterface *socket);

private:

    ESP8266 _esp;
    bool _ids[ESP8266_SOCKET_COUNT];
    
    // Implementation of the SocketInterface for the ESP8266
    class ESP8266Socket : public SocketInterface
    {
    public:
    
        // ESP8266 specific details
        ESP8266 *_esp;
        ns_protocol_t _proto;
        int _id;
        
        ESP8266Socket(ESP8266 *esp, ns_protocol_t proto, int id)
            : _esp(esp), _proto(proto), _id(id) {}
    
        // Implementation of SocketInterface
        virtual int32_t open(const char *ip, uint16_t port);
        virtual int32_t close();
    
        virtual int32_t send(const void *data, uint32_t size);
        virtual int32_t recv(void *data, uint32_t size);
    };
};

#endif
