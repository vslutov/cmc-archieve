#include "ps04-1.cpp"

int
main()
{
    size_t size = 6;
    vector<int> a = vector<int>(size);
    for(size_t i = 0; i < size; ++i) {
        a[i] = i;
    }
    process(a);
}
