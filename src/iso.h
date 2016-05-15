#pragma once

#include "kerneltypes.h"

void iso_read_disc(int drive);

void iso_read_bootrecord(char* buffer);
void iso_read_primaryvolume(int drive, char* buffer);
void iso_read_suppvolume(char* buffer);
void iso_read_volumepart(char* buffer);

void iso_read_datetime_format(char* buffer, int offset);

void iso_read_lpath(int drive, int lba);
void iso_read_directory(int drive, int lba);

uint32_t iso_read_uint32(char* buffer);
uint16_t iso_read_uint16(char* buffer);