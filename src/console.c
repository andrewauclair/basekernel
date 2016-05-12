/*
Copyright (C) 2015 The University of Notre Dame
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

#include "console.h"
#include "graphics.h"
#include "font.h"

static int xsize=80;
static int ysize=25;

static int xpos=0;
static int ypos=0;

static char command_line[81];

struct graphics_color bgcolor = {0,0,0};
struct graphics_color fgcolor = {255,255,255};

void console_reset()
{
	xpos = ypos = 0;
	graphics_clear(bgcolor);
}

static void console_writechar( int x, int y, char ch )
{
	graphics_char(x*FONT_WIDTH,y*FONT_HEIGHT,ch,fgcolor,bgcolor);
}

void console_heartbeat()
{
	static int onoff=0;
	if(onoff) {
		graphics_char(xpos*FONT_WIDTH,ypos*FONT_HEIGHT,'_',fgcolor,bgcolor);
	} else {
		graphics_char(xpos*FONT_WIDTH,ypos*FONT_HEIGHT,'_',bgcolor,bgcolor);
	}
	onoff = !onoff;
}

void console_putchar( char c )
{
	console_writechar(xpos,ypos,' ');

	switch(c) {
		case 13:
		case 10:
			xpos=0;
			ypos++;
			for (int i = 0; i < xsize; ++i)
			{
				command_line[i] = ' ';
			}
			break;
		case '\f':
			console_reset();
			break;
		case '\b':
			xpos--;
			break;
		default:
			command_line[xpos] = c;
			console_writechar(xpos,ypos,c);
			xpos++;
			break;
	}

	if(xpos<0) {
		xpos=xsize-1;
		ypos--;
	}

	if(xpos>=xsize) {
		xpos=0;
		ypos++;
	}

	if(ypos>=ysize) {
		int scrolly = ysize - (ypos - 1);
		//console_reset();
		console_scroll(scrolly);
		xpos = 0;
		//ypos -= (ypos - ysize - 1);
	}

	console_writechar(xpos,ypos,'_');
	graphics_swap_buffers();

}

void console_putstring( const char *s )
{
	while(*s) {
		console_putchar(*s);
		s++;
	}

	graphics_swap_buffers();
}

int console_write( int unit, const void *buffer, int length, int offset )
{
	char *cbuffer = (char*)buffer;
	while(length>0) {
		console_putchar(*cbuffer);
		cbuffer++;
		length--;
	}
	graphics_swap_buffers();

	return 1;
}

void console_init()
{
	xsize = graphics_width()/FONT_WIDTH;
	ysize = graphics_height()/FONT_HEIGHT;
	console_reset();
	console_putstring("\nconsole: initialized\n");
}

void console_scroll(int y)
{
	ypos -= y;

	//console_printf("scroll %d characters", y);
	graphics_scroll(y * FONT_HEIGHT);
}
