#include <limits.h>
#include <math.h>

int isnan(double x) { return x != x; }

double floor(double x) {
    if (x >= LLONG_MAX || x <= LLONG_MIN || isnan(x)) {
        // x doesn't have any fractional digits in significant part
        return x;
    }
    long long xl = ((long long)x);
    if (x >= 0) {
        return xl;
    } else {
        if (x == xl) {
            return xl;
        } else {
            return xl - 1;
        }
    }
}

double round(double x) { return floor(x + 0.5); }

double fmod(double x, double y) { return x - floor(x / y) * y; }

double fabs(double x) { return (x >= 0) ? x : -x; }

double sin(double x) {
    const int terms = 32;
    const double x2 = x * x;
    double r = x; // first term short-circuited
    double term = x;
    for (int i = 1, deno = 2; i < terms; i++, deno += 2) {
        term = (-term * x2) / (deno * (deno + 1));
        r += term;
    }
    return r;
}

double cos(double x) { return sin(M_PI / 2 - x); }
