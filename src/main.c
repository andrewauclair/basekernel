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
#include "iso.h"
#include "commands.h"

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

	
	//ata_read(0, data, 0, 0);
	//ata_read(1, data, 0, 0);
	int i = 0;
	/*for (i = 0x10; i < 0x15; ++i)
	{
		uint8_t data;
		atapi_read(2, data, 1, i);
		console_printf("0x%x\n", data);
	}*/
	uint16_t data[1024];
	char *cbuffer = (char*)data;
	
	//for (i = 15; i < 18; ++i)
	{
		memset(cbuffer, 0, 1024);
		//atapi_drive_read_sector(1, 1, 0x8000, cbuffer);
		//atapi_read(2, cbuffer, 1, 16);// 1, 0x8000);
		//console_printf("s%d %x %d %d %d %d %d\n", i, cbuffer[0], data[1], data[2], data[3], data[4], data[5]);
		//console_printf("s%d %x ", i, cbuffer[0]);
		int j = 0;
		int k = 0;
		for (k = 0; k < 5; ++k)
		{
			for (j = 0; j < 7; ++j)
			{
				//console_printf("%x ", data[(k * 7) + j]);
			}
			//console_printf("\n");
		}
		//console_printf(" %x", cbuffer[6]);
		//console_printf("\n");
		//console_printf("s%d %d %d %d %d %d %d\n", i, data[1023], data[1022], data[1021], data[1020], data[1019], data[1018]);
	}
	/*struct graphics_color color = { 255, 255, 0 };
	graphics_line(0, 0, 480, 30, color);
	graphics_line(480, 30, 500, 360, color);
	graphics_line(400, 400, 50, 50, color);
*/
	// will create page fault
	/*uint32_t* memtest = 0x80000000;
	(*memtest) = 4;
	
	console_reset();
	console_printf("memtest %d\n", *memtest);
	uint32_t* memtest2 = 0x80001000;
	(*memtest2) = 8;
	console_printf("memtest2 %d\n", *memtest2);*/
	//iso_read_disc(2);

	//process_dump(current);
	//while(1) console_putchar(keyboard_read());
	commands_loop();

	return 0;
}
