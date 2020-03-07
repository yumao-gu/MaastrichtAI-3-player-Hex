//
// Created by lenovo on 2020/3/6.
//

#ifndef HEX_PLAYER_H
#define HEX_PLAYER_H

#include <iostream>
#include "./HEX.h"
#include <math.h>
#include "./BOARD.h"

class PLAYER
{
public:
    UNIT _player;
    LOC _side;

    PLAYER(UNIT player,LOC side):_player(player),_side(side){}

    void Move(Hex hex,Board &board)
    {
        unordered_set<Hex,Hash_Hex<Hex>>::iterator it;
        it = board._map.find(hex);

        assert(it != board._map.end());

        board._map.erase(it);
        hex._unit = this->_player;
        board._map.insert(hex);
    }

    bool IsNotBlocked(Hex hex)
    {
        return hex._unit == empty or hex._unit == this->_player;
    }

    unordered_set<Hex,Hash_Hex<Hex>> Reachable(Hex start,int movement,Board board,bool verbose = true)
    {
        unordered_set<Hex,Hash_Hex<Hex>> visited;
        visited.insert(start);
        vector<unordered_set<Hex,Hash_Hex<Hex>>> fringes;
        fringes.push_back({start});

        for(int k = 1; k <= movement;k++)
        {
            if(fringes[k-1].empty())
            {
                break;
            }
            fringes.push_back({});
            if(verbose)
            {
                cout <<'\n' << k << "th layout:" << '\t';
            }
            for(unordered_set<Hex,Hash_Hex<Hex>>::iterator it = fringes[k-1].begin();it != fringes[k-1].end();it++)
            {
                for(int j = 0; j < 6; j++)
                {
                    Hex neighbor = hex_neighbor(*it , j);
                    unordered_set<Hex,Hash_Hex<Hex>>::iterator board_it;
                    board_it = board._map.find(neighbor);
                    if(board_it == board._map.end())
                    {
                        continue;
                    }
                    else if(!visited.count(neighbor) && this->IsNotBlocked(*board_it))
                    {
                        visited.insert(neighbor);
                        fringes[k].insert(neighbor);
                        if(verbose)
                        {
                            cout << neighbor << '\t';
                        }
                    }
                }
            }
        }
        return visited;
    }

    bool CanWin(Board board,bool verbose = false)
    {
        unordered_set<Hex,Hash_Hex<Hex>>::iterator it1;
        for(it1 = board._map.begin();it1 != board._map.end(); it1++ )
        {
            if((*it1).v[_side] == -board._radius && this->IsNotBlocked(*it1))
            {
                break;
            }
        }
        if(it1 == board._map.end())
        {
            return false;
        }

        unordered_set<Hex,Hash_Hex<Hex>>::iterator it2;
        for(it2 = board._map.begin();it2 != board._map.end(); it2++ )
        {
            if((*it2).v[_side] == board._radius && this->IsNotBlocked(*it2))
            {
                break;
            }
        }
        if(it2 == board._map.end())
        {
            return false;
        }

        unordered_set<Hex,Hash_Hex<Hex>> canreach = this->Reachable((*it1),20,board,verbose);
        return canreach.count((*it2));
    }
};

#endif //HEX_PLAYER_H
