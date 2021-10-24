#pragma once

#define M_PI 3.14159265358979323846

#ifdef __cplusplus
extern "C" {
namespace std {
#endif

int isnan(double x);
double floor(double x);
double round(double x);
double fmod(double x, double y);
double fabs(double x);

double sin(double x);
double cos(double x);

#ifdef __cplusplus
} // namespace std end
} // extern C end
#endif
