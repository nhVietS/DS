/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IDeck.h
 * Author: LTSACH
 *
 * Created on 27 August 2020, 10:32
 */

#ifndef IDECK_H
#define IDECK_H

template<class T>
class IDeck{
public:
    virtual void push(T item)=0;
    virtual T pop()=0;
    virtual T peek()=0;
    virtual bool empty()=0;
    virtual int size()=0;
    virtual void clear()=0;
    virtual bool remove(T item)=0;
    virtual bool contains(T item)=0;
};

#endif /* IDECK_H */

