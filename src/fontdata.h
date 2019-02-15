#ifndef FONTDATA_H_
#define FONTDATA_H_

#define FONT_START	32
#define FONT_COUNT	58

static unsigned char fontdata[] = {

	/* 32 0x20 ' ' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 33 0x21 '!' */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */

	/* 34 0x22 '"' */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x24, /* 00100100 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 35 0x23 '#' */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0xfe, /* 11111110 */
	0x6c, /* 01101100 */
	0xfe, /* 11111110 */
	0x6c, /* 01101100 */
	0x6c, /* 01101100 */
	0x00, /* 00000000 */

	/* 36 0x24 '$' */
	0x18, /* 00011000 */
	0x3e, /* 00111110 */
	0x60, /* 01100000 */
	0x3c, /* 00111100 */
	0x06, /* 00000110 */
	0x7c, /* 01111100 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */

	/* 37 0x25 '%' */
	0x00, /* 00000000 */
	0xc6, /* 11000110 */
	0xcc, /* 11001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x66, /* 01100110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */

	/* 38 0x26 '&' */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x76, /* 01110110 */
	0xdc, /* 11011100 */
	0xcc, /* 11001100 */
	0x76, /* 01110110 */
	0x00, /* 00000000 */

	/* 39 0x27 ''' */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 40 0x28 '(' */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x00, /* 00000000 */

	/* 41 0x29 ')' */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */

	/* 42 0x2a '*' */
	0x00, /* 00000000 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0xff, /* 11111111 */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 43 0x2b '+' */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 44 0x2c ',' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */

	/* 45 0x2d '-' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 46 0x2e '.' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */

	/* 47 0x2f '/' */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0xc0, /* 11000000 */
	0x80, /* 10000000 */
	0x00, /* 00000000 */

	/* 48 0x30 '0' */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */

	/* 49 0x31 '1' */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x38, /* 00111000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */

	/* 50 0x32 '2' */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0x1c, /* 00011100 */
	0x30, /* 00110000 */
	0x66, /* 01100110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */

	/* 51 0x33 '3' */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0x3c, /* 00111100 */
	0x06, /* 00000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */

	/* 52 0x34 '4' */
	0x00, /* 00000000 */
	0x1c, /* 00011100 */
	0x3c, /* 00111100 */
	0x6c, /* 01101100 */
	0xfe, /* 11111110 */
	0x0c, /* 00001100 */
	0x1e, /* 00011110 */
	0x00, /* 00000000 */

	/* 53 0x35 '5' */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xc0, /* 11000000 */
	0xfc, /* 11111100 */
	0x06, /* 00000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */

	/* 54 0x36 '6' */
	0x00, /* 00000000 */
	0x38, /* 00111000 */
	0x60, /* 01100000 */
	0xfc, /* 11111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */

	/* 55 0x37 '7' */
	0x00, /* 00000000 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x30, /* 00110000 */
	0x00, /* 00000000 */

	/* 56 0x38 '8' */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */

	/* 57 0x39 '9' */
	0x00, /* 00000000 */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0x7e, /* 01111110 */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x78, /* 01111000 */
	0x00, /* 00000000 */

	/* 58 0x3a ':' */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */

	/* 59 0x3b ';' */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */

	/* 60 0x3c '<' */
	0x06, /* 00000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x06, /* 00000110 */
	0x00, /* 00000000 */

	/* 61 0x3d '=' */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */
	0x7e, /* 01111110 */
	0x00, /* 00000000 */
	0x00, /* 00000000 */

	/* 62 0x3e '>' */
	0x60, /* 01100000 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x30, /* 00110000 */
	0x60, /* 01100000 */
	0x00, /* 00000000 */

	/* 63 0x3f '?' */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0x0c, /* 00001100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */
	0x18, /* 00011000 */
	0x00, /* 00000000 */

	/* 64 0x40 '@' */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xde, /* 11011110 */
	0xde, /* 11011110 */
	0xde, /* 11011110 */
	0xc0, /* 11000000 */
	0x78, /* 01111000 */
	0x00, /* 00000000 */

	/* 65 0x41 'A' */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */

	/* 66 0x42 'B' */
	0xfc, /* 11111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x7c, /* 01111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0xfc, /* 11111100 */
	0x00, /* 00000000 */

	/* 67 0x43 'C' */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */

	/* 68 0x44 'D' */
	0xf8, /* 11111000 */
	0x6c, /* 01101100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x6c, /* 01101100 */
	0xf8, /* 11111000 */
	0x00, /* 00000000 */

	/* 69 0x45 'E' */
	0xfe, /* 11111110 */
	0x62, /* 01100010 */
	0x68, /* 01101000 */
	0x78, /* 01111000 */
	0x68, /* 01101000 */
	0x62, /* 01100010 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */

	/* 70 0x46 'F' */
	0xfe, /* 11111110 */
	0x62, /* 01100010 */
	0x68, /* 01101000 */
	0x78, /* 01111000 */
	0x68, /* 01101000 */
	0x60, /* 01100000 */
	0xf0, /* 11110000 */
	0x00, /* 00000000 */

	/* 71 0x47 'G' */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0xc0, /* 11000000 */
	0xc0, /* 11000000 */
	0xce, /* 11001110 */
	0x66, /* 01100110 */
	0x3a, /* 00111010 */
	0x00, /* 00000000 */

	/* 72 0x48 'H' */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */

	/* 73 0x49 'I' */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */

	/* 74 0x4a 'J' */
	0x1e, /* 00011110 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0x0c, /* 00001100 */
	0xcc, /* 11001100 */
	0xcc, /* 11001100 */
	0x78, /* 01111000 */
	0x00, /* 00000000 */

	/* 75 0x4b 'K' */
	0xe6, /* 11100110 */
	0x66, /* 01100110 */
	0x6c, /* 01101100 */
	0x78, /* 01111000 */
	0x6c, /* 01101100 */
	0x66, /* 01100110 */
	0xe6, /* 11100110 */
	0x00, /* 00000000 */

	/* 76 0x4c 'L' */
	0xf0, /* 11110000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0x62, /* 01100010 */
	0x66, /* 01100110 */
	0xfe, /* 11111110 */
	0x00, /* 00000000 */

	/* 77 0x4d 'M' */
	0xc6, /* 11000110 */
	0xee, /* 11101110 */
	0xfe, /* 11111110 */
	0xfe, /* 11111110 */
	0xd6, /* 11010110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */

	/* 78 0x4e 'N' */
	0xc6, /* 11000110 */
	0xe6, /* 11100110 */
	0xf6, /* 11110110 */
	0xde, /* 11011110 */
	0xce, /* 11001110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */

	/* 79 0x4f 'O' */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */

	/* 80 0x50 'P' */
	0xfc, /* 11111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x7c, /* 01111100 */
	0x60, /* 01100000 */
	0x60, /* 01100000 */
	0xf0, /* 11110000 */
	0x00, /* 00000000 */

	/* 81 0x51 'Q' */
	0x7c, /* 01111100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xce, /* 11001110 */
	0x7c, /* 01111100 */
	0x0e, /* 00001110 */

	/* 82 0x52 'R' */
	0xfc, /* 11111100 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x7c, /* 01111100 */
	0x6c, /* 01101100 */
	0x66, /* 01100110 */
	0xe6, /* 11100110 */
	0x00, /* 00000000 */

	/* 83 0x53 'S' */
	0x3c, /* 00111100 */
	0x66, /* 01100110 */
	0x30, /* 00110000 */
	0x18, /* 00011000 */
	0x0c, /* 00001100 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */

	/* 84 0x54 'T' */
	0x7e, /* 01111110 */
	0x7e, /* 01111110 */
	0x5a, /* 01011010 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */

	/* 85 0x55 'U' */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x7c, /* 01111100 */
	0x00, /* 00000000 */

	/* 86 0x56 'V' */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x00, /* 00000000 */

	/* 87 0x57 'W' */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0xd6, /* 11010110 */
	0xd6, /* 11010110 */
	0xfe, /* 11111110 */
	0x6c, /* 01101100 */
	0x00, /* 00000000 */

	/* 88 0x58 'X' */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x6c, /* 01101100 */
	0x38, /* 00111000 */
	0x6c, /* 01101100 */
	0xc6, /* 11000110 */
	0xc6, /* 11000110 */
	0x00, /* 00000000 */

	/* 89 0x59 'Y' */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x66, /* 01100110 */
	0x3c, /* 00111100 */
	0x18, /* 00011000 */
	0x18, /* 00011000 */
	0x3c, /* 00111100 */
	0x00, /* 00000000 */

	/* 90 0x5a 'Z' */
	0xfe, /* 11111110 */
	0xc6, /* 11000110 */
	0x8c, /* 10001100 */
	0x18, /* 00011000 */
	0x32, /* 00110010 */
	0x66, /* 01100110 */
	0xfe, /* 11111110 */
	0x00 /* 00000000 */
};

#endif	/* FONTDATA_H_ */
