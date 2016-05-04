OBJECTS=kernelcore.o main.o console.o memory.o keyboard.o clock.o interrupt.o pic.o ata.o string.o graphics.o font.o syscall.o syscall_handler.o process.o mutex.o list.o pagetable.o rtc.o

KERNEL_CCFLAGS=-Wall -c -ffreestanding -m32 -march=i386
KERNEL_LDFLAGS=-m elf_i386

all: basekernel.img
	mkdir -p tmp/deploy
	cp basekernel.img tmp/deploy
	mkisofs -U -b basekernel.img -hide basekernel.img -V "BaseKernel" -iso-level 3 -o tmp/basekernel.iso tmp/deploy
	
basekernel.img: bootblock kernel
	cat bootblock kernel > basekernel.img
	truncate basekernel.img --size 1474560

bootblock: bootblock.o
	ld ${KERNEL_LDFLAGS} -Ttext 0 -s --oformat binary $< -o $@

kernel: ${OBJECTS}
	ld ${KERNEL_LDFLAGS} -Ttext 0x10000 -s --oformat binary ${OBJECTS} -o $@

%.o: %.c
	gcc ${KERNEL_CCFLAGS} $< -o $@

%.o: %.S
	gcc ${KERNEL_CCFLAGS} $< -o $@

clean:
	rm -rf basekernel.img bootblock kernel *.o
