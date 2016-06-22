<details><summary>Sam, this is for you</summary><blockquote><p>This is what it will look like if you expand it</p></blockquote></details>


# Getting started with the network and socket interface

## Network Socket API

The Network Socket API provides a common interface for using [sockets](https://en.wikipedia.org/wiki/Network_socket) on network devices. The API provides a simple class-based interface that should be familiar to users experienced with other socket APIs. Additionally, the API provides a simple interface for implementing network devices, making it easy to connect hardware agnostic programs to new devices.

## Network Interface 

The [NetworkStack](https://docs.mbed.com/docs/testing-docs-features/en/latest/api/classNetworkStack.html) provides an abstract class for network devices that support sockets. Devices must inherit and implement the NetworkStack class and a network interface as well as adding implementation specific methods for using the device. A NetworkStack must be provided to a Socket constructor to open a socket on the interface. Currently defined network interfaces include:

* [EthernetInterface](https://docs.mbed.com/docs/testing-docs-features/en/latest/api/classEthernetInterface.html)
* [WiFiInterface](https://docs.mbed.com/docs/testing-docs-features/en/latest/api/classWiFiInterface.html)
* [CellularInterface](https://docs.mbed.com/docs/testing-docs-features/en/latest/api/classCellularInterface.html)
* [MeshInterface](https://docs.mbed.com/docs/testing-docs-features/en/latest/api/classMeshInterface.html)


## Sockets

The Socket class is used for managing network sockets. Once opened, the socket provides a pipe through which data can sent and received to a specific endpoint. The socket class can be instantiated as either a [TCPSocket](https://docs.mbed.com/docs/testing-docs-features/en/latest/api/classTCPSocket.html) or a [UDPSocket](https://docs.mbed.com/docs/testing-docs-features/en/latest/api/classUDPSocket.html) which defines the protocol used for the connection.

## Errors 

The convention for the NetworkSocketAPI is to have functions that may fail and return a signed integer. To indicate success, the function should return a **non-negative integer** which may also contain the size of a transaction. To indicate failure the function should return a negative integer which should be one of the following error codes from the **nsapi_error_t** enum:

```cpp

/** Enum of standardized error codes 
 *
 *  Valid error codes have negative values and may
 *  be returned by any network operation.
 *
 *  @enum nsapi_error_t
 */
enum nsapi_error_t {
    NSAPI_ERROR_WOULD_BLOCK   = -3001,     /*!< no data is not available but call is non-blocking */
    NSAPI_ERROR_UNSUPPORTED   = -3002,     /*!< unsupported configuration */
    NSAPI_ERROR_NO_CONNECTION = -3003,     /*!< not connected to a network */
    NSAPI_ERROR_NO_SOCKET     = -3004,     /*!< socket not available for use */
    NSAPI_ERROR_NO_ADDRESS    = -3005,     /*!< IP address is not known */
    NSAPI_ERROR_NO_MEMORY     = -3006,     /*!< memory resource not available */
    NSAPI_ERROR_DNS_FAILURE   = -3007,     /*!< DNS failed to complete successfully */
    NSAPI_ERROR_DHCP_FAILURE  = -3008,     /*!< DHCP failed to complete successfully */
    NSAPI_ERROR_AUTH_FAILURE  = -3009,     /*!< connection to access point failed */
    NSAPI_ERROR_DEVICE_ERROR  = -3010,     /*!< failure interfacing with the network processor */
};
```

## Example 

[![View code](https://www.mbed.com/embed/?url=https://github.com/iriark01/testing_docs_tools)](https://github.com/iriark01/testing_docs_tools/blob/master/samples/HelloLWIPInterface/HelloLWIPInterface.cpp)
