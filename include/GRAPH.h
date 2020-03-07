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

    list<Node*> _nodes;            //节点数组
    list<Node*>::iterator LIt;     //一个节点迭代器
    int height(Node*);
    int level(Node*,Iterator);
};

//This is TreeSub Class Iterator
//class Iterator{
//private:
//    Tree* _tree;                        //Tree data
//    list<TreeNode*>::iterator _lit;     //List Iterator
//public:
//    Iterator();                                 //默认构造函数
//    Iterator(const Iterator&);                  //复制构造函数
//    Iterator(Tree*,TreeNode*);                  //构造函数
//    Iterator(Tree*,list<TreeNode*>::iterator);  //构造函数
//    //运算符重载
//    void operator=(const Iterator&);            //赋值运算符重载
//    bool operator==(const Iterator&);           //关系运算符重载
//    bool operator!=(const Iterator&);           //关系运算符重载
//    Iterator& operator++();                     //前缀++运算符
//    Iterator operator++(int);                   //后缀++运算符
//    int operator*()const;                       //获得节点信息
//    bool operator!();                           //赋值运算符重载
//
//    typedef list<TreeNode*>::iterator List;
//    friend class Tree;
//};

#endif //HEX_GRAPH_H
