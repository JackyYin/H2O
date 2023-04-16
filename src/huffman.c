#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#define HUFFMAN_CHECKER_START(N)                                               \
	static bool huffman_check##N(uint8_t **buf, uint8_t *start_bit,        \
				     uint8_t *wb, size_t *wblen)               \
	{                                                                      \
		uint8_t bitlen = 0;                                            \
		uint8_t *tmpbuf = *buf;                                        \
		uint8_t tmpsb = *start_bit;                                    \
		uint32_t val = 0;                                              \
		while (bitlen < N) {                                           \
			uint8_t left = N - bitlen;                             \
			uint8_t curlen =                                       \
			    (8 - tmpsb) < left ? (8 - tmpsb) : left;           \
			val |= (((*tmpbuf) >> (8 - tmpsb - curlen)) &          \
				start_bit_masks[8 - curlen])                   \
			       << (N - bitlen - curlen);                       \
			bitlen += curlen;                                      \
			tmpsb += curlen;                                       \
			if (tmpsb > 7) {                                       \
				tmpsb &= 0x07;                                 \
				tmpbuf++;                                      \
			}                                                      \
		}

#define HUFFMAN_CHECKER_STORE(X) wb[(*wblen)++] = (uint8_t)(X);

#define HUFFMAN_CHECKER_CLOSE                                                  \
	*buf = tmpbuf;                                                         \
	*start_bit = tmpsb;                                                    \
	return true;                                                           \
	}

static uint8_t start_bit_masks[8] = {0xFF, 0x7F, 0x3F, 0x1F,
				     0x0F, 0x07, 0x03, 0x01};

HUFFMAN_CHECKER_START(5)
switch (val) {
case 0:
	HUFFMAN_CHECKER_STORE('0');
	break;
case 1:
	HUFFMAN_CHECKER_STORE('1');
	break;
case 2:
	HUFFMAN_CHECKER_STORE('2');
	break;
case 3:
	HUFFMAN_CHECKER_STORE('a');
	break;
case 4:
	HUFFMAN_CHECKER_STORE('c');
	break;
case 5:
	HUFFMAN_CHECKER_STORE('e');
	break;
case 6:
	HUFFMAN_CHECKER_STORE('i');
	break;
case 7:
	HUFFMAN_CHECKER_STORE('o');
	break;
case 8:
	HUFFMAN_CHECKER_STORE('s');
	break;
case 9:
	HUFFMAN_CHECKER_STORE('t');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(6)
switch (val) {
case 0x14:
	HUFFMAN_CHECKER_STORE(' ');
	break;
case 0x15:
	HUFFMAN_CHECKER_STORE('%');
	break;
case 0x16:
	HUFFMAN_CHECKER_STORE('-');
	break;
case 0x17:
	HUFFMAN_CHECKER_STORE('.');
	break;
case 0x18:
	HUFFMAN_CHECKER_STORE('/');
	break;
case 0x19:
	HUFFMAN_CHECKER_STORE('3');
	break;
case 0x1a:
	HUFFMAN_CHECKER_STORE('4');
	break;
case 0x1b:
	HUFFMAN_CHECKER_STORE('5');
	break;
case 0x1c:
	HUFFMAN_CHECKER_STORE('6');
	break;
case 0x1d:
	HUFFMAN_CHECKER_STORE('7');
	break;
case 0x1e:
	HUFFMAN_CHECKER_STORE('8');
	break;
case 0x1f:
	HUFFMAN_CHECKER_STORE('9');
	break;
case 0x20:
	HUFFMAN_CHECKER_STORE('=');
	break;
case 0x21:
	HUFFMAN_CHECKER_STORE('A');
	break;
case 0x22:
	HUFFMAN_CHECKER_STORE('_');
	break;
case 0x23:
	HUFFMAN_CHECKER_STORE('b');
	break;
case 0x24:
	HUFFMAN_CHECKER_STORE('d');
	break;
case 0x25:
	HUFFMAN_CHECKER_STORE('f');
	break;
case 0x26:
	HUFFMAN_CHECKER_STORE('g');
	break;
case 0x27:
	HUFFMAN_CHECKER_STORE('h');
	break;
case 0x28:
	HUFFMAN_CHECKER_STORE('l');
	break;
case 0x29:
	HUFFMAN_CHECKER_STORE('m');
	break;
case 0x2a:
	HUFFMAN_CHECKER_STORE('n');
	break;
case 0x2b:
	HUFFMAN_CHECKER_STORE('p');
	break;
case 0x2c:
	HUFFMAN_CHECKER_STORE('r');
	break;
case 0x2d:
	HUFFMAN_CHECKER_STORE('u');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(7)
switch (val) {
case 0x5c:
	HUFFMAN_CHECKER_STORE(':');
	break;
case 0x5d:
	HUFFMAN_CHECKER_STORE('B');
	break;
case 0x5e:
	HUFFMAN_CHECKER_STORE('C');
	break;
case 0x5f:
	HUFFMAN_CHECKER_STORE('D');
	break;
case 0x60:
	HUFFMAN_CHECKER_STORE('E');
	break;
case 0x61:
	HUFFMAN_CHECKER_STORE('F');
	break;
case 0x62:
	HUFFMAN_CHECKER_STORE('G');
	break;
case 0x63:
	HUFFMAN_CHECKER_STORE('H');
	break;
case 0x64:
	HUFFMAN_CHECKER_STORE('I');
	break;
case 0x65:
	HUFFMAN_CHECKER_STORE('J');
	break;
case 0x66:
	HUFFMAN_CHECKER_STORE('K');
	break;
case 0x67:
	HUFFMAN_CHECKER_STORE('L');
	break;
case 0x68:
	HUFFMAN_CHECKER_STORE('M');
	break;
case 0x69:
	HUFFMAN_CHECKER_STORE('N');
	break;
case 0x6a:
	HUFFMAN_CHECKER_STORE('O');
	break;
case 0x6b:
	HUFFMAN_CHECKER_STORE('P');
	break;
case 0x6c:
	HUFFMAN_CHECKER_STORE('Q');
	break;
case 0x6d:
	HUFFMAN_CHECKER_STORE('R');
	break;
case 0x6e:
	HUFFMAN_CHECKER_STORE('S');
	break;
case 0x6f:
	HUFFMAN_CHECKER_STORE('T');
	break;
case 0x70:
	HUFFMAN_CHECKER_STORE('U');
	break;
case 0x71:
	HUFFMAN_CHECKER_STORE('V');
	break;
case 0x72:
	HUFFMAN_CHECKER_STORE('W');
	break;
case 0x73:
	HUFFMAN_CHECKER_STORE('Y');
	break;
case 0x74:
	HUFFMAN_CHECKER_STORE('j');
	break;
case 0x75:
	HUFFMAN_CHECKER_STORE('k');
	break;
case 0x76:
	HUFFMAN_CHECKER_STORE('q');
	break;
case 0x77:
	HUFFMAN_CHECKER_STORE('v');
	break;
case 0x78:
	HUFFMAN_CHECKER_STORE('w');
	break;
case 0x79:
	HUFFMAN_CHECKER_STORE('x');
	break;
case 0x7a:
	HUFFMAN_CHECKER_STORE('y');
	break;
case 0x7b:
	HUFFMAN_CHECKER_STORE('z');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(8)
switch (val) {
case 0xf8:
	HUFFMAN_CHECKER_STORE('&');
	break;
case 0xf9:
	HUFFMAN_CHECKER_STORE('*');
	break;
case 0xfa:
	HUFFMAN_CHECKER_STORE(',');
	break;
case 0xfb:
	HUFFMAN_CHECKER_STORE(';');
	break;
case 0xfc:
	HUFFMAN_CHECKER_STORE('X');
	break;
case 0xfd:
	HUFFMAN_CHECKER_STORE('Z');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(10)
switch (val) {
case 0x03f8:
	HUFFMAN_CHECKER_STORE('!');
	break;
case 0x03f9:
	HUFFMAN_CHECKER_STORE('"');
	break;
case 0x03fa:
	HUFFMAN_CHECKER_STORE('(');
	break;
case 0x03fb:
	HUFFMAN_CHECKER_STORE(')');
	break;
case 0x03fc:
	HUFFMAN_CHECKER_STORE('?');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(11)
switch (val) {
case 0x07fa:
	HUFFMAN_CHECKER_STORE('\'');
	break;
case 0x07fb:
	HUFFMAN_CHECKER_STORE('+');
	break;
case 0x07fc:
	HUFFMAN_CHECKER_STORE('|');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(12)
switch (val) {
case 0x0ffa:
	HUFFMAN_CHECKER_STORE('#');
	break;
case 0x0ffb:
	HUFFMAN_CHECKER_STORE('>');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(13)
switch (val) {
case 0x1ff8:
	HUFFMAN_CHECKER_STORE(0);
	break;
case 0x1ff9:
	HUFFMAN_CHECKER_STORE('$');
	break;
case 0x1ffa:
	HUFFMAN_CHECKER_STORE('@');
	break;
case 0x1ffb:
	HUFFMAN_CHECKER_STORE('[');
	break;
case 0x1ffc:
	HUFFMAN_CHECKER_STORE(']');
	break;
case 0x1ffd:
	HUFFMAN_CHECKER_STORE('~');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(14)
switch (val) {
case 0x3ffc:
	HUFFMAN_CHECKER_STORE('^');
	break;
case 0x3ffd:
	HUFFMAN_CHECKER_STORE('}');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(15)
switch (val) {
case 0x7ffc:
	HUFFMAN_CHECKER_STORE('<');
	break;
case 0x7ffd:
	HUFFMAN_CHECKER_STORE('`');
	break;
case 0x7ffe:
	HUFFMAN_CHECKER_STORE('{');
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(19)
switch (val) {
case 0x07fff0:
	HUFFMAN_CHECKER_STORE('\\');
	break;
case 0x07fff1:
	HUFFMAN_CHECKER_STORE(195);
	break;
case 0x07fff2:
	HUFFMAN_CHECKER_STORE(208);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(20)
switch (val) {
case 0x0fffe6:
	HUFFMAN_CHECKER_STORE(128);
	break;
case 0x0fffe7:
	HUFFMAN_CHECKER_STORE(130);
	break;
case 0x0fffe8:
	HUFFMAN_CHECKER_STORE(131);
	break;
case 0x0fffe9:
	HUFFMAN_CHECKER_STORE(162);
	break;
case 0x0fffea:
	HUFFMAN_CHECKER_STORE(184);
	break;
case 0x0fffeb:
	HUFFMAN_CHECKER_STORE(194);
	break;
case 0x0fffec:
	HUFFMAN_CHECKER_STORE(224);
	break;
case 0x0fffed:
	HUFFMAN_CHECKER_STORE(226);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(21)
switch (val) {
case 0x1fffdc:
	HUFFMAN_CHECKER_STORE(153);
	break;
case 0x1fffdd:
	HUFFMAN_CHECKER_STORE(161);
	break;
case 0x1fffde:
	HUFFMAN_CHECKER_STORE(167);
	break;
case 0x1fffdf:
	HUFFMAN_CHECKER_STORE(172);
	break;
case 0x1fffe0:
	HUFFMAN_CHECKER_STORE(176);
	break;
case 0x1fffe1:
	HUFFMAN_CHECKER_STORE(177);
	break;
case 0x1fffe2:
	HUFFMAN_CHECKER_STORE(179);
	break;
case 0x1fffe3:
	HUFFMAN_CHECKER_STORE(209);
	break;
case 0x1fffe4:
	HUFFMAN_CHECKER_STORE(216);
	break;
case 0x1fffe5:
	HUFFMAN_CHECKER_STORE(217);
	break;
case 0x1fffe6:
	HUFFMAN_CHECKER_STORE(227);
	break;
case 0x1fffe7:
	HUFFMAN_CHECKER_STORE(229);
	break;
case 0x1fffe8:
	HUFFMAN_CHECKER_STORE(230);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(22)
switch (val) {
case 0x3fffd2:
	HUFFMAN_CHECKER_STORE(129);
	break;
case 0x3fffd3:
	HUFFMAN_CHECKER_STORE(132);
	break;
case 0x3fffd4:
	HUFFMAN_CHECKER_STORE(133);
	break;
case 0x3fffd5:
	HUFFMAN_CHECKER_STORE(134);
	break;
case 0x3fffd6:
	HUFFMAN_CHECKER_STORE(136);
	break;
case 0x3fffd7:
	HUFFMAN_CHECKER_STORE(146);
	break;
case 0x3fffd8:
	HUFFMAN_CHECKER_STORE(154);
	break;
case 0x3fffd9:
	HUFFMAN_CHECKER_STORE(156);
	break;
case 0x3fffda:
	HUFFMAN_CHECKER_STORE(160);
	break;
case 0x3fffdb:
	HUFFMAN_CHECKER_STORE(163);
	break;
case 0x3fffdc:
	HUFFMAN_CHECKER_STORE(164);
	break;
case 0x3fffdd:
	HUFFMAN_CHECKER_STORE(169);
	break;
case 0x3fffde:
	HUFFMAN_CHECKER_STORE(170);
	break;
case 0x3fffdf:
	HUFFMAN_CHECKER_STORE(173);
	break;
case 0x3fffe0:
	HUFFMAN_CHECKER_STORE(178);
	break;
case 0x3fffe1:
	HUFFMAN_CHECKER_STORE(181);
	break;
case 0x3fffe2:
	HUFFMAN_CHECKER_STORE(185);
	break;
case 0x3fffe3:
	HUFFMAN_CHECKER_STORE(186);
	break;
case 0x3fffe4:
	HUFFMAN_CHECKER_STORE(187);
	break;
case 0x3fffe5:
	HUFFMAN_CHECKER_STORE(189);
	break;
case 0x3fffe6:
	HUFFMAN_CHECKER_STORE(190);
	break;
case 0x3fffe7:
	HUFFMAN_CHECKER_STORE(196);
	break;
case 0x3fffe8:
	HUFFMAN_CHECKER_STORE(198);
	break;
case 0x3fffe9:
	HUFFMAN_CHECKER_STORE(228);
	break;
case 0x3fffea:
	HUFFMAN_CHECKER_STORE(232);
	break;
case 0x3fffeb:
	HUFFMAN_CHECKER_STORE(233);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(23)
switch (val) {
case 0x7fffd8:
	HUFFMAN_CHECKER_STORE(1);
	break;
case 0x7fffd9:
	HUFFMAN_CHECKER_STORE(135);
	break;
case 0x7fffda:
	HUFFMAN_CHECKER_STORE(137);
	break;
case 0x7fffdb:
	HUFFMAN_CHECKER_STORE(138);
	break;
case 0x7fffdc:
	HUFFMAN_CHECKER_STORE(139);
	break;
case 0x7fffdd:
	HUFFMAN_CHECKER_STORE(140);
	break;
case 0x7fffde:
	HUFFMAN_CHECKER_STORE(141);
	break;
case 0x7fffdf:
	HUFFMAN_CHECKER_STORE(143);
	break;
case 0x7fffe0:
	HUFFMAN_CHECKER_STORE(147);
	break;
case 0x7fffe1:
	HUFFMAN_CHECKER_STORE(149);
	break;
case 0x7fffe2:
	HUFFMAN_CHECKER_STORE(150);
	break;
case 0x7fffe3:
	HUFFMAN_CHECKER_STORE(151);
	break;
case 0x7fffe4:
	HUFFMAN_CHECKER_STORE(152);
	break;
case 0x7fffe5:
	HUFFMAN_CHECKER_STORE(155);
	break;
case 0x7fffe6:
	HUFFMAN_CHECKER_STORE(157);
	break;
case 0x7fffe7:
	HUFFMAN_CHECKER_STORE(158);
	break;
case 0x7fffe8:
	HUFFMAN_CHECKER_STORE(165);
	break;
case 0x7fffe9:
	HUFFMAN_CHECKER_STORE(166);
	break;
case 0x7fffea:
	HUFFMAN_CHECKER_STORE(168);
	break;
case 0x7fffeb:
	HUFFMAN_CHECKER_STORE(174);
	break;
case 0x7fffec:
	HUFFMAN_CHECKER_STORE(175);
	break;
case 0x7fffed:
	HUFFMAN_CHECKER_STORE(180);
	break;
case 0x7fffee:
	HUFFMAN_CHECKER_STORE(182);
	break;
case 0x7fffef:
	HUFFMAN_CHECKER_STORE(183);
	break;
case 0x7ffff0:
	HUFFMAN_CHECKER_STORE(188);
	break;
case 0x7ffff1:
	HUFFMAN_CHECKER_STORE(191);
	break;
case 0x7ffff2:
	HUFFMAN_CHECKER_STORE(197);
	break;
case 0x7ffff3:
	HUFFMAN_CHECKER_STORE(231);
	break;
case 0x7ffff4:
	HUFFMAN_CHECKER_STORE(239);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(24)
switch (val) {
case 0xffffea:
	HUFFMAN_CHECKER_STORE(9);
	break;
case 0xffffeb:
	HUFFMAN_CHECKER_STORE(142);
	break;
case 0xffffec:
	HUFFMAN_CHECKER_STORE(144);
	break;
case 0xffffed:
	HUFFMAN_CHECKER_STORE(145);
	break;
case 0xffffee:
	HUFFMAN_CHECKER_STORE(148);
	break;
case 0xffffef:
	HUFFMAN_CHECKER_STORE(159);
	break;
case 0xfffff0:
	HUFFMAN_CHECKER_STORE(171);
	break;
case 0xfffff1:
	HUFFMAN_CHECKER_STORE(206);
	break;
case 0xfffff2:
	HUFFMAN_CHECKER_STORE(215);
	break;
case 0xfffff3:
	HUFFMAN_CHECKER_STORE(225);
	break;
case 0xfffff4:
	HUFFMAN_CHECKER_STORE(236);
	break;
case 0xfffff5:
	HUFFMAN_CHECKER_STORE(237);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(25)
switch (val) {
case 0x01ffffec:
	HUFFMAN_CHECKER_STORE(199);
	break;
case 0x01ffffed:
	HUFFMAN_CHECKER_STORE(207);
	break;
case 0x01ffffee:
	HUFFMAN_CHECKER_STORE(234);
	break;
case 0x01ffffef:
	HUFFMAN_CHECKER_STORE(235);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(26)
switch (val) {
case 0x03ffffe0:
	HUFFMAN_CHECKER_STORE(192);
	break;
case 0x03ffffe1:
	HUFFMAN_CHECKER_STORE(193);
	break;
case 0x03ffffe2:
	HUFFMAN_CHECKER_STORE(200);
	break;
case 0x03ffffe3:
	HUFFMAN_CHECKER_STORE(201);
	break;
case 0x03ffffe4:
	HUFFMAN_CHECKER_STORE(202);
	break;
case 0x03ffffe5:
	HUFFMAN_CHECKER_STORE(205);
	break;
case 0x03ffffe6:
	HUFFMAN_CHECKER_STORE(210);
	break;
case 0x03ffffe7:
	HUFFMAN_CHECKER_STORE(213);
	break;
case 0x03ffffe8:
	HUFFMAN_CHECKER_STORE(218);
	break;
case 0x03ffffe9:
	HUFFMAN_CHECKER_STORE(219);
	break;
case 0x03ffffea:
	HUFFMAN_CHECKER_STORE(238);
	break;
case 0x03ffffeb:
	HUFFMAN_CHECKER_STORE(240);
	break;
case 0x03ffffec:
	HUFFMAN_CHECKER_STORE(242);
	break;
case 0x03ffffed:
	HUFFMAN_CHECKER_STORE(243);
	break;
case 0x03ffffee:
	HUFFMAN_CHECKER_STORE(255);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(27)
switch (val) {
case 0x07ffffde:
	HUFFMAN_CHECKER_STORE(203);
	break;
case 0x07ffffdf:
	HUFFMAN_CHECKER_STORE(204);
	break;
case 0x07ffffe0:
	HUFFMAN_CHECKER_STORE(211);
	break;
case 0x07ffffe1:
	HUFFMAN_CHECKER_STORE(212);
	break;
case 0x07ffffe2:
	HUFFMAN_CHECKER_STORE(214);
	break;
case 0x07ffffe3:
	HUFFMAN_CHECKER_STORE(221);
	break;
case 0x07ffffe4:
	HUFFMAN_CHECKER_STORE(222);
	break;
case 0x07ffffe5:
	HUFFMAN_CHECKER_STORE(223);
	break;
case 0x07ffffe6:
	HUFFMAN_CHECKER_STORE(241);
	break;
case 0x07ffffe7:
	HUFFMAN_CHECKER_STORE(244);
	break;
case 0x07ffffe8:
	HUFFMAN_CHECKER_STORE(245);
	break;
case 0x07ffffe9:
	HUFFMAN_CHECKER_STORE(246);
	break;
case 0x07ffffea:
	HUFFMAN_CHECKER_STORE(247);
	break;
case 0x07ffffeb:
	HUFFMAN_CHECKER_STORE(248);
	break;
case 0x07ffffec:
	HUFFMAN_CHECKER_STORE(250);
	break;
case 0x07ffffed:
	HUFFMAN_CHECKER_STORE(251);
	break;
case 0x07ffffee:
	HUFFMAN_CHECKER_STORE(252);
	break;
case 0x07ffffef:
	HUFFMAN_CHECKER_STORE(253);
	break;
case 0x07fffff0:
	HUFFMAN_CHECKER_STORE(254);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(28)
switch (val) {
case 0x0fffffe2:
	HUFFMAN_CHECKER_STORE(2);
	break;
case 0x0fffffe3:
	HUFFMAN_CHECKER_STORE(3);
	break;
case 0x0fffffe4:
	HUFFMAN_CHECKER_STORE(4);
	break;
case 0x0fffffe5:
	HUFFMAN_CHECKER_STORE(5);
	break;
case 0x0fffffe6:
	HUFFMAN_CHECKER_STORE(6);
	break;
case 0x0fffffe7:
	HUFFMAN_CHECKER_STORE(7);
	break;
case 0x0fffffe8:
	HUFFMAN_CHECKER_STORE(8);
	break;
case 0x0fffffe9:
	HUFFMAN_CHECKER_STORE(11);
	break;
case 0x0fffffea:
	HUFFMAN_CHECKER_STORE(12);
	break;
case 0x0fffffeb:
	HUFFMAN_CHECKER_STORE(14);
	break;
case 0x0fffffec:
	HUFFMAN_CHECKER_STORE(15);
	break;
case 0x0fffffed:
	HUFFMAN_CHECKER_STORE(16);
	break;
case 0x0fffffee:
	HUFFMAN_CHECKER_STORE(17);
	break;
case 0x0fffffef:
	HUFFMAN_CHECKER_STORE(18);
	break;
case 0x0ffffff0:
	HUFFMAN_CHECKER_STORE(19);
	break;
case 0x0ffffff1:
	HUFFMAN_CHECKER_STORE(20);
	break;
case 0x0ffffff2:
	HUFFMAN_CHECKER_STORE(21);
	break;
case 0x0ffffff3:
	HUFFMAN_CHECKER_STORE(23);
	break;
case 0x0ffffff4:
	HUFFMAN_CHECKER_STORE(24);
	break;
case 0x0ffffff5:
	HUFFMAN_CHECKER_STORE(25);
	break;
case 0x0ffffff6:
	HUFFMAN_CHECKER_STORE(26);
	break;
case 0x0ffffff7:
	HUFFMAN_CHECKER_STORE(27);
	break;
case 0x0ffffff8:
	HUFFMAN_CHECKER_STORE(28);
	break;
case 0x0ffffff9:
	HUFFMAN_CHECKER_STORE(29);
	break;
case 0x0ffffffa:
	HUFFMAN_CHECKER_STORE(30);
	break;
case 0x0ffffffb:
	HUFFMAN_CHECKER_STORE(31);
	break;
case 0x0ffffffc:
	HUFFMAN_CHECKER_STORE(127);
	break;
case 0x0ffffffd:
	HUFFMAN_CHECKER_STORE(220);
	break;
case 0x0ffffffe:
	HUFFMAN_CHECKER_STORE(249);
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

HUFFMAN_CHECKER_START(30)
switch (val) {
case 0x3ffffffc:
	HUFFMAN_CHECKER_STORE(10);
	break;
case 0x3ffffffd:
	HUFFMAN_CHECKER_STORE(13);
	break;
case 0x3ffffffe:
	HUFFMAN_CHECKER_STORE(22);
	break;
case 0x3fffffff:
	/*
	 * EOS
	 * We are not able to store 256 in a uint8_t, do nothing!
	 */
	break;
default:
	return false;
}
HUFFMAN_CHECKER_CLOSE

void huffman_decode(uint8_t *sbuf, size_t sbuflen, uint8_t *dbuf,
		    size_t dbuflen)
{
	uint8_t *bufend = sbuf + sbuflen;
	uint8_t start_bits = 0;
	size_t curdblen = 0;

	while (sbuf < bufend) {
		uint8_t bits_cnt = 0xff;

		if (curdblen >= dbuflen)
			break;

		if ((sbuf + 1) == bufend)
			bits_cnt = (8 - start_bits);

		if (bits_cnt < 5)
			break;

		if (huffman_check5(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 6)
			break;

		if (huffman_check6(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 7)
			break;

		if (huffman_check7(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 8)
			break;

		if (huffman_check8(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if ((sbuf + 2) > bufend)
			break;
		else if ((sbuf + 2) == bufend)
			bits_cnt = (8 - start_bits) + 8;

		if (bits_cnt < 10)
			break;

		if (huffman_check10(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 11)
			break;

		if (huffman_check11(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 12)
			break;

		if (huffman_check12(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 13)
			break;

		if (huffman_check13(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 14)
			break;

		if (huffman_check14(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 15)
			break;

		if (huffman_check15(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if ((sbuf + 3) > bufend)
			break;
		else if ((sbuf + 3) == bufend)
			bits_cnt = (8 - start_bits) + 16;

		if (bits_cnt < 19)
			break;

		if (huffman_check19(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 20)
			break;

		if (huffman_check20(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 21)
			break;

		if (huffman_check21(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 22)
			break;

		if (huffman_check22(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 23)
			break;

		if (huffman_check23(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 24)
			break;

		if (huffman_check24(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if ((sbuf + 4) > bufend)
			break;
		else if ((sbuf + 4) == bufend)
			bits_cnt = (8 - start_bits) + 24;

		if (huffman_check25(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 26)
			break;

		if (huffman_check26(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 27)
			break;

		if (huffman_check27(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 28)
			break;

		if (huffman_check28(&sbuf, &start_bits, dbuf, &curdblen))
			continue;

		if (bits_cnt < 30)
			break;

		if (huffman_check30(&sbuf, &start_bits, dbuf, &curdblen))
			continue;
	}
}
