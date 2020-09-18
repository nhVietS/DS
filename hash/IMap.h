/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IMap.h
 * Author: LTSACH
 *
 * Created on 22 August 2020, 21:53
 */

#ifndef IMAP_H
#define IMAP_H

#include <string>
using namespace std;

template<class K, class V>
class IMap {
public:
    /*
    put(K key, V value): 
    if key is not in the map: 
        + add a mapping key->value to the map
        + return null
    else:
        + associate key with the new value 
        + return the old value
    */
    virtual V put(K key, V value)=0;
    /*
    get(K key):
    if key in the map: return the associated value
    else: return null
    */
    virtual V get(K key)=0;
    /*
    remove(K key):
    if key is in the map: remove it from the map, and return its value
    else: return null
    */
    virtual V remove(K key)=0;
    /*
    remove(K key, V value):
    if maping key->value in the map: remove it and return true
    else: return false
    */
    virtual bool remove(K key, V value)=0;
    /*
    containsKey(K key):
    if key is in the map: return true
    else: return false
    */
    virtual bool containsKey(K key)=0;
    /*
    containsKey(V value):
    if value is in the map: return true
    else: return false
    */
    virtual bool containsValue(V value)=0;
    
    /*
    isEmpty():
    return true if the map is empty
    else: return false
    */
    virtual bool empty()=0;
    /*
    size():
    return num of pairs key->value
    */
    virtual int size()=0;
    
    virtual void clear() = 0;
    
    virtual string toString()=0;
};


#endif /* IMAP_H */

