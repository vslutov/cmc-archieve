template<class T>
void
myreverse(T begin, T end)
{
    if (begin == end) {
        return;
    }
    -- end;
    while (begin < end) {
        auto t = *begin;
        *begin = *end;
        *end = t;

        ++begin;
        --end;
    }
}
