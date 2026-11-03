// 31 - 引用：别名语义，区别于指针
#include <iostream>

void swap(int &a, int &b) {        // 引用传参：函数内改 a/b 就是改实参
    int t = a; a = b; b = t;
}

void modify(int *p) { *p += 100; } // 指针版本对比

int main() {
    int x = 1;
    int &ref = x;                  // 引用必须初始化，且终生绑定一个对象
    ref = 10;                      // 通过引用改 x
    std::println("x={} ref={}", x, ref);

    int y = 2, z = 3;
    swap(y, z);
    std::println("y={} z={}", y, z);
    modify(&y);
    std::println("y={}", y);

    // const 引用：可以绑定临时值（右值），延长其生命周期
    const int &cr = 42;
    std::println("cr={}", cr);

    // 引用与指针的区别：引用无"空值"、无算术、不占语义空间；指针可为空可重指
    int *p = &x;
    p = &y;                        // 指针可以改指
    std::println("*p={} sizeof(ref)={}", *p, sizeof(ref));  // sizeof(ref)==sizeof(int)
    return 0;
}
