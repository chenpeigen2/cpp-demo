// 17 - <math.h>：数学函数与特殊值
#include <stdio.h>
#include <math.h>

int main(void) {
    // 基本函数
    printf("sqrt(2)=%.4f pow(2,10)=%g exp(1)=%.4f log(e)=%g log2(8)=%g\n",
           sqrt(2.0), pow(2.0, 10.0), exp(1.0), log(M_E), log2(8.0));

    // 三角（参数是弧度）
    printf("sin(pi/2)=%g cos(0)=%g atan2(1,1)=%.4f\n",
           sin(M_PI / 2), cos(0.0), atan2(1.0, 1.0));

    // 取整一族：floor 向下 ceil 向上 round 四舍五入(远离0) trunc 截断
    printf("floor(-2.5)=%g ceil(-2.5)=%g round(-2.5)=%g trunc(-2.5)=%g\n",
           floor(-2.5), ceil(-2.5), round(-2.5), trunc(-2.5));

    // fmod 取余（浮点）、fabs 绝对值、fmin/fmax
    printf("fmod(7.5,2)=%g fabs(-3.2)=%g fmax(1,2)=%g\n",
           fmod(7.5, 2.0), fabs(-3.2), fmax(1.0, 2.0));

    // 浮点特殊值：INFINITY 与 NaN 有独特行为
    double inf = HUGE_VAL;              // 无穷大
    double nan = NAN;
    printf("inf+1=%g 1/inf=%g inf==inf=%d\n", inf + 1.0, 1.0 / inf, inf == inf);
    printf("nan==nan=%d（NaN 不等于任何值）isnan(nan)=%d isinf(inf)=%d\n",
           nan == nan, isnan(nan), isinf(inf));

    // hypot 避免中间溢出；cbrt 立方根
    printf("hypot(3,4)=%g cbrt(27)=%g\n", hypot(3.0, 4.0), cbrt(27.0));
    return 0;
}
