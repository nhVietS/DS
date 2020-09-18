/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Stack.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 22:01
 */

#ifndef STACK_H
#define STACK_H

#include <list>

#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"

template<class T>
class Stack: public IDeck<T>{
public:
    class Iterator; //forward declaration
    
protected:
    DLinkedList<T> list;
    void (*removeData)(DLinkedList<T>*);
    bool (*comparator)(T& lhs, T& rhs);
    
public:
    Stack(
            void (*removeData)(DLinkedList<T>*)=0, 
            bool (*comparator)(T&, T&)=0){
        this->comparator = comparator;
        this->removeData = removeData;
    }
    void push(T item){
       list.add(0, item); 
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
    Iterator top(){
        return Iterator(this, true);
    }
    Iterator bottom(){
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
        Stack<T>* stack;
        typename DLinkedList<T>::Iterator listIt;
    public:
        Iterator(Stack<T>* stack=0, bool begin=true){
            this->stack = stack;
            if(begin){
                if(stack != 0) this->listIt = stack->list.begin();
                else this->listIt = 0;
            }
            else{
                if(stack != 0) this->listIt = stack->list.end();
                else this->listIt = 0;
            }
        }
        Iterator& operator=(const Iterator& iterator ){
            this->stack = iterator.stack;
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


#endif /* STACK_H */

