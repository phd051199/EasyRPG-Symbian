#ifndef EASYRPG_RGSS_SHINONOME_FONT_H
#define EASYRPG_RGSS_SHINONOME_FONT_H

#include <stdint.h>

struct ShinonomeGlyph
{
    uint16_t code;
    bool is_full;
    uint16_t data[12];
};

extern const ShinonomeGlyph SHINONOME_GOTHIC[11217];

#endif
