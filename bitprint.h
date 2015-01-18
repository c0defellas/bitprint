/*
 * Copyright 2015 c0defellas
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this name except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * bitprint - Bits printer macro
 */

#ifndef BITPRINT_H
#define BITPRINT_H

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#define PB_BITSALLOCATED 1
#define PB_ENOMEM ENOMEM

#define PRINT_BITS(value)					\
	_Generic(value,						\
		 char               : _pb_print_bits_c,		\
		 unsigned char      : _pb_print_bits_uc,	\
		 short              : _pb_print_bits_s,		\
		 unsigned short     : _pb_print_bits_us,	\
		 int                : _pb_print_bits_i,		\
		 unsigned int       : _pb_print_bits_ui,	\
		 long               : _pb_print_bits_l,		\
		 unsigned long      : _pb_print_bits_ul,	\
		 long long          : _pb_print_bits_ll,	\
		 unsigned long long : _pb_print_bits_ull)	\
	(value)

bool pb_is_little_endian()
{
	int i = 1;
	return ((char *) &i)[0] ? true : false;
}

bool pb_is_big_endian()
{
	return !pb_is_little_endian();
}


static unsigned char *pb_bits_map[256] = {NULL};

void pb_unpopulate_bits_map()
{
	for (int i = 0; i <= UCHAR_MAX; i++) {
		if (!pb_bits_map[0])
			continue;
		free(pb_bits_map[i]);
		pb_bits_map[i] = NULL;
	}
}

int pb_populate_bits_map()
{
	if (pb_bits_map[0])
		return PB_BITSALLOCATED;

	int bits_length = sizeof(char) * 8;
	char bitwise = 1 << (bits_length - 1);
	unsigned char *bits;
	unsigned char byte;

	for (int i = 0; i <= UCHAR_MAX; i++) {
		byte = (unsigned char) i;
		bits = calloc(1, bits_length + 1);

		if (!bits)
			goto err;

		for (int k = 0; k < bits_length; k++) {
			bits[k] = byte & bitwise ? '1' : '0';
			byte <<= 1;
		}
		pb_bits_map[i] = bits;
	}

	return 0;
err:
	pb_unpopulate_bits_map();
	return PB_ENOMEM;
}

int _pb_print_bits(void *ptr, int len) {
        int i;
	int ret = 0;

        if (!ptr)
                return 0;

	if (pb_is_little_endian()) {
		for (i = len - 1; i >= 0; i--) {
			ret += printf("%s ", pb_bits_map[((unsigned char *) ptr)[i]]);
			ret--;
		}
	}
	else {
		for (i = 0; i < len; i++) {
			ret += printf("%s ", pb_bits_map[((unsigned char *) ptr)[i]]);
			ret--;
		}
	}

	printf("\n");

	return ret;
}

int _pb_print_bits_c(char value)
{
	return _pb_print_bits(&value, sizeof(char));;
}

int _pb_print_bits_uc(unsigned char value)
{
	return _pb_print_bits(&value, sizeof(unsigned char));;
}

int _pb_print_bits_s(short value)
{
	return _pb_print_bits(&value, sizeof(short));;
}

int _pb_print_bits_us(unsigned short value)
{
	return _pb_print_bits(&value, sizeof(unsigned short));;
}

int _pb_print_bits_i(int value)
{
	return _pb_print_bits(&value, sizeof(int));
}

int _pb_print_bits_ui(unsigned int value)
{
	return _pb_print_bits(&value, sizeof(unsigned int));
}

int _pb_print_bits_l(long value)
{
	return _pb_print_bits(&value, sizeof(long));
}

int _pb_print_bits_ul(unsigned long value)
{
	return _pb_print_bits(&value, sizeof(unsigned long));
}

int _pb_print_bits_ll(long long value)
{
	return _pb_print_bits(&value, sizeof(long long));
}

int _pb_print_bits_ull(unsigned long long value)
{
	return _pb_print_bits(&value, sizeof(unsigned long long));
}

#endif
