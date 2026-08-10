#ifndef EASYRPG_SYMBIAN_LIBC_COMPAT_MATH_H
#define EASYRPG_SYMBIAN_LIBC_COMPAT_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

double sin(double);
double cos(double);
double tan(double);
double asin(double);
double acos(double);
double atan(double);
double atan2(double, double);
double sqrt(double);
double pow(double, double);
double floor(double);
double ceil(double);
double fmod(double, double);
double fabs(double);
float sinf(float);
float cosf(float);
float sqrtf(float);
float floorf(float);
float ceilf(float);

#ifdef __cplusplus
}
#endif

#endif
