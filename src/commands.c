#include "keyboard.h"
#include "string.h"
#include "iso.h"
#include "console.h"
#include "memory.h"
#include "graphics.h"
#include "icon.h"

void commands_loop()
{
	char input[256];
	int index = 0;

	while (1)
	{
		char key = keyboard_read();
		console_putchar(key);

		if (key == 13 || key == 10)
		{
			// execute command
			input[index] = '\0';
			commands_run(input);

			memset(input, 0, 256);
			index = 0;
		}
		else
		{
			input[index] = key;
			index++;
		}
	}
}

void commands_run(char* cmd)
{
	if (strcmp(cmd, "disk") == 0)
	{
		iso_read_disc(2);
	}
	else if (strcmp(cmd, "memory") == 0)
	{
		console_printf("pages free: %d\npages total: %d\n", memory_pages_free(), memory_pages_total());
	}
	else if (strcmp(cmd, "clear") == 0)
	{
		console_reset();
	}
}
