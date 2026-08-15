#include "ruby.h"

VALUE eRgssError = Qnil;

void RgssTableBindingInit();
void RgssColorBindingInit();
void RgssToneBindingInit();
void RgssRectBindingInit();
void RgssFileIntBindingInit();
void RgssZlibBindingInit();
int RgssRendererBindingInit(int aGeneration);
void RgssInputBindingInit(int aFramesPerSecond);
void RgssAudioBindingInit();
void RgssM4BindingInit(int aGeneration);

int RgssBindingInit(int aGeneration)
{
    eRgssError = rb_define_class("RGSSError", rb_eStandardError);

    RgssTableBindingInit();
    RgssColorBindingInit();
    RgssToneBindingInit();
    RgssRectBindingInit();
    RgssFileIntBindingInit();
    RgssZlibBindingInit();
    if (RgssRendererBindingInit(aGeneration) != 0)
        return -1;
    RgssM4BindingInit(aGeneration);
    RgssInputBindingInit(40);
    RgssAudioBindingInit();
    return 0;
}
