#ifndef EASYRPGSYMBIAN_GOHUFONT_H
#define EASYRPGSYMBIAN_GOHUFONT_H

unsigned int EasyRpgTcvn3DecodeByte(unsigned int aByte);
int EasyRpgTcvn3EncodeCodepoint(unsigned int aCodepoint);
const unsigned char* EasyRpgGohuGlyphRows(unsigned int aCodepoint);

#endif
