#include "pra_io.h"
#include "pra_pic.h"

void pic_remap(uint8_t offset1, uint8_t offset2)
{
    uint8_t a1, a2;

    a1 = io_in_byte(PIC1_DATA);
    a2 = io_in_byte(PIC2_DATA);

    io_out_byte(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    io_out_byte(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    io_out_byte(PIC1_DATA, offset1);
    io_wait();
    io_out_byte(PIC2_DATA, offset2);
    io_wait();
    io_out_byte(PIC1_DATA, 4);
    io_wait();
    io_out_byte(PIC2_DATA, 2);
    io_wait();

    io_out_byte(PIC1_DATA, ICW4_8086);
    io_wait();
    io_out_byte(PIC2_DATA, ICW4_8086);
    io_wait();

    io_out_byte(PIC1_DATA, a1);
    io_out_byte(PIC2_DATA, a2);
}

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
        io_out_byte(PIC2_COMMAND, PIC_EOI);
    io_out_byte(PIC1_COMMAND, PIC_EOI);
}

void irq_set_mask_all()
{
    for (size_t i = 0; i < 16; i++)
        irq_set_mask(i);
}

void irq_set_mask(uint8_t irq_line)
{
    uint16_t port;
    uint8_t value;

    if (irq_line < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = io_in_byte(port) | (1 << irq_line);
    io_out_byte(port, value);
}

void irq_clear_mask(uint8_t irq_line)
{
    uint16_t port;
    uint8_t value;

    if (irq_line < 8)
    {
        port = PIC1_DATA;
    }
    else
    {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = io_in_byte(port) & ~(1 << irq_line);
    io_out_byte(port, value);
}
