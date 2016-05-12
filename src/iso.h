#pragma once

#include "kerneltypes.h"

void iso_read_disc(int drive);

void iso_read_bootrecord(char* buffer);
void iso_read_primaryvolume(char* buffer);
void iso_read_suppvolume(char* buffer);
void iso_read_volumepart(char* buffer);

void iso_read_datetime_format(char* buffer, int offset);