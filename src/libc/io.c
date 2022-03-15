#include <io.h>

void io_out_byte(uint16_t port, uint8_t value);
uint8_t io_in_byte(uint16_t port);
void io_wait();
