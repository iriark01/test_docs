# Just looking at the BLE doxygne



[![View library](https://www.mbed.com/embed/?type=library)](https://docs.mbed.com/docs/ble-api/en/master/api/classBLE.html)



# And at tables

Callback parameter|Buffer content
|------------------|-------------|
|`PANA_SERVER_MATERIAL_UPDATE`|Buffer contains server key material. The size is defined by the symbol<br> `PANA_SERVER_MATERIAL_BUF_SIZE`.|
|`PANA_SERVER_CLIENT_SESSION_UPDATE`|Add a new PANA session or update the previously stored one. All three segments are valid.<br> An offset number is used to determine which session this is. The offset number for new sessions is zero.<br> The callback should return the offset number for the sessions; this information is not used by the stack but is provided back on the following update.|
|`PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE`|Update only the client session data. The first two segments are stored in the buffer.<br>The callback should use the offset field to determine which session data this is.|
|`PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE`|Remove the previously stored session. Only the first segment is stored in the buffer.<br>The callback should use the offset field to determine which session data this is.|

# Another table


| Symbol             | Description                                                                |
|------------------- |----------------------------------------------------------------------------|
| `FLASH_ORIGIN`     | Origin of the physical flash memory where uVisor code is placed            |
| `FLASH_OFFSET`     | Offset in flash at which uVisor is located                                 |
| `SRAM_ORIGIN`      | Origin of the physical SRAM memory where uVisor .bss and stacks are placed |
| `SRAM_OFFSET`      | Offset in SRAM at which uVisor .bss and stacks are located                 |
| `FLASH_LENGTH_MIN` | min( [`FLASH_LENGTH(i)` for `i` in family's devices] )                     |
| `SRAM_LENGTH_MIN`  | min( [`SRAM_LENGTH(i)` for `i` in family's devices] )                      |
| `NVIC_VECTORS`     | max( [`NVIC_VECTORS(i)` for `i` in family's devices] )                     |
| `CORE_*` *         | Core version (e.g `CORE_CORTEX_M3`)                                        |
