#include <stdio.h>

class I {
    int i;
public:
    I() : i(6) { printf("owl\n"); }
    I(int a) : i(a) { printf("sheep %d\n", i); }
    I(const I &other) : i(other.i) { printf("horse %d\n", i); }
    ~I() { printf("wolf\n"); }
    int Get() { return i; }
    void operator*=(const I &op) { i *= op.i; }
};
void f( I x, I &y ) {
    x *= 1;
    y *= x;
}
int main() {
    I i1;
    I i2(3);
    i1 *= 7;
    f(i1, i2);
    printf("%d %d\n", i1.Get(), i2.Get());
    return 0;
}
