#include <smbios.h>
#include <kmalloc.h>
#include <io.h>
#include <string_utils.h>
#include <stddef.h>

static smbios_entry_point_t *smbios;

pra_err_t smbios_init()
{
    uint8_t *smbios_memory = (uint8_t*) 0xF0000;
    while ((uint32_t) smbios_memory < 0x100000)
    {
        if (smbios_memory[0] == '_' && smbios_memory[1] == 'S' && smbios_memory[2] == 'M' && smbios_memory[3] == '_')
        {
            size_t length = smbios_memory[5];
            uint8_t checksum = 0;
            for (size_t i = 0; i < length; i++)
            {
                checksum += smbios_memory[i];
            }
            if (checksum == 0)
                break;
        }
        smbios_memory += 16;
    }
    smbios = (smbios_entry_point_t*) smbios_memory;
    if ((uint32_t) smbios_memory == 0x100000)
        return PRA_FAIL;
    return PRA_OK;
}

size_t smbios_table_length(smbios_header_t *header)
{
    const uint8_t *strtab = (uint8_t*) header + header->length;
    size_t i;
    for (i = 1; strtab[i - 1] != '\0' || strtab[i] != '\0'; i++)
        ;
    return header->length + i + 1;
}

pra_err_t smbios_print_version()
{
    if (!smbios || (uint32_t) smbios >= 0x100000)
        return PRA_FAIL;
    io_printf("SMBIOS %d.%d\n", smbios->major_version, smbios->minor_version);

    smbios_header_t *header = (smbios_header_t*) smbios->table_address;
    for (;;)
    {
        if (header->type == 4)
            break;

        size_t table_length = smbios_table_length(header);
        header = (smbios_header_t*) ((uint8_t*) header + table_length);
    }

    // uint8_t *strings = (uint8_t*) header + header->length;
    smbios_processor_information_t *processor = (smbios_processor_information_t*) header;

    io_printf("Max Speed: %d MHz\n", processor->max_speed);
    io_printf("Current Speed: %d MHz\n", processor->current_speed);
    io_printf("Voltage: 0x%x\n", processor->voltage);

    return PRA_OK;
}
