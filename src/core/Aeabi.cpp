extern "C" int __aeabi_idivmod(int numerator, int denominator);
extern "C" unsigned int __aeabi_uidivmod(unsigned int numerator, unsigned int denominator);

extern "C" int __aeabi_idiv(int numerator, int denominator)
{
    return __aeabi_idivmod(numerator, denominator);
}

extern "C" unsigned int __aeabi_uidiv(unsigned int numerator, unsigned int denominator)
{
    return __aeabi_uidivmod(numerator, denominator);
}
