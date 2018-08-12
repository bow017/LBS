#define BYTE_SIZE 8

#include "server/grid/lbs_bitmap.h"

int lbs_bitmap_init(lbs_bitmap_t* lbs_bitmap, uint32_t bits_num)
{
	uint32_t size = bits_num % BYTE_SIZE ? (bits_num/BYTE_SIZE ) + 1 : (bits_num/BYTE_SIZE);

	lbs_bitmap->bits = new uint8_t[size];

	lbs_bitmap->bits_num = bits_num;

}

int lbs_bitmap_destroy(lbs_bitmap_t* lbs_bitmap)
{
	delete lbs_bitmap->bits;
}

int lbs_bitmap_setbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos)
{
	int index = pos/BYTE_SIZE;
	int shift = pos%BYTE_SIZE;

	if(pos > lbs_bitmap -> bits_num)
	{
		return -1;
	}

	bits[index]|= (0x01 << (7 - shift));

	return 0;
}

int lbs_bitmap_unsetbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos)
{
	int index = pos/BYTE_SIZE;
	int shift = pos%BYTE_SIZE;

	if(pos > lbs_bitmap -> bits_num)
	{
		return -1;
	}

	bits[index] &= ~(0x01 << (7-shift));

	return 0;
}

int lbs_bitmap_isset(lbs_bitmap_t* lbs_bitmap, uint32_t pos)
{
	int index = pos/BYTE_SIZE;
	int shift = pos%BYTE_SIZE;

	return bit[index]& (0x01 << (7-shift));
}
