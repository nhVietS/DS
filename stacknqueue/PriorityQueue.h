/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PriorityQueue.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 10:06
 */

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "heap/Heap.h"

template<class T>
class PriorityQueue: public Heap<T>{
public:
    class Iterator;
    
    PriorityQueue(
            int (*comparator)(T& , T&)=0, 
            void (*removeData)(Heap<T>*)=0, 
            string (*itemstr)(T&)=0):
        Heap<T>(comparator, removeData, itemstr){
    }
};

#endif /* PRIORITYQUEUE_H */

