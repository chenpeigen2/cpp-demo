// 23 - <complex.h>：复数运算（C99 原生支持）
#include <stdio.h>
#include <complex.h>
#include <math.h>

int main(void) {
    // complex 是 _Complex 的宏； double _Complex 是复数类型
    double complex z1 = 1.0 + 2.0 * I;   // I 是虚数单位
    double complex z2 = 3.0 - 1.0 * I;

    // 四则运算直接重载在类型上（C 里是类型规则，不是运算符重载）
    printf("z1=%g%+gi z2=%g%+gi\n", creal(z1), cimag(z1), creal(z2), cimag(z2));
    printf("z1+z2=%g%+gi\n", creal(z1 + z2), cimag(z1 + z2));
    printf("z1*z2=%g%+gi\n", creal(z1 * z2), cimag(z1 * z2));
    printf("z1/z2=%g%+gi\n", creal(z1 / z2), cimag(z1 / z2));

    // 共轭、模、辐角
    double complex conj_z = conj(z1);
    printf("conj(z1)=%g%+gi |z1|=%g arg(z1)=%.4f\n",
           creal(conj_z), cimag(conj_z), cabs(z1), carg(z1));

    // 复指数与复平方根
    double complex e = cexp(I * M_PI);          // e^(iπ) = -1
    printf("e^(iπ)=%g%+gi（应为 -1）\n", creal(e), cimag(e));
    double complex s = csqrt(-4.0);
    printf("sqrt(-4)=%g%+gi\n", creal(s), cimag(s));

    // 投影函数
    printf("creal=%g cimag=%g\n", creal(z1), cimag(z1));
    return 0;
}
