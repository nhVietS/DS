/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   XHashMapDemo.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 10:56
 */

#ifndef XHASHMAPDEMO_H
#define XHASHMAPDEMO_H
#include "hash/XHashMap.h"
#include "util/Point.h"

void hashDemo1(){
    int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
    int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
    XHashMap<int, int> hash(&XHashMap<int, int>::simpleHash);
    for(int idx=0; idx < 8; idx++){
        hash.put(keys[idx], values[idx]);
    }
    hash.println();
}

int hashFunc(int* item, int size){
    return *item%size;
}
bool keyEQ(int* lhs, int* rhs){
    return *lhs == *rhs;
}
string key2str(int* item){
    stringstream os;
    os << *item;
    return os.str();
}
bool valueEQ(int* lhs, int* rhs){
    return *lhs == *rhs;
}
string value2str(int* item){
    stringstream os;
    os << *item;
    return os.str();
}

void hashDemo2(){
    int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
    int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
    
    XHashMap<int, int*> hashMap(
            &XHashMap<int, int>::simpleHash,
            0.75, 
            &valueEQ,
            &value2str,
            &XHashMap<int, int*>::freeValue,
            0,
            0,
            0
            );
    for(int idx=0; idx < 8; idx++){
        hashMap.put(keys[idx], new int(values[idx]));
    }
    hashMap.println();
}

void hashDemo3(){
    int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
    int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
    
    XHashMap<int*, int*> hashMap(
            &hashFunc,
            0.75, 
            &valueEQ,
            &value2str,
            &XHashMap<int*, int*>::freeValue,
            &keyEQ,
            &key2str,
            &XHashMap<int*, int*>::freeKey
            );
    for(int idx=0; idx < 8; idx++){
        hashMap.put(new int(keys[idx]), new int(values[idx]));
    }
    hashMap.println();
}

bool pointEQ(Point* p1, Point* p2){
    return (p1->getX() == p2->getX()) && 
           (p1->getY() == p2->getY());
}
string point2str(Point* p){
    stringstream os;
    os      << "(" 
            << p->getX()
            << ","
            << p->getY()
            << ")";
    return os.str();
}

void hashDemo4(){
    int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
    Point* values[]    = {
                    new Point(2.3, 4.5), new Point(4.2, 6.7),
                    new Point(5.2, 7.2), new Point(9.1, 1.8),
                    new Point(8.9, 9.1), new Point(8.1, 3.1),
                    new Point(3.3, 5.4), new Point(7.3, 4.2) };
    
    XHashMap<int*, Point*> hashMap(
            &hashFunc,
            0.75, 
            &pointEQ,
            &point2str,
            &XHashMap<int*, Point*>::freeValue,
            &keyEQ,
            &key2str,
            &XHashMap<int*, Point*>::freeKey
            );
    for(int idx=0; idx < 8; idx++){
        hashMap.put(new int(keys[idx]), values[idx]);
    }
    hashMap.println();
}


#endif /* XHASHMAPDEMO_H */

