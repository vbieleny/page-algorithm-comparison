void kernel_main() __attribute__ ((section (".text.kernel")));

void kernel_main()
{
    char *video_memory = 0xb8000;
    video_memory[0] = 'V';
    for (;;)
        asm volatile("hlt");
}
