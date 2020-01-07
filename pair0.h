#ifndef PAIR_H
#define PAIR_H

template <class T1, class T2>
class pair0
{
public:
    const T1* first;
    const T2* second;
    pair0(T1* first, T2* second);
};

#endif // PAIR_H
