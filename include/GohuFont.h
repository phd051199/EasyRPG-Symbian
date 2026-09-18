#ifndef EASYRPG_GOHU_FONT_H
#define EASYRPG_GOHU_FONT_H

unsigned int EasyRpgTcvn3DecodeByte(unsigned int aByte);
int EasyRpgTcvn3EncodeCodepoint(unsigned int aCodepoint);
const unsigned char *EasyRpgGohuGlyphRows(unsigned int aCodepoint);

#endif
