/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IBST.h
 * Author: LTSACH
 *
 * Created on 20 August 2020, 15:05
 */

#ifndef IBST_H
#define IBST_H
#include <memory>
#include "list/DLinkedList.h"
/* 
 * K: KeyType: required supporting operator "less-than: <"; can be:
 *   + Primitive types: int, float, double
 *   + Class Type: Point, Student (must override operator:"<")
 * V: Value Type: must be a pointer type
 *    For example: int*, Point*
 */
template<class K, class V>
class IBST{
public:
    virtual void add(K key, V value=0)=0;
    virtual V remove(K key)=0;
    virtual V search(K key, bool& found)=0; //return NULL if not found
    virtual int size()=0;
    virtual int height()=0;
    virtual void clear()=0;
    virtual DLinkedList<V> ascendingList()=0;
    virtual DLinkedList<V> descendingList()=0;
};


#endif /* IBST_H */
