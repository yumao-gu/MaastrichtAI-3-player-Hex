//
// Created by lenovo on 2020/3/6.
//

#ifndef HEX_GRAPH_H
#define HEX_GRAPH_H

#include "./HEX.h"
#include <math.h>
#include "./BOARD.h"
#include "./PLAYER.h"
#include <list>
#include <algorithm>
#include <sstream>

using namespace std;

struct Node;   //定义一个结构体原形
struct Line;    //定义一个结构体原形
class Graph;        //定义一个类原形
class Iterator;    //定义一个类原形
typedef list<Line*> Line_List; //重命名一个节点链表
typedef list<Node*> Node_List;

struct Node {
    UNIT _player;
    Line* _parent;
    Line_List _children;

    Node(UNIT player, Line* Parent)
    {
        _player = player;
        _parent = Parent;
    }

    void SetParent(Line* node)
    {
        _parent = node;
    }

    void InsertChildren(Line* node)
    {
        Line* p = node;
        _children.push_back(p);
    }
};

struct Line {
    UNIT _player;
    int _q,_r,_s;;
    Node* _parent;
    Node* _child;;

    Line(UNIT player, Node* Parent, Hex hex)
    {
        _player = player;
        _parent = Parent;
        _q = hex.q;
        _r = hex.r;
        _s = hex.s;
    }

    void SetParent(Node* node)
    {
        _parent = node;
    }

    void SetChild(Node* node)
    {
        _child = node;
    }
};

void clone(Node* node, Node_List& nodes, Line* cpparent)
{
    Node* cp = new Node(node->_player, cpparent);
    nodes.push_back(cp);
    Line_List& l = node->_children;
    Line_List& cl = cp->_children;

    for (Line_List::iterator lt = l.begin(); lt != l.end(); lt++)
    {
        Hex cpl_hex = Hex((*lt)->_q,(*lt)->_r,(*lt)->_s);
        Line* cpl = new Line(node->_player,cp,cpl_hex);
        clone((*lt)->_child,nodes,cpl);
        cl.push_back(cpl);
    }
}

class Graph{
public:
    friend class Iterator;

    static bool IsRoot(Iterator);         //判断是否是根
    static bool isLeaf(Iterator);         //判断是否是叶子
    static Iterator Parent(Iterator);     //返回其父节点
    static int NumChildren(Iterator);     //返回其子节点数目
    int level(Node*,Iterator);

    //跌代器函数
    Iterator begin();                //Tree Begin
    Iterator end();                  //Tree End

    Graph()
    {
        _nodes.push_back(new Node(A,NULL));
    }

    ~Graph()
    {
        for (list<Node*>::iterator it = _nodes.begin(); it != _nodes.end(); it++)
        {
            delete* it;
        }
    }

    Graph(const Graph& t)
    {
        if (t._nodes.empty())
            return;
//        cout<<"t size : " << t.Size()<<endl;
//        cout<<"t children size : " << t._nodes.front()->_children.size()<<endl;
//        cout<<"t._nodes.front()->_children hex: " << t._nodes.front()->_children.front()->_q<<'\t'
//            << t._nodes.front()->_children.front()->_r<<'\t'<< t._nodes.front()->_children.front()->_s<<'\t'<<endl;
        clone(t._nodes.front(), _nodes, 0);
    }

    void Clear()
    {
        for (list<Node*>::iterator it = _nodes.begin(); it != _nodes.end(); it++)
        {
            delete* it;
        }
        _nodes.clear();
    }

    bool IsEmpty()const
    {
        return _nodes.empty();
    }

    int Size()const
    {
        return (int)_nodes.size();
    }

    int Leaves()
    {
        int i = 0;
        list<Node*>::iterator it = _nodes.begin();
        while (it != _nodes.end())
        {
            if ((*it)->_children.size() == 0)
            {
                i++;
            }
            it++;
        }
        return i;
    }

    int Root() const
    {
        return (*_nodes.begin())->_player;
    }

    int height(Node* node)
    {
        if (!node)
        {
            return -1;
        }
        else
        {
            list<Line*> plist = node->_children;
            if (plist.size() == 0)
            {
                return 0;
            }
            int hA = 0;
            for (list<Line*>::iterator it = plist.begin(); it != plist.end(); it++)
            {
                int hB = height((*it)->_child);
                if (hB > hA)
                {
                    hA = hB;
                }
            }
            return hA + 1;
        }
    }

    int Height()
    {
        if (_nodes.size() != 0)
        {
            Node* GNode = _nodes.front();
            return height(GNode);
        }
        else
        {
            return -1; //判断为空树
        }
    }

    Graph& operator=(const Graph & t)
    {
        Clear();
        Graph* p = new Graph(t);
        _nodes = p->_nodes;
        return *this;
    }

    void AddNode(UNIT p, Node *node,Hex hex,UNIT c)
    {
        Line* l = new Line(p,node,hex);
        Node* n = new Node(c,l);
        l->SetChild(n);
        node->InsertChildren(l);
        _nodes.push_back(n);
    }

    void show(Iterator it,int show_height);

    void Show(Iterator it);

    void GraphShowClear()
    {
        graph_show.clear();
    }

    void GraphShowResize(int size)
    {
        GraphShowClear();
        graph_show.resize(size);
    }

private:
    list<Node*> _nodes;            //节点数组
    list<Node*>::iterator LIt;     //一个节点迭代器
    vector<string> graph_show;
};
//、
class Iterator{
public:

    friend class Graph;

    Iterator() {};

    Iterator(const Iterator& it)
    {
        _graph = it._graph;
        _lit = it._lit;
    }

    Iterator(Graph* t, Node* n)
    {
        _graph = t;
        list<Node*>& nodes = _graph->_nodes;
        _lit = find(nodes.begin(), nodes.end(), n);
    }

    Iterator(Graph * t, list<Node*>::iterator lt)
    {
        _graph = t;
        _lit = lt;
    }

    void operator =(const Iterator& it)
    {
        _graph = it._graph;
        _lit = it._lit;
    }

    bool operator ==(const Iterator & it)
    {
        return _lit == it._lit;
    }

    bool operator !=(const Iterator & it)
    {
        return _lit != it._lit;
    }

    Iterator& operator ++()
    {
        ++_lit;
        return *this;
    }

    Iterator operator ++(int)
    {
        Iterator it(*this);
        ++_lit;
        return it;
    }

    Node* operator *() const
    {
        return (*_lit);
    }

    bool operator !()
    {
        return _lit == _graph->_nodes.end();
    }

private:
    Graph* _graph;                        //Tree data
    list<Node*>::iterator _lit;     //List Iterator
};

Iterator Graph::begin()
{
    return Iterator(this, _nodes.begin());
}

Iterator Graph::end()
{
    return Iterator(this, _nodes.end());
}

bool Graph::IsRoot(Iterator it)
{
    Node* p = *it;
    if (p->_parent == 0)
    {
        return true;
    }
    return false;
}

bool Graph::isLeaf(Iterator it)
{
    Node* p = *it;
    if (p->_children.size() == 0)
    {
        return true;
    }
    return false;
}

Iterator Graph::Parent(Iterator it)
{
    Node* p = *it;
    Graph* t = it._graph;
    Iterator Ite(t, p->_parent->_parent);
    return Ite;
}

int Graph::NumChildren(Iterator it)
{
    Node* p = *it;
    return (int)p->_children.size();
}

void Graph::show(Iterator it,int show_height)
{
    if(IsRoot(it))
    {
        assert(show_height == 0);
        graph_show[show_height] += "Game Begin\n";
    }
    else
    {
        stringstream info;
        info << "\t{player:\t" << to_string((*it)->_player) << "\tparent:\t" << to_string((*it)->_parent->_player)
             <<"\thex:\t"<<to_string((*it)->_parent->_q)<<'\t'<<to_string((*it)->_parent->_r)
             <<'\t'<<to_string((*it)->_parent->_s)<<"}\t";
        graph_show[show_height] += info.str();
    }
    for(auto cit = (*it)->_children.begin(); cit != (*it)->_children.end(); cit++)
    {
        Iterator Cit = Iterator(this, (*cit)->_child);
        show(Cit,show_height+1);
    }
}

void Graph::Show(Iterator it)
{
    int height = this->height((*it));
    this->GraphShowResize(height);
    this->show(it,0);
}

#endif //HEX_GRAPH_H
