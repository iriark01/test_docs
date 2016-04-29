#include "NetworkInterface.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include <stdio.h>
#include "string.h"

#define NSAPI_MAX_DATA_SIZE 500

char expected_data[NSAPI_MAX_DATA_SIZE];
char received_data[NSAPI_MAX_DATA_SIZE];

int nsapi_ni_isConnected_test(NetworkInterface *iface)
{
  return !(iface->isConnected());
}

int nsapi_ni_getIPAddress_test(NetworkInterface *iface)
{

  if (!iface->getIPAddress()[0]) {
    printf("'getIpAddress' did not return an IP address\r\n");
    return -1;
  }

  return 0;
}

int nsapi_ni_getMACAddress_test(NetworkInterface *iface)
{

  if (!iface->getMACAddress()[0]) {
    printf("'getMacAddress' did not return a MAC address\r\n");
    return -1;
  }

  return 0;
}

int nsapi_ni_getHostByName_test(NetworkInterface *iface)
{
  char ip_address[NS_IP_SIZE] = "\0";

  int32_t ret = iface->getHostByName("developer.mbed.org", ip_address);

  if (ret) {
    printf("'getHostByName' failed with code %d\r\n", ret);
    return -1;
  } else if (!ip_address[0]) {
    printf("Returned IP address was null\r\n");
    return -2;
  } else {
    return 0;
  }
}

int nsapi_socket_open_test(Socket *socket, const char *test_address, uint16_t test_port)
{
  int32_t ret = socket->open(test_address, test_port);

  if (ret) {
    printf("'open' failed with code %d\r\n", ret);
    return -1;
  } else {
    return 0;
  }
}

int nsapi_socket_isConnected_test(Socket *socket, const char *test_address, uint16_t test_port)
{
  return !socket->isConnected();
}


int nsapi_socket_getIpAddress_test(Socket *socket, const char *test_address, uint16_t test_port)
{
  const char *cur_ip_address = socket->getIPAddress();

  if (!cur_ip_address[0]) {
    printf("'getIpAddress' did not return an IP address\r\n");
    return -1;
  } else if (strcmp(cur_ip_address, test_address)) {
    printf("'getIpAddress' did not reutrurn the expected IP address\r\n");
    printf("Expected: %s\r\n", test_address);
    printf("Actual: %s\r\n", cur_ip_address);
    return -2;
  }

  return 0;
}

int nsapi_socket_getPort_test(Socket *socket, const char *test_address, uint16_t test_port)
{
  uint16_t cur_port = socket->getPort();

  if (cur_port != test_port) {
    printf("'getPort' did not return the expected port\r\n");
    printf("Expected: %d\r\n", test_port);
    printf("Actual: %d\r\n", cur_port);
    return -1;
  }

  return 0;
}

void nsapi_socket_test_setup(char *buffer, int size)
{
  int i;
  for (i = 0; i < size; i++) {
    buffer[i] = '0' + (i % 10);
  }
}

int nsapi_socket_blocking_test_helper(Socket *socket, const char *test_address, uint16_t test_port, uint32_t data_size)
{
  nsapi_socket_test_setup(expected_data, data_size);

  int32_t ret = socket->send(expected_data, data_size);

  if (ret) {
    printf("'send' failed during test with code %d\r\n", ret);
    return -4;
  }

  int32_t bytes_received = socket->recv(received_data, sizeof(received_data));

  if (bytes_received >= (int32_t)data_size) {
    if (memcmp(received_data, expected_data, data_size) == 0) {
      return 0;
    } else {
      printf("'recv' returned incorrect data with length %d\r\n", bytes_received);
      return -2;
    }
  } else if (bytes_received < 0) {
    printf("'recv' failed with code %d\r\n", bytes_received);
    return -3;
  } else {
    printf("'recv' returned no data\r\n");
    return -1;
  }
}

int nsapi_socket_blocking_test(Socket *socket, const char *test_address, uint16_t test_port)
{
  int32_t result, i;
  int32_t packet_sizes[] = {10, 100, 500};
  int32_t num_packet_sizes = 3;

  for (i = 0; i < num_packet_sizes; i++) {
    result = nsapi_socket_blocking_test_helper(socket, test_address, test_port, packet_sizes[i]);

    if (result) {
      printf("nsapi_socket_blocking_test failed with data size %d\r\n", packet_sizes[i]);
      break;
    }
  }

  if (i >= num_packet_sizes) {
    return 0;
  } else {
    return -num_packet_sizes;
  }
}

int nsapi_socket_non_blocking_test_helper(Socket *socket, const char *test_address, uint16_t test_port, uint32_t data_size)
{
  int32_t bytes_received;
  int result = -1;

  nsapi_socket_test_setup(expected_data, data_size);

  // First check to make sure `recv` will not block and return 0 for bytes
  // received. If the tests do proceed after this test, be sure your `recv`
  // respects the `blocking` flag
  bytes_received = socket->recv(received_data, NSAPI_MAX_DATA_SIZE, false);

  if (bytes_received > 0) {
    printf("'recv' returned data of length %d when no data was expected\r\n", bytes_received);
    return -5;
  } else if (bytes_received < 0) {
    printf("'recv' failed when no data was expected with code %d\r\n", bytes_received);
    return -5;
  }

  int32_t ret = socket->send(expected_data, data_size);

  if (ret) {
    printf("'send' failed during test with code %d\r\n", ret);
    return -4;
  }

  // TODO: Create a better way to "wait" for data besides busy-looping
  for (int i = 0; i < 10000000; i++) {
    bytes_received = socket->recv(received_data, data_size, false);

    if (bytes_received >= (int32_t)data_size) {
      if (memcmp(received_data, expected_data, data_size) == 0) {
        result = 0;
        break;
      } else {
        result =  -2;
        break;
      }
    } else if (bytes_received < 0) {
      result = -3;
      break;
    }
  }

  if (result == -1) {
    printf("'recv' returned no data\r\n");
  } else if (result == -2) {
    printf("'recv' returned incorrect data with length %d\r\n", bytes_received);
  } else if (result == -3) {
    printf("'recv' failed with code %d\r\n", bytes_received);
  }

  return result;
}

int nsapi_socket_non_blocking_test(Socket *socket, const char *test_address, uint16_t test_port)
{
  int32_t result, i;
  int32_t packet_sizes[] = {10, 100, 500};
  int32_t num_packet_sizes = 3;

  for (i = 0; i < num_packet_sizes; i++) {
    result = nsapi_socket_non_blocking_test_helper(socket, test_address, test_port, packet_sizes[i]);

    if (result) {
      printf("nsapi_socket_non_blocking_test failed with data size of %d\r\n", packet_sizes[i]);
      break;
    }
  }

  if (i >= num_packet_sizes) {
    return 0;
  } else {
    return -num_packet_sizes;
  }
}

int nsapi_socket_close_test(Socket *socket, const char *test_address, uint16_t test_port)
{
  int32_t ret = socket->close();

  if (ret) {
    printf("'close' failed with code %d\r\n", ret);
    return -1;
  } else {
    return 0;
  }
}

void nspai_print_test_header(const char *name) {
  printf("---------------------\r\n");
  printf("%s: running...\r\n", name);
}

void nspai_print_test_result(const char *name, int result) {
  printf("%s: ", name);

  if (!result) {
    printf("PASS\r\n");
  } else {
    printf("FAIL (Return code %d)\r\n", result);
  }
}

int nsapi_ni_run_test(const char *name, NetworkInterface *iface, int (*test)(NetworkInterface*)) {
  int result;
  nspai_print_test_header(name);
  result = test(iface);
  nspai_print_test_result(name, result);
  return result;
}

int nsapi_socket_run_test(const char *name, Socket *socket, const char *test_address, uint16_t test_port, int (*test)(Socket*, const char*, uint16_t)) {
  int result;
  nspai_print_test_header(name);
  result = test(socket, test_address, test_port);
  nspai_print_test_result(name, result);
  return result;
}

int nsapi_tests(const char *name, NetworkInterface *iface, const char *test_address, uint16_t test_port)
{
  TCPSocket tcp_socket(iface);
  UDPSocket udp_socket(iface);

  int ret = 0;

  printf("--- Running NetworkInterface Tests ---\r\n\r\n");
  ret |= nsapi_ni_run_test("nsapi_ni_isConnected_test", iface, &nsapi_ni_isConnected_test);
  ret |= nsapi_ni_run_test("nsapi_ni_getIPAddress_test", iface, &nsapi_ni_getIPAddress_test);
  ret |= nsapi_ni_run_test("nsapi_ni_getMACAddress_test", iface, &nsapi_ni_getMACAddress_test);
  ret |= nsapi_ni_run_test("nsapi_ni_getHostByName_test", iface, &nsapi_ni_getHostByName_test);

  printf("\r\n\r\n--- Running TCPSocket Tests ---\r\n\r\n");
  ret |= nsapi_socket_run_test("nsapi_socket_open_test", &tcp_socket, test_address, test_port, &nsapi_socket_open_test);
  ret |= nsapi_socket_run_test("nsapi_socket_getIpAddress_test", &tcp_socket, test_address, test_port, &nsapi_socket_getIpAddress_test);
  ret |= nsapi_socket_run_test("nsapi_socket_getPort_test", &tcp_socket, test_address, test_port, &nsapi_socket_getPort_test);
  ret |= nsapi_socket_run_test("nsapi_socket_blocking_test", &tcp_socket, test_address, test_port, &nsapi_socket_blocking_test);
  ret |= nsapi_socket_run_test("nsapi_socket_non_blocking_test", &tcp_socket, test_address, test_port, &nsapi_socket_non_blocking_test);
  ret |= nsapi_socket_run_test("nsapi_socket_close_test", &tcp_socket, test_address, test_port, &nsapi_socket_close_test);


  printf("\r\n\r\n--- Running UDPSocket Tests ---\r\n\r\n");
  ret |= nsapi_socket_run_test("nsapi_socket_open_test", &udp_socket, test_address, test_port, &nsapi_socket_open_test);
  ret |= nsapi_socket_run_test("nsapi_socket_getIpAddress_test", &udp_socket, test_address, test_port, &nsapi_socket_getIpAddress_test);
  ret |= nsapi_socket_run_test("nsapi_socket_getPort_test", &udp_socket, test_address, test_port, &nsapi_socket_getPort_test);
  ret |= nsapi_socket_run_test("nsapi_socket_blocking_test", &udp_socket, test_address, test_port, &nsapi_socket_blocking_test);
  ret |= nsapi_socket_run_test("nsapi_socket_non_blocking_test", &udp_socket, test_address, test_port, &nsapi_socket_non_blocking_test);
  ret |= nsapi_socket_run_test("nsapi_socket_close_test", &udp_socket, test_address, test_port, &nsapi_socket_close_test);

  if (ret == 0) {
    printf("\r\n\r\n--- ALL TESTS PASSING ---\r\n");
  } else {
    printf("\r\n\r\n--- TEST FAILURES OCCURRED ---\r\n");
  }

  return ret != 0;
}

