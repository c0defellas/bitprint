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
 * test - Bits printer macro test
 */

#include "bitprint.h"

int main(void)
{
	char c = 3;
	short s = 2;
	int i = 5;
	long l = 3;
	unsigned long long ull = 1550000;

	int ret;

	if (pb_is_little_endian())
		printf("Little Endian\n");
	else
		printf("Big Endian\n");

	ret = pb_populate_bits_map();
	if (!ret)
		printf("Bit string map populated\n\n");
	else if (ret == PB_ENOMEM) {
		printf("Out of memory\n");
		return ret;
	}

	printf("type char has %d bits\n\n", PRINT_BITS(c));
	printf("type short has %d bits\n\n", PRINT_BITS(s));
	printf("type int has %d bits\n\n", PRINT_BITS(i));
	printf("type long has %d bits\n\n", PRINT_BITS(l));
	printf("type unsigned long long has %d bits\n\n", PRINT_BITS(ull));
	printf("type unsigned char has %d bits\n\n", PRINT_BITS((unsigned char) 7));
	printf("type unsigned short has %d bits\n\n", PRINT_BITS((unsigned short) 9));
	printf("type int has %d bits\n\n", PRINT_BITS(-55));
	printf("type long has %d bits\n\n", PRINT_BITS(-10000L));
	printf("type long long has %d bits\n\n", PRINT_BITS(-50000LL));

	printf("byte %d is %s in bits\n\n", 1, pb_bits_map[1]);
	printf("byte %d is %s in bits\n\n", 100, pb_bits_map[100]);
	printf("byte %d is %s in bits\n\n", 255, pb_bits_map[255]);

	pb_unpopulate_bits_map();

	return 0;
}
