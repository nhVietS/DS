/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Queue.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 22:33
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"

template<class T>
class Queue: public IDeck<T>{
public:
    class Iterator; //forward declaration
    
protected:
    DLinkedList<T> list;
    void (*removeData)(DLinkedList<T>*);
    bool (*comparator)(T& lhs, T& rhs);
    
public:
    Queue(
            void (*removeData)(DLinkedList<T>*)=0, 
            bool (*comparator)(T&, T&)=0){
        this->comparator = comparator;
        this->removeData = removeData;
    }
    void push(T item){
       list.add(item); 
    }
    T pop(){
       return list.removeAt(0);
    }
    T peek(){
        return list.get(0);
    }
    bool empty(){
        return list.empty();
    }
    int size(){
        return list.size();
    }
    void clear(){
        list.clear();
    }
    bool remove(T item){
        return list.removeItem(item);
    }
    bool contains(T item){
        bool found = false;
        typename DLinkedList<T>::Iterator it = list.begin();
        while(it != list.end()){
            found = equals(*it, item, this->comparator);
            if(found) break;
            it++;
        }
        return found;
    }
    
    Iterator front(){
        return Iterator(this, true);
    }
    Iterator rear(){
        return Iterator(this, false);
    }
    
private:
    static bool equals(T& lhs, T& rhs, bool (*comparator)(T&, T& )){
        if(comparator == 0) return lhs == rhs;
        else return comparator(lhs, rhs);
    }
    
//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////

public:
//Iterator: BEGIN
    class Iterator{
    private:
        Queue<T>* queue;
        typename DLinkedList<T>::Iterator listIt;
    public:
        Iterator(Queue<T>* queue=0, bool begin=true){
            this->queue = queue;
            if(begin){
                if(queue != 0) this->listIt = queue->list.begin();
                else this->listIt = 0;
            }
            else{
                if(queue != 0) this->listIt = queue->list.end();
                else this->listIt = 0;
            }
        }
        Iterator& operator=(const Iterator& iterator ){
            this->queue = iterator.queue;
            this->listIt = iterator.listIt;
            return *this;
        }
        
        T& operator*(){
            return *(this->listIt);
        }
        bool operator!=(const Iterator& iterator){
            return this->listIt != iterator.listIt;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            listIt++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
        void remove(void (*removeItem)(T)=0){
            listIt.remove(removeItem);
        }
    };
    //Iterator: END
};


#endif /* QUEUE_H */

