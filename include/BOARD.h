//
// Created by lenovo on 2020/3/6.
//

#ifndef HEX_BOARD_H
#define HEX_BOARD_H

#include "./HEX.h"
#include <math.h>
#include <set>
#include <unordered_set>
#include <iterator>

using namespace std;

template <typename Hex>
struct Hash_Hex{
    size_t operator()(const Hex& h) const
    {
        hash<int> int_hash;
        size_t hq = int_hash(h.q);
        size_t hr = int_hash(h.r);
        return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
};

class Board
{
public:
    int _radius = 3;
    unordered_set<Hex,Hash_Hex<Hex>> _map;

    Board(int radius):_radius(radius)
    {
        for(auto q = -_radius; q <= _radius; q++)
        {
            int r1 = max(-_radius, -q - _radius);
            int r2 = min(_radius, -q + _radius);
            for (int r = r1; r <= r2; r++)
            {
                Hex hex(q,r,-q-r);
                _map.insert(hex);
            }
        }
    }

    void Print()
    {
        cout << "Board : " << endl;
        for(unordered_set<Hex,Hash_Hex<Hex>>::iterator it = _map.begin();it != _map.end();it++)
        {
            cout << (*it) << endl;
        }
        cout << "END Board!" << endl;
    }

};


#endif //HEX_BOARD_H
