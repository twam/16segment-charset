#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define COLOR_RESET "\e[0m"
#define COLOR_BOLDWHITE "\e[1m"
#define COLOR_BOLDRED "\e[1;31m"
#define COLOR_GRAY "\e[0;37m"

#define CHAR_SMALL

#define CHARSET_SIZE 128

#define CHARS_PER_LINE 16
#define CHARS_PER_LINE 16

uint16_t charset[CHARSET_SIZE] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0204, 0x0000, 0xAABB, 0xFF99, 0x2771, 0x0200, 0x1400, 0x4100, 0xFF00, 0xAA00, 0x1000, 0x8800, 0x0020, 0x4400, //  !"#$%&'()*+,-./
	0x00FF, 0x000C, 0x8877, 0x883F, 0x888C, 0x88BB, 0x88FB, 0x000F, 0x88FF, 0x88BF, 0x8020, 0xC000, 0x1400, 0x8830, 0x4100, 0x2807, // 0123456789:;<=>?
	0x0CF7, 0x88CF, 0x2A3F, 0x00F3, 0x223F, 0x80F3, 0x80C3, 0x08FB, 0x88CC, 0x2233, 0x007C, 0x94C0, 0x00F0, 0x05CC, 0x11CC, 0x00FF, // @ABCDEFGHIJKLMNO
	0x88C7, 0x10FF, 0x98C7, 0x88BB, 0x2203, 0x00FC, 0x44C0, 0x50CC, 0x5500, 0x2500, 0x4433, 0x2212, 0x1100, 0x2221, 0x0000, 0x0030, // PQRSTUVWXYZ[\]^_
	0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, // `abcdefghijklmno
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xA212, 0x2200, 0x2A21, 0x0000, 0x0000, // pqrstuvwxyz{|}~!
};

#ifdef CHAR_SMALL
const char* digit[] = {
	" a b ",
	"hkmnc",
	" u p ",
	"gtsrd",
	" f e " };
#else
const char* digit[] = {
	" aaa bbb ",
	"hk  m  nc",
	"h k m n c",
	"h  kmn  c",
	" uuu ppp ",
	"g  tsr  d",
	"g t s r d",
	"gt  s  rd",
	" fff eee " };
#endif

const char* bitmapping = "abcdefghkmnprstu";
const char* symbols = "--||--||\\|/-\\|/-";

char bit2segment(int bit) {
	return bitmapping[bit];
}

int segment2bit(char segment) {
	char *pos = strchr(bitmapping, segment);
	if (pos == NULL) {
		return -1;
	} else {
		return pos-bitmapping;
	}
}

void display_character_line(int row, uint16_t segments)
{
	for (int col = 0; col < strlen(digit[row]); ++col) {
		int segment = segment2bit(digit[row][col]);

		if (segment == -1) {
			printf(" ");
		} else {
			if (segments & (1 << segment)) {
				printf(COLOR_BOLDRED);
			} else {
				printf(COLOR_GRAY);				
			}
			printf("%c", symbols[segment]);
			printf(COLOR_RESET);
		}
	}
}

void display_character(uint16_t segments)
{
	for (int row = 0; row < sizeof(digit)/sizeof(char*); ++row) {
		display_character_line(row, segments);
		printf("\n");		
	}
}

void display_charset(uint16_t* charset)
{
	printf("   ");
	for (int char_in_line = 0; char_in_line < CHARS_PER_LINE; ++char_in_line) {
		#ifdef CHAR_SMALL
		printf(COLOR_BOLDWHITE "   %02u   " COLOR_RESET, char_in_line);
		#else
		printf(COLOR_BOLDWHITE "     %02u     " COLOR_RESET, char_in_line);
	#endif
	}
	printf("\n\n");

	for (int character_line = 0; character_line < CHARSET_SIZE/CHARS_PER_LINE; ++character_line) {
		for (int line = 0; line < sizeof(digit)/sizeof(char*); ++line) {
			if (line == sizeof(digit)/sizeof(char*)/2) {
				printf(COLOR_BOLDWHITE "%02u   " COLOR_RESET, character_line);
			} else {
				printf("     ");
			}

			for (int char_in_line = 0; char_in_line < CHARS_PER_LINE; ++char_in_line) {
				int character = character_line * CHARS_PER_LINE + char_in_line;
				display_character_line(line, charset[character]);
				printf("   ");
			}
			printf("\n");
		}
		printf("\n");
	}
}

int main(int argc, char** argv) {
	display_charset(charset);

	return EXIT_SUCCESS;
}