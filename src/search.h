//
// Author: Tuomas Aaltonen - tuomas.aaltonen@tuni.fi
//
// High-Level Synthesis Blue Book style
// Balanced recursive template search for P encoders
//

#ifndef SEARCH_H
#define SEARCH_H

#include "defs.h"

//HLS Blue-book style
template<int N> struct search_s
{
    static searchStruct_t recSearch(searchStruct_t *a)
    {
        searchStruct_t m0 = search_s<N/2>::recSearch(a);
        searchStruct_t m1 = search_s<N-N/2>::recSearch(a+N/2);

        searchStruct_t tmp;

        if(m0.f == 1)
            tmp = m0;
        else
            tmp = m1;

        return tmp;
    }
};

//Template specialization for termination
template<> struct search_s<1>
{
    static searchStruct_t recSearch(searchStruct_t *a)
    {
        return a[0];
    }
};

//Helper for partial specialization
template<int N> searchStruct_t recSearch(searchStruct_t *a)
{
    return search_s<N>::recSearch(a);
}

#endif
