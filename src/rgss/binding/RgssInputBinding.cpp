#include "RgssBindingUtil.h"

#include "../core/RgssInput.h"
#include "RgssHost.h"

namespace {

rgss::RgssInputState gInput;
VALUE mInput = Qnil;

VALUE InputUpdate(VALUE)
{
    const RgssHostVtable* host = RgssHost();
    const unsigned int mask = host && host->PollInput
                                ? host->PollInput(host->ctx) : 0;
    gInput.Update(mask);
    return Qnil;
}

VALUE InputPress(VALUE, VALUE aButton)
{
    return gInput.Press(NUM2INT(aButton)) ? Qtrue : Qfalse;
}

VALUE InputTrigger(VALUE, VALUE aButton)
{
    return gInput.Trigger(NUM2INT(aButton)) ? Qtrue : Qfalse;
}

VALUE InputRelease(VALUE, VALUE aButton)
{
    return gInput.Release(NUM2INT(aButton)) ? Qtrue : Qfalse;
}

VALUE InputRepeat(VALUE, VALUE aButton)
{
    return gInput.Repeat(NUM2INT(aButton)) ? Qtrue : Qfalse;
}

VALUE InputDir4(VALUE)
{
    return INT2FIX(gInput.Dir4());
}

VALUE InputDir8(VALUE)
{
    return INT2FIX(gInput.Dir8());
}

void DefineButton(const char* aName, int aCode)
{
    rb_define_const(mInput, aName, INT2FIX(aCode));
}

} // namespace

void RgssInputBindingSetFrameRate(int aFramesPerSecond)
{
    gInput.SetFrameRate(aFramesPerSecond);
}

void RgssInputBindingInit(int aFramesPerSecond)
{
    gInput.SetFrameRate(aFramesPerSecond);
    mInput = rb_define_module("Input");

    DefineButton("DOWN", 2);
    DefineButton("LEFT", 4);
    DefineButton("RIGHT", 6);
    DefineButton("UP", 8);
    DefineButton("A", 11);
    DefineButton("B", 12);
    DefineButton("C", 13);
    DefineButton("X", 14);
    DefineButton("Y", 15);
    DefineButton("Z", 16);
    DefineButton("L", 17);
    DefineButton("R", 18);
    DefineButton("SHIFT", 21);
    DefineButton("CTRL", 22);
    DefineButton("ALT", 23);
    DefineButton("F5", 25);
    DefineButton("F6", 26);
    DefineButton("F7", 27);
    DefineButton("F8", 28);
    DefineButton("F9", 29);

    rb_define_module_function(mInput, "update", RUBY_METHOD_FUNC(InputUpdate), 0);
    rb_define_module_function(mInput, "press?", RUBY_METHOD_FUNC(InputPress), 1);
    rb_define_module_function(mInput, "trigger?", RUBY_METHOD_FUNC(InputTrigger), 1);
    rb_define_module_function(mInput, "release?", RUBY_METHOD_FUNC(InputRelease), 1);
    rb_define_module_function(mInput, "repeat?", RUBY_METHOD_FUNC(InputRepeat), 1);
    rb_define_module_function(mInput, "dir4", RUBY_METHOD_FUNC(InputDir4), 0);
    rb_define_module_function(mInput, "dir8", RUBY_METHOD_FUNC(InputDir8), 0);
}
