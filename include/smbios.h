#pragma once

#include <attrs.h>
#include <pra_err.h>
#include <stdint.h>
#include <stddef.h>

typedef struct
{
    int8_t entry_point_string[4];  // This is _SM_
    uint8_t checksum;              // This value summed with all the values of the table, should be 0 (overflow)
    uint8_t length;                // Length of the Entry Point Table. Since version 2.1 of SMBIOS, this is 0x1F
    uint8_t major_version;         // Major Version of SMBIOS
    uint8_t minor_version;         // Minor Version of SMBIOS
    uint16_t max_structure_size;   // Maximum size of a SMBIOS Structure (we will se later)
    uint8_t entry_point_revision;  //...
    int8_t formatted_area[5];      //...
    int8_t entry_point_string2[5]; // This is _DMI_
    uint8_t checksum2;             // Checksum for values from EntryPointString2 to the end of table
    uint16_t table_length;         // Length of the Table containing all the structures
    uint32_t table_address;        // Address of the Table
    uint16_t number_of_structures; // Number of structures in the table
    uint8_t bcd_revision;          // Unused
} PACKED smbios_entry_point_t;

typedef struct
{
    uint8_t type;
    uint8_t length;
    uint16_t handle;
} PACKED smbios_header_t;

typedef struct
{
    smbios_header_t header;
    uint8_t socket_designation;
    uint8_t type;
    uint8_t family;
    uint8_t manufacturer;
    uint64_t id;
    uint8_t version;
    uint8_t voltage;
    uint16_t external_clock;
    uint16_t max_speed;
    uint16_t current_speed;
    uint8_t status;
    uint8_t upgrade;
    uint16_t l1_cache_handle;
    uint16_t l2_cache_handle;
    uint16_t l3_cache_handle;
    uint8_t serial_number;
    uint8_t asset_tag;
    uint8_t part_number;
    uint8_t core_count;
    uint8_t core_enabled;
    uint8_t thread_count;
    uint16_t characteristics;
    uint16_t processor_family_2;
} PACKED smbios_processor_information_t;

pra_err_t smbios_init();
size_t smbios_table_length(smbios_header_t *header);
pra_err_t smbios_print_version();
