/*
Copyright (C) 2015 The University of Notre Dame
This software is distributed under the GNU General Public License.
See the file LICENSE for details.
*/

#pragma once

#include "kerneltypes.h"

struct graphics_color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

extern uint8_t* back_buffer;

int graphics_width();
int graphics_height();

void graphics_rect( int x, int y, int w, int h, struct graphics_color c );
void graphics_clear( struct graphics_color c );
void graphics_char( int x, int y, char ch, struct graphics_color fgcolor, struct graphics_color bgcolor );
void graphics_scroll(int y);
void graphics_line(int x1, int y1, int x2, int y2, struct graphics_color color);
void graphics_circle(int x, int y, int r, struct graphics_color color);
void graphics_bitmap(int x, int y, int width, int height, uint8_t *data, struct graphics_color fgcolor, struct graphics_color bgcolor);

void graphics_interrupt_handler(int intr, int code);
void graphics_init();
void graphics_alloc_backbuffer();
void graphics_swap_buffers();
