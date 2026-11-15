image_size = 500 * 1024 * 1024
word_size = 4
dma_block_size = 4096
interrupt_time = 2

words = image_size // word_size
dma_blocks = image_size // dma_block_size

word_interrupts = words
dma_interrupts = dma_blocks
single_dma_interrupts = 1

word_cpu_time = word_interrupts * interrupt_time / 1000000
dma_cpu_time = dma_interrupts * interrupt_time / 1000000

reduction = word_interrupts // dma_interrupts

print("Image size:", image_size, "bytes")
print("Interrupt-driven I/O:")
print("Number of interrupts:", word_interrupts)
print("CPU time:", round(word_cpu_time, 3), "seconds")

print("\nDMA (4 KB blocks):")
print("Number of interrupts:", dma_interrupts)
print("CPU time:", round(dma_cpu_time, 3), "seconds")

print("\nDMA (one transfer):")
print("Number of interrupts:", single_dma_interrupts)

print("\nReduction:", reduction, "times fewer interrupts")
