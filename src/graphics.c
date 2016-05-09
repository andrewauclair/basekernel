/*
Copyright (C) 2015 The University of Notre Dame
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

#include "graphics.h"
#include "kerneltypes.h"
#include "font.h"
#include "kernelcore.h"
#include "memory.h"
#include "interrupt.h"
#include "string.h"
#include "console.h"

uint8_t* back_buffer = 0;

int graphics_width()
{
	return video_xres;
}

int graphics_height()
{
	return video_yres;
}

static inline void plot_pixel( int x, int y, struct graphics_color c )
{
	uint8_t *v = video_buffer + video_xbytes*y+x*3;
	//uint8_t* v = back_buffer + video_xbytes * y + x * 3;
	v[2] = c.r;
	v[1] = c.g;
	v[0] = c.b;
}

void graphics_rect( int x, int y, int w, int h, struct graphics_color c )
{
	int i, j;

	for(j=0;j<h;j++) {
		for(i=0;i<w;i++) {
			plot_pixel(x+i,y+j,c);
		}
	}
}

void graphics_clear( struct graphics_color c )
{
	graphics_rect(0,0,video_xres,video_yres,c);
}

void graphics_bitmap( int x, int y, int width, int height, uint8_t *data, struct graphics_color fgcolor, struct graphics_color bgcolor )
{
	int i,j,b;
	int value;

	b=0;

	for(j=0;j<height;j++) {
		for(i=0;i<width;i++) {
			value = ((*data)<<b)&0x80;
			if(value) {
				plot_pixel(x+i,y+j,fgcolor);
			} else {
				plot_pixel(x+i,y+j,bgcolor);
			}
			b++;
			if(b==8) {
				data++;
				b=0;
			}
		}
	}
}

void graphics_char( int x, int y, char ch, struct graphics_color fgcolor, struct graphics_color bgcolor )
{
	int u = ((int)ch)*FONT_WIDTH*(FONT_HEIGHT/8);
	return graphics_bitmap(x,y,FONT_WIDTH,FONT_HEIGHT,&fontdata[u],fgcolor,bgcolor);
}

void graphics_scroll(int y)
{
	memmove(video_buffer, video_buffer + (video_xbytes * y), (video_xbytes * video_yres) - (video_xbytes * y));
	memset(video_buffer + (video_xbytes * video_yres) - (video_xbytes * y), 0, (video_xbytes * y));
}

void graphics_line(int x1, int y1, int x2, int y2, struct graphics_color color)
{
	if (x1 > x2)
	{
		graphics_line(x2, y1, x1, y2, color);
		return;
	}
	if (y1 > y2)
	{
		graphics_line(x1, y2, x2, y1, color);
		return;
	}

	int dx = x2 - x1;
	int dy = y2 - y1;
	int y = 0;
	int x = 0;
	for (x = x1; x <= x2; ++x)
	{
		y = y1 + dy * (x - x1) / dx;
		plot_pixel(x, y, color);
	}
	
	for (y = y1; y <= y2; ++y)
	{
		x = x1 + dx * (y - y1) / dy;
		plot_pixel(x, y, color);
	}
}

void graphics_circle(int x, int y, int r, struct graphics_color color)
{

}

void graphics_interrupt_handler(int intr, int code)
{
	// swap buffers
	//console_printf("swap buffers");

	graphics_swap_buffers();
}

void graphics_init()
{
	interrupt_register(40, graphics_interrupt_handler);
	interrupt_enable(40);
}

void graphics_alloc_backbuffer()
{
	struct graphics_color bg = { 0,0,0 };

	graphics_rect(0, 0, video_xres, video_yres, bg);

	graphics_swap_buffers();
}

void graphics_swap_buffers()
{
	//memcpy(video_buffer, back_buffer, video_xres * video_yres * 3);
}
