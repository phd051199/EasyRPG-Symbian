#include <memory>
#include <string>

#include "bitmap.h"
#include "font.h"
#include "rect.h"
#include "text.h"

namespace {
class SymbianFont : public Font {
public:
    SymbianFont(const std::string& name, int size, bool bold, bool italic)
        : Font(name, size, bold, italic) {}

    Rect GetSize(std::u32string const& text) const override {
        return Rect(0, 0, static_cast<int>(text.size()) * 6, 12);
    }

    BitmapRef Glyph(char32_t) override {
        return BitmapRef();
    }
};

FontRef gDefaultFont;
FontRef gMinchoFont;
}

FontRef Font::exfont;

Font::Font(const std::string& font_name, int font_size, bool font_bold, bool font_italic)
    : name(font_name), size(font_size), bold(font_bold), italic(font_italic) {}

Rect Font::GetSize(std::string const& text) const {
    return Rect(0, 0, static_cast<int>(text.size()) * 6, 12);
}

void Font::Render(Bitmap&, int, int, Bitmap const&, int, char32_t) {}
void Font::Render(Bitmap&, int, int, Color const&, char32_t) {}

FontRef Font::Create(const std::string& name, int size, bool bold, bool italic) {
    return FontRef(new SymbianFont(name, size, bold, italic));
}

FontRef Font::Default(bool mincho) {
    FontRef& font = mincho ? gMinchoFont : gDefaultFont;
    if (!font) font = Font::Create(mincho ? "Mincho" : "Gothic", default_size, default_bold, default_italic);
    return font;
}

void Font::Dispose() {
    gDefaultFont.reset();
    gMinchoFont.reset();
    exfont.reset();
}

namespace Text {
void Draw(Bitmap&, int, int, int, std::string const&, Text::Alignment) {}
void Draw(Bitmap&, int, int, Color, std::string const&) {}
}
