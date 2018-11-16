template<class T>
typename T::value_type process(T v)
{
    auto sum = typename T::value_type();
    int count = 0;
    for (auto x = v.rbegin(); x != v.rend(); ++x) {
        if (count <= 2) {
            sum += *x;
        } else {
            break;
        }
        ++ count;
    }

    return sum;
}
