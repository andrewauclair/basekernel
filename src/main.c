/*
Copyright (C) 2015 The University of Notre Dame
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

#include "console.h"
#include "memory.h"
#include "process.h"
#include "interrupt.h"
#include "keyboard.h"
#include "clock.h"
#include "ata.h"
#include "string.h"
#include "graphics.h"
#include "ascii.h"
#include "syscall.h"
#include "rtc.h"
#include "kernelcore.h"
#include "memorylayout.h"

/*
This is the C initialization point of the kernel.
By the time we reach this point, we are in protected mode,
with interrupts disabled, a valid C stack, but no malloc heap.
Now we initialize each subsystem in the proper order:
*/

int kernel_main()
{
	graphics_alloc_backbuffer();
	console_init();

	console_printf("video: %d x %d\n",video_xres,video_yres,video_xbytes);
	console_printf("kernel: %d bytes\n",kernel_size);

	memory_init();
	interrupt_init();
	rtc_init();
	clock_init();
	keyboard_init();
	graphics_init();

/*
process_init() is a big step.  This initializes the process table, but also gives us our own process structure, private stack, and enables paging.  Now we can do complex things like wait upon events.
*/
	process_init();

	ata_init();

	console_printf("\nBASEKERNEL READY:\n");

	struct graphics_color color = { 255, 255, 0 };
	graphics_line(0, 0, 480, 30, color);
	graphics_line(480, 30, 500, 360, color);
	graphics_line(400, 400, 50, 50, color);

	// will create page fault
	/*uint32_t* memtest = 0x80000000;
	(*memtest) = 4;
	
	console_reset();
	console_printf("memtest %d\n", *memtest);
	uint32_t* memtest2 = 0x80001000;
	(*memtest2) = 8;
	console_printf("memtest2 %d\n", *memtest2);*/

	//process_dump(current);
	while(1) console_putchar(keyboard_read());

	return 0;
}
