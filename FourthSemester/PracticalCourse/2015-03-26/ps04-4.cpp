template <class Iter, class Pred>
int
myfilter(Iter begin, Iter end, Pred pred, typename Iter::value_type val = typename Iter::value_type())
{
    int count = 0;
    while (begin < end) {
        if (pred(*begin)) {
            *begin = val;
            ++count;
        }
        ++begin;
    }
    return count;
}
