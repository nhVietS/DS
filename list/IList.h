/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IList.h
 * Author: LTSACH
 *
 * Created on 13 August 2020, 13:54
 */
//http://www.cplusplus.com/doc/

//https://www.geeksforgeeks.org/implementing-iterator-pattern-of-a-single-linked-list/
//https://stackoverflow.com/questions/9155109/expected-after-expression

#ifndef ILIST_H
#define ILIST_H
#include <string>
using namespace std;

template<class T>
class IComparator{
public:
    int compare(T lhs, T rhs) = 0;
};

template<class T>
class IList{
public:
    virtual void    add(T e)=0;
    virtual void    add(int index, T e)=0;
    virtual T       removeAt(int index)=0;
    virtual bool    removeItem(T item, void (*removeItemData)(T)=0)=0;
    virtual bool    empty()=0;
    virtual int     size()=0;
    virtual void    clear()=0;
    virtual T       get(int index)=0;
    virtual int     indexOf(T item)=0;
    virtual bool    contains(T item)=0;
    virtual string  toString()=0;
};
#endif /* ILIST_H */

