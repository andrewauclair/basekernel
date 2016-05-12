#include "kerneltypes.h"
#include "ata.h"
#include "console.h"

void iso_read_disc(int drive)
{
	uint16_t data[1024];
	char* cbuffer = (char*)data;

	memset(cbuffer, 0, 1024);

	atapi_read(drive, cbuffer, 1, 16);

	int i = 0;
	uint8_t t = 0;
	for (i = 0; i < 1024; i += 2)
	{
		t = cbuffer[i];
		//cbuffer[i] = cbuffer[i + 1];
		//cbuffer[i + 1] = t;
	}

	int type = cbuffer[0];
	char identifier[6];
	identifier[5] = '\0';
	int version = cbuffer[6];

	console_printf("Volume Descriptor: %d (", type);
	switch (type)
	{
	case 0:
		console_printf("Boot Record)\n");
		break;
	case 1:
		console_printf("Primary Volume Descriptor)\n");
		break;
	case 2:
		console_printf("Supplementary Volume Descriptor)\n");
		break;
	case 3:
		console_printf("Volume Partition Descriptor)\n");
		break;
	case 255:
		console_printf("Volume Descriptor Set Terminator)\n");
		break;
	default:
		console_printf("Reserved)\n");
		break;
	}
	for (i = 0; i < 5; ++i)
	{
		identifier[i] = cbuffer[i + 1];

	}
	console_printf("Identifier: %s\n", identifier);

	console_printf("Version: %d\n", version);

	switch (type)
	{
	case 0:
		iso_read_bootrecord(cbuffer);
		break;
	case 1:
		iso_read_primaryvolume(cbuffer);
		break;
	case 2:
		iso_read_suppvolume(cbuffer);
		break;
	case 3:
		iso_read_volumepart(cbuffer);
		break;
	}







	int j = 0;
	int k = 0;
	for (k = 0; k < 5; ++k)
	{
		for (j = 0; j < 7; ++j)
		{
			console_printf("%x ", cbuffer[(k * 7) + j]);
		}
		console_printf("\n");
	}
}

void iso_read_bootrecord(char* buffer)
{
	// read boot system identier, offset 7, length 32
	// read boot identier, offset 39, length 32
}

void iso_read_primaryvolume(char* buffer)
{
	//uint32_t year = buffer[813];
	
	// system id
	// volume id
	// volume space size
	uint32_t volumeSpaceSize = buffer[80];
	console_printf("Volume Space Size: %d\n", volumeSpaceSize);

	uint16_t volumeSetSize = buffer[120];
	console_printf("Volume Set Size: %d\n", volumeSetSize);

	uint16_t volumeSeqNum = buffer[124];
	console_printf("Volume Sequence Number: %d\n", volumeSeqNum);

	uint16_t logicalBlockSize = buffer[128];
	console_printf("Logical Block Size: %d\n", logicalBlockSize);

	uint32_t pathTableSize = buffer[132];
	console_printf("Path Table Size: %d\n", pathTableSize);

	console_printf("creation date: ");
	iso_read_datetime_format(buffer, 813);
	console_printf("modified date: ");
	iso_read_datetime_format(buffer, 830);
	console_printf("expiration date: ");
	iso_read_datetime_format(buffer, 847);
	console_printf("effective date: ");
	iso_read_datetime_format(buffer, 864);
}

void iso_read_suppvolume(char* buffer)
{
}

void iso_read_volumepart(char* buffer)
{
}

void iso_read_datetime_format(char* buffer, int offset)
{
	char year[5]; year[4] = '\0';
	char month[3]; month[2] = '\0';
	char day[3]; day[2] = '\0';
	char hour[3]; hour[2] = '\0';
	char minute[3]; minute[2] = '\0';
	char second[3]; second[2] = '\0';
	char hundsec[3]; hundsec[2] = '\0';

	//console_printf("create year: %d\n", year);

	int i = 0;
	int index = offset;

	for (i = 0; i < 4; ++i)
	{
		year[i] = buffer[index];
		index++;
		//console_printf("%x ", buffer[813 + i]);
	}

	for (i = 0; i < 2; ++i)
	{
		month[i] = buffer[index];
		index++;
	}
	for (i = 0; i < 2; ++i)
	{
		day[i] = buffer[index];
		index++;
	}
	for (i = 0; i < 2; ++i)
	{
		hour[i] = buffer[index];
		index++;
	}
	for (i = 0; i < 2; ++i)
	{
		minute[i] = buffer[index];
		index++;
	}
	for (i = 0; i < 2; ++i)
	{
		second[i] = buffer[index];
		index++;
	}
	for (i = 0; i < 2; ++i)
	{
		hundsec[i] = buffer[index];
		index++;
	}

	uint8_t gmtOffset = buffer[index];

	console_printf("%s-%s-%s %s:%s:%s.%s\n", year, month, day, hour, minute, second, hundsec);
}
