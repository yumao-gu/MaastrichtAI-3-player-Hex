//
// Created by lenovo on 2020/3/6.
//

#ifndef HEX_HEX_H
#define HEX_HEX_H

#include "assert.h"
#include <vector>
#include <iostream>

using namespace std;

enum UNIT{A,B,C,empty};
enum LOC{A_side,B_side,C_side};

template<typename Number>
struct _Hex{ // Both storage types, both constructors
    union {
        const Number v[3];
        struct { const Number q, r, s; };
    };
    UNIT _unit = empty;

    _Hex(Number q_, Number r_): v{q_, r_, -q_ - r_} {}
    _Hex(Number q_, Number r_, Number s_,UNIT unit = empty): v{q_, r_, s_},_unit(unit)
    {
        assert( q_ + r_ + s_ == 0);
    }
};

typedef _Hex<int> Hex;

bool operator == (Hex a, Hex b) {
    return a.q == b.q && a.r == b.r && a.s == b.s;
}

ostream & operator<<(ostream &out, const Hex &A){
    out << '\t' << A.q << '\t' << A.r << '\t' << A.s << "\t unit: \t" << A._unit;
    return out;
}

bool operator != (Hex a, Hex b) {
    return !(a == b);
}

Hex hex_add(Hex a, Hex b) {
    return Hex(a.q + b.q, a.r + b.r, a.s + b.s);
}

Hex hex_subtract(Hex a, Hex b) {
    return Hex(a.q - b.q, a.r - b.r, a.s - b.s);
}

Hex hex_multiply(Hex a, int k) {
    return Hex(a.q * k, a.r * k, a.s * k);
}

int hex_length(Hex hex) {
    return int((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
}

int hex_distance(Hex a, Hex b) {
    return hex_length(hex_subtract(a, b));
}

const vector<Hex> hex_directions = {
        Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1),
        Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)
};

Hex hex_direction(int direction /* 0 to 5 */) {
    assert (0 <= direction && direction < 6);
    return hex_directions[direction];
}

Hex hex_neighbor(Hex hex, int direction) {
    return hex_add(hex, hex_direction(direction));
}

#endif //HEX_HEX_H
