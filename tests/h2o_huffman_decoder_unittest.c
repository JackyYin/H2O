#include "h2o_huffman_decoder_unittest.h"
#include "huffman.h"

static void test_H2O_huffman_decode_5(void)
{
	uint8_t src[1] = {};
	uint8_t dst[1] = {};

	src[0] = 0x07; // 00000 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '0');

	src[0] = 0x0F; // 00001 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '1');

	src[0] = 0x17; // 00010 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '2');

	src[0] = 0x1F; // 00011 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'a');

	src[0] = 0x27; // 00100 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'c');

	src[0] = 0x2F; // 00101 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'e');

	src[0] = 0x37; // 00110 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'i');

	src[0] = 0x3F; // 00111 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'o');

	src[0] = 0x47; // 01000 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 's');

	src[0] = 0x4F; // 01001 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 't');
}

static void test_H2O_huffman_decode_6(void)
{
	uint8_t src[1] = {};
	uint8_t dst[1] = {};

	src[0] = 0x53; // 010100 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == ' ');

	src[0] = 0x57; // 010101 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '%');

	src[0] = 0x5B; // 010110 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '-');

	src[0] = 0x5F; // 010111 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '.');

	src[0] = 0x63; // 011000 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '/');

	src[0] = 0x67; // 011001 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '3');

	src[0] = 0x6B; // 011010 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '4');

	src[0] = 0x6F; // 011011 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '5');

	src[0] = 0x73; // 011100 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '6');

	src[0] = 0x77; // 011101 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '7');

	src[0] = 0x7B; // 011110 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '8');

	src[0] = 0x7F; // 011111 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '9');

	src[0] = 0x83; // 100000 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '=');

	src[0] = 0x87; // 100001 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'A');

	src[0] = 0x8B; // 100010 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '_');

	src[0] = 0x8F; // 100011 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'b');

	src[0] = 0x93; // 100100 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'd');

	src[0] = 0x97; // 100101 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'f');

	src[0] = 0x9B; // 100110 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'g');

	src[0] = 0x9F; // 100111 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'h');

	src[0] = 0xA3; // 101000 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'l');

	src[0] = 0xA7; // 101001 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'm');

	src[0] = 0xAB; // 101010 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'n');

	src[0] = 0xAF; // 101011 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'p');

	src[0] = 0xB3; // 101100 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'r');

	src[0] = 0xB7; // 101101 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'u');
}

static void test_H2O_huffman_decode_7(void)
{
	uint8_t src[1] = {};
	uint8_t dst[1] = {};

	src[0] = 0xB9; // 1011100 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == ':');

	src[0] = 0xBB; // 1011101 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'B');

	src[0] = 0xBD; // 1011110 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'C');

	src[0] = 0xBF; // 1011111 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'D');

	src[0] = 0xC1; // 1100000 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'E');

	src[0] = 0xC3; // 1100001 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'F');

	src[0] = 0xC5; // 1100010 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'G');

	src[0] = 0xC7; // 1100011 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'H');

	src[0] = 0xC9; // 1100100 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'I');

	src[0] = 0xCB; // 1100101 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'J');

	src[0] = 0xCD; // 1100110 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'K');

	src[0] = 0xCF; // 1100111 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'L');

	src[0] = 0xD1; // 1101000 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'M');

	src[0] = 0xD3; // 1101001 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'N');

	src[0] = 0xD5; // 1101010 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'O');

	src[0] = 0xD7; // 1101011 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'P');

	src[0] = 0xD9; // 1101100 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'Q');

	src[0] = 0xDB; // 1101101 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'R');

	src[0] = 0xDD; // 1101110 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'S');

	src[0] = 0xDF; // 1101111 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'T');

	src[0] = 0xE1; // 1110000 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'U');

	src[0] = 0xE3; // 1110001 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'V');

	src[0] = 0xE5; // 1110010 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'W');

	src[0] = 0xE7; // 1110011 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'Y');

	src[0] = 0xE9; // 1110100 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'j');

	src[0] = 0xEB; // 1110101 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'k');

	src[0] = 0xED; // 1110110 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'q');

	src[0] = 0xEF; // 1110111 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'v');

	src[0] = 0xF1; // 1111000 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'w');

	src[0] = 0xF3; // 1111001 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'x');

	src[0] = 0xF5; // 1111010 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'y');

	src[0] = 0xF7; // 1111011 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'z');
}

static void test_H2O_huffman_decode_8(void)
{
	uint8_t src[1] = {};
	uint8_t dst[1] = {};

	src[0] = 0xF8; // 11111000
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '&');

	src[0] = 0xF9; // 11111001
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '*');

	src[0] = 0xFA; // 11111010
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == ',');

	src[0] = 0xFB; // 11111011
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == ';');

	src[0] = 0xFC; // 11111100
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'X');

	src[0] = 0xFD; // 11111101
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == 'Z');
}

static void test_H2O_huffman_decode_10(void)
{
	uint8_t src[2] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFE; // 11111110
	src[1] = 0x3F; // 00 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '!');

	src[0] = 0xFE; // 11111110
	src[1] = 0x7F; // 01 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '"');

	src[0] = 0xFE; // 11111110
	src[1] = 0xBF; // 10 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '(');

	src[0] = 0xFE; // 11111110
	src[1] = 0xFF; // 11 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == ')');

	src[0] = 0xFF; // 11111111
	src[1] = 0x3F; // 00 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '?');
}

static void test_H2O_huffman_decode_11(void)
{
	uint8_t src[2] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0x5F; // 010 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '\'');

	src[0] = 0xFF; // 11111111
	src[1] = 0x7F; // 011 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '+');

	src[0] = 0xFF; // 11111111
	src[1] = 0x9F; // 100 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '|');
}

static void test_H2O_huffman_decode_12(void)
{
	uint8_t src[2] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xAF; // 1010 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '#');

	src[0] = 0xFF; // 11111111
	src[1] = 0xBF; // 1011 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '>');
}

static void test_H2O_huffman_decode_13(void)
{
	uint8_t src[2] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xC7; // 11000 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 0);

	src[0] = 0xFF; // 11111111
	src[1] = 0xCF; // 11001 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '$');

	src[0] = 0xFF; // 11111111
	src[1] = 0xD7; // 11010 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '@');

	src[0] = 0xFF; // 11111111
	src[1] = 0xDF; // 11011 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '[');

	src[0] = 0xFF; // 11111111
	src[1] = 0xE7; // 11100 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == ']');

	src[0] = 0xFF; // 11111111
	src[1] = 0xEF; // 11101 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '~');
}

static void test_H2O_huffman_decode_14(void)
{
	uint8_t src[2] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xF3; // 111100 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '^');

	src[0] = 0xFF; // 11111111
	src[1] = 0xF7; // 111101 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '}');
}

static void test_H2O_huffman_decode_15(void)
{
	uint8_t src[2] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xF9; // 1111100 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '<');

	src[0] = 0xFF; // 11111111
	src[1] = 0xFB; // 1111101 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '`');

	src[0] = 0xFF; // 11111111
	src[1] = 0xFD; // 1111110 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '{');
}

static void test_H2O_huffman_decode_19(void)
{
	uint8_t src[3] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0x1F; // 000 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((char)(dst[0]) == '\\');

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0x3F; // 001 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 195);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0x5F; // 010 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 208);
}

static void test_H2O_huffman_decode_20(void)
{
	uint8_t src[3] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0x6F; // 0110 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 128);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0x7F; // 0111 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 130);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0x8F; // 1000 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 131);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0x9F; // 1001 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 162);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0xAF; // 1010 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 184);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0xBF; // 1011 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 194);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0xCF; // 1100 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 224);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0xDF; // 1101 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 226);
}

static void test_H2O_huffman_decode_21(void)
{
	uint8_t src[3] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0xE7; // 11100 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 153);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0xEF; // 11101 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 161);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0xF7; // 11110 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 167);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFE; // 11111110
	src[2] = 0xFF; // 11111 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 172);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x07; // 00000 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 176);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x0F; // 00001 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 177);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x17; // 00010 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 179);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x1F; // 00011 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 209);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x27; // 00100 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 216);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x2F; // 00101 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 217);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x37; // 00110 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 227);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x3F; // 00111 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 229);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x47; // 01000 111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 230);
}

static void test_H2O_huffman_decode_22(void)
{
	uint8_t src[3] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x4B; // 010010 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 129);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x4F; // 010011 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 132);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x53; // 010100 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 133);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x57; // 010101 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 134);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x5B; // 010110 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 136);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x5F; // 010111 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 146);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x63; // 011000 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 154);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x67; // 011001 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 156);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x6B; // 011010 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 160);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x6F; // 011011 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 163);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x73; // 011100 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 164);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x77; // 011101 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 169);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x7B; // 011110 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 170);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x7F; // 011111 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 173);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x83; // 100000 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 178);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x87; // 100001 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 181);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x8B; // 100010 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 185);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x8F; // 100011 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 186);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x93; // 100100 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 187);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x97; // 100101 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 189);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x9B; // 100110 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 190);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0x9F; // 100111 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 196);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xA3; // 101000 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 198);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xA7; // 101001 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 228);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xAB; // 101010 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 232);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xAF; // 101011 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 233);
}

static void test_H2O_huffman_decode_23(void)
{
	uint8_t src[3] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xB1; // 1011000 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 1);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xB3; // 1011001 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 135);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xB5; // 1011010 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 137);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xB7; // 1011011 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 138);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xB9; // 1011100 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 139);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xBB; // 1011101 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 140);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xBD; // 1011110 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 141);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xBF; // 1011111 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 143);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xC1; // 1100000 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 147);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xC3; // 1100001 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 149);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xC5; // 1100010 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 150);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xC7; // 1100011 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 151);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xC9; // 1100100 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 152);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xCB; // 1100101 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 155);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xCD; // 1100110 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 157);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xCF; // 1100111 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 158);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xD1; // 1101000 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 165);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xD3; // 1101001 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 166);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xD5; // 1101010 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 168);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xD7; // 1101011 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 174);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xD9; // 1101100 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 175);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xDB; // 1101101 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 180);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xDD; // 1101110 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 182);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xDF; // 1101111 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 183);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xE1; // 1110000 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 188);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xE3; // 1110001 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 191);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xE5; // 1110010 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 197);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xE7; // 1110011 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 231);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xE9; // 1110100 1
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 239);
}

static void test_H2O_huffman_decode_24(void)
{
	uint8_t src[3] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xEA; // 11101010
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 9);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xEB; // 11101011
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 142);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xEC; // 11101100
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 144);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xED; // 11101101
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 145);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xEE; // 11101110
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 148);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xEF; // 11101111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 159);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF0; // 11110000
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 171);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF1; // 11110001
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 206);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF2; // 11110010
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 215);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF3; // 11110011
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 225);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF4; // 11110100
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 236);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF5; // 11110101
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 237);
}

static void test_H2O_huffman_decode_25(void)
{
	uint8_t src[4] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF6; // 11110110
	src[3] = 0x7F; // 0 1111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 199);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF6; // 11110110
	src[3] = 0xFF; // 1 1111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 207);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF7; // 11110111
	src[3] = 0x7F; // 0 1111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 234);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF7; // 11110111
	src[3] = 0xFF; // 1 1111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 235);
}

static void test_H2O_huffman_decode_26(void)
{
	uint8_t src[4] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF8; // 11111000
	src[3] = 0x3F; // 00 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 192);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF8; // 11111000
	src[3] = 0x7F; // 01 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 193);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF8; // 11111000
	src[3] = 0xBF; // 10 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 200);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF8; // 11111000
	src[3] = 0xFF; // 11 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 201);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF9; // 11111001
	src[3] = 0x3F; // 00 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 202);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF9; // 11111001
	src[3] = 0x7F; // 01 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 205);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF9; // 11111001
	src[3] = 0xBF; // 10 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 210);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xF9; // 11111001
	src[3] = 0xFF; // 11 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 213);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFA; // 11111010
	src[3] = 0x3F; // 00 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 218);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFA; // 11111010
	src[3] = 0x7F; // 01 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 219);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFA; // 11111010
	src[3] = 0xBF; // 10 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 238);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFA; // 11111010
	src[3] = 0xFF; // 11 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 240);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFB; // 11111011
	src[3] = 0x3F; // 00 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 242);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFB; // 11111011
	src[3] = 0x7F; // 01 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 243);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFB; // 11111011
	src[3] = 0xBF; // 10 111111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 255);
}

static void test_H2O_huffman_decode_27(void)
{
	uint8_t src[4] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFB; // 11111011
	src[3] = 0xDF; // 110 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 203);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFB; // 11111011
	src[3] = 0xFF; // 111 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 204);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFC; // 11111100
	src[3] = 0x1F; // 000 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 211);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFC; // 11111100
	src[3] = 0x3F; // 001 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 212);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFC; // 11111100
	src[3] = 0x5F; // 010 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 214);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFC; // 11111100
	src[3] = 0x7F; // 011 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 221);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFC; // 11111100
	src[3] = 0x9F; // 100 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 222);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFC; // 11111100
	src[3] = 0xBF; // 101 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 223);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFC; // 11111100
	src[3] = 0xDF; // 110 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 241);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFC; // 11111100
	src[3] = 0xFF; // 111 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 244);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFD; // 11111101
	src[3] = 0x1F; // 000 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 245);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFD; // 11111101
	src[3] = 0x3F; // 001 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 246);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFD; // 11111101
	src[3] = 0x5F; // 010 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 247);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFD; // 11111101
	src[3] = 0x7F; // 011 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 248);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFD; // 11111101
	src[3] = 0x9F; // 100 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 250);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFD; // 11111101
	src[3] = 0xBF; // 101 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 251);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFD; // 11111101
	src[3] = 0xDF; // 110 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 252);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFD; // 11111101
	src[3] = 0xFF; // 111 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 253);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0x1F; // 000 11111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 254);
}

static void test_H2O_huffman_decode_28(void)
{
	uint8_t src[4] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0x2F; // 0010 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 2);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0x3F; // 0011 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 3);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0x4F; // 0100 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 4);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0x5F; // 0101 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 5);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0x6F; // 0110 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 6);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0x7F; // 0111 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 7);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0x8F; // 1000 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 8);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0x9F; // 1001 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 11);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0xAF; // 1010 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 12);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0xBF; // 1011 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 14);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0xCF; // 1100 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 15);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0xDF; // 1101 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 16);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0xEF; // 1110 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 17);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFE; // 11111110
	src[3] = 0xFF; // 1111 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 18);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x0F; // 0000 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 19);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x1F; // 0001 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 20);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x2F; // 0010 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 21);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x3F; // 0011 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 23);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x4F; // 0100 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 24);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x5F; // 0101 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 25);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x6F; // 0110 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 26);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x7F; // 0111 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 27);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x8F; // 1000 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 28);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0x9F; // 1001 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 29);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0xAF; // 1010 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 30);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0xBF; // 1011 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 31);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0xCF; // 1100 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 127);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0xDF; // 1101 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 220);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0xEF; // 1110 1111
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 249);
}

static void test_H2O_huffman_decode_30(void)
{
	uint8_t src[4] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0xF3; // 111100 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 10);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0xF7; // 111101 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 13);

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0xFB; // 111110 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT((dst[0]) == 22);
}

static void test_H2O_huffman_decode_EOS(void)
{
	uint8_t src[4] = {};
	uint8_t dst[1] = {};

	src[0] = 0xFF; // 11111111
	src[1] = 0xFF; // 11111111
	src[2] = 0xFF; // 11111111
	src[3] = 0xFF; // 111111 11
	huffman_decode(src, sizeof(src), dst, sizeof(dst));
	CU_ASSERT(!dst[0]);
}

bool h2o_huffman_decoder_ut_register()
{
	CU_pSuite pSuite = CU_add_suite(
	    "H2O Huffman decoder", init_suite_general, clean_suite_general);

	if (!pSuite)
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (5 bits)",
			 test_H2O_huffman_decode_5))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (6 bits)",
			 test_H2O_huffman_decode_6))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (7 bits)",
			 test_H2O_huffman_decode_7))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (8 bits)",
			 test_H2O_huffman_decode_8))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (10 bits)",
			 test_H2O_huffman_decode_10))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (11 bits)",
			 test_H2O_huffman_decode_11))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (12 bits)",
			 test_H2O_huffman_decode_12))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (13 bits)",
			 test_H2O_huffman_decode_13))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (14 bits)",
			 test_H2O_huffman_decode_14))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (15 bits)",
			 test_H2O_huffman_decode_15))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (19 bits)",
			 test_H2O_huffman_decode_19))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (20 bits)",
			 test_H2O_huffman_decode_20))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (21 bits)",
			 test_H2O_huffman_decode_21))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (22 bits)",
			 test_H2O_huffman_decode_22))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (23 bits)",
			 test_H2O_huffman_decode_23))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (24 bits)",
			 test_H2O_huffman_decode_24))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (25 bits)",
			 test_H2O_huffman_decode_25))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (26 bits)",
			 test_H2O_huffman_decode_26))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (27 bits)",
			 test_H2O_huffman_decode_27))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (28 bits)",
			 test_H2O_huffman_decode_28))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode (30 bits)",
			 test_H2O_huffman_decode_30))
		return false;

	if (!CU_add_test(pSuite, "test huffman decode EOS",
			 test_H2O_huffman_decode_EOS))
		return false;
	return true;
}
