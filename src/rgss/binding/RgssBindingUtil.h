#ifndef RGSS_BINDING_UTIL_H
#define RGSS_BINDING_UTIL_H

/*
 * Minimal port of mkxp-z binding/binding-util.h for Ruby 3.0 only.
 * Compiled with -fno-exceptions: rb_raise (longjmp based) is the only
 * error path; never keep non-trivial C++ destructors in scope across
 * an rb_raise.
 */

#include "ruby.h"

/* Defined in RgssBindingInit.cpp; base class is rb_eStandardError. */
extern VALUE eRgssError;

inline void rgssRaise(const char* aMessage)
{
    rb_raise(eRgssError, "%s", aMessage);
}

/*
 * Per-class typed-data boilerplate. In the class binding .cpp do
 * `using rgss::Table;` first, then:
 *   RGSS_DEFINE_TYPE(Table)            -> Table##Type + alloc func
 *   Table* t = RGSS_GET(self, Table);  -> raises TypeError on mismatch
 *   RGSS_ATTACH(obj, new Table);       -> attach to klass.allocate result
 * `new` never throws here (no exceptions); allocation failure aborts.
 */
#define RGSS_DEFINE_TYPE(Klass)                                        \
    static void Klass##Free(void* aData)                               \
    {                                                                  \
        delete static_cast<Klass*>(aData);                             \
    }                                                                  \
    const rb_data_type_t Klass##Type = {                               \
        #Klass, { 0, Klass##Free, 0, 0 }, 0, 0,                        \
        RUBY_TYPED_FREE_IMMEDIATELY };                                 \
    static VALUE Klass##Alloc(VALUE aKlass)                            \
    {                                                                  \
        return TypedData_Wrap_Struct(aKlass, &Klass##Type, new Klass); \
    }

#define RGSS_GET(aSelf, Klass) rgssGetData<Klass>(aSelf, &Klass##Type)

template<typename C>
C* rgssGetData(VALUE aSelf, const rb_data_type_t* aType)
{
    C* data = 0;
    TypedData_Get_Struct(aSelf, C, aType, data);
    if (!data)
        rgssRaise("uninitialized object");
    return data;
}

/* Attach a core to an allocated-but-uninitialized object (rb_obj_alloc). */
template<typename C>
void rgssAttach(VALUE aSelf, C* aData, const rb_data_type_t* aType)
{
    if (RTYPEDDATA_TYPE(aSelf) != aType)
        rb_raise(rb_eTypeError, "wrong argument type");
    if (RTYPEDDATA_DATA(aSelf))
        delete static_cast<C*>(RTYPEDDATA_DATA(aSelf));
    RTYPEDDATA_DATA(aSelf) = aData;
}

#define RGSS_ATTACH(aSelf, aData, Klass) \
    rgssAttach<Klass>(aSelf, aData, &Klass##Type)

#endif
