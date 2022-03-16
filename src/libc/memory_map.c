#include <memory_map.h>
#include <io.h>

static const uintptr_t MEMORY_MAP_ADDRESS = 0x7e00;
static const uintptr_t MEMORY_MAP_COUNT = 0x9000;

memory_map_entry_t *memory_map = (memory_map_entry_t*) MEMORY_MAP_ADDRESS;

void memory_map_print()
{
    io_printf(DEFAULT_STREAM, "Memory map:\n");
    uint16_t count = *((uint16_t*) MEMORY_MAP_COUNT);
    for (int i = 0; i < count; i++)
    {
        memory_map_entry_t entry = memory_map[i];
        char *type_str;
        switch (entry.type)
        {
            case 1:
                type_str = "Usable (normal) RAM";
                break;
            case 2:
                type_str = "Reserved - unusable";
                break;
            case 3:
                type_str = "ACPI reclaimable memory";
                break;
            case 4:
                type_str = "ACPI NVS memory";
                break;
            case 5:
                type_str = "Area containing bad memory";
                break;
            default:
                type_str = "Unrecognized memory";
                break;
        }
        io_printf(DEFAULT_STREAM, "0x%x - 0x%x (%d B): %s\n", entry.base_low, entry.base_low + entry.length_low, entry.length_low, type_str);
    }
    uint32_t total_memory = (memory_map[count - 1].base_low + memory_map[count - 1].length_low) - memory_map[0].base_low;
    io_printf(DEFAULT_STREAM, "Total memory: %d MB\n", total_memory / 1024 / 1024);
}
