/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DLinkedList.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 21:21
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;


template<class T>
class DLinkedList: IList<T> {
public:
    class Node;
    class Iterator;
    
protected:
    Node *head;
    Node *tail;
    int count;
    bool (*comparator)(T& lhs, T& rhs);
    void (*removeData)(DLinkedList<T>*);
    string (*item2str)(T&);
    
public:
    DLinkedList(
            void (*removeData)(DLinkedList<T>*)=0, //function removing user data
            bool (*comparator)(T&, T&)=0, //function comparing two elements
            string (*item2str)(T&)=0); //function converting elements to strings
    virtual ~DLinkedList();
    virtual void    add(T e);
    virtual void    add(int index, T e);
    virtual T       removeAt(int index);
    virtual bool    removeItem(T item, void (*removeItemData)(T)=0);
    virtual bool    empty();
    virtual int     size();
    virtual void    clear();
    virtual T       get(int index);
    virtual int     indexOf(T item);
    virtual bool    contains(T item);
    virtual string  toString();
    virtual void    dump();
    
    DLinkedList(const DLinkedList<T>& list){
        this->count = 0;
        this->head = new Node();
        this->tail = new Node();
        this->head->next = this->tail; this->tail->next = 0;
        this->tail->prev = this->head; this->head->prev = 0;
        
        //copy
        this->removeData = list.removeData;
        this->comparator = list.comparator;
        this->item2str = list.item2str;
        
        Node* ptr= list.head->next;
        while(ptr != list.tail){
            this->add(ptr->data);
            ptr = ptr->next;
        }
    }
    void setItem2Str(string (*item2str)(T&)){
        this->item2str = item2str;
    }
    static void free(DLinkedList<T> *list){
        typename DLinkedList<T>::Iterator it = list->begin();
        while(it != list->end()){
            delete *it;
            it++;
        }
    }
    
    //public methods, simple implementation
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }
    
private:
    void checkIndex(int index){
        if((index < 0) || (index >= count))
            throw std::out_of_range("index is out of range");
        
    }
    static bool equals(T& lhs, T& rhs, bool (*comparator)(T&, T& )){
        if(comparator == 0) return lhs == rhs;
        else return comparator(lhs, rhs);
    }
    
//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
public:
    
    class Node{
    private:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;
    public:
        Node(){
            next= 0;
            prev= 0;
        }
        Node(Node* next, Node* prev){
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node* next, Node* prev){
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };
    
//////////////////////////////////////////////////////////////////////
    class Iterator{
    private:
        DLinkedList<T>* pList;
        Node* pNode;
        
    public:
        Iterator(DLinkedList<T>* pList=0, bool begin=true){
            if(begin){
                if(pList !=0) this->pNode = pList->head->next;
                else pNode = 0;
            }
            else{
                if(pList !=0) this->pNode = pList->tail;
                else pNode = 0;
            }
            this->pList = pList;
        }
        
        Iterator& operator=(const Iterator& iterator){
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T)=0){
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node* pNext = pNode->prev; //MUST prev, so iterator++ will go to end
            if(removeItemData != 0) removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }
        
        T& operator*(){
            return pNode->data;
        }
        bool operator!=(const Iterator& iterator){
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            pNode = pNode->next;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
    };
    

};
//////////////////////////////////////////////////////////////////////
//Define a shorter name for DLinkedList:

template<class T>
using List = DLinkedList<T>;



//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
DLinkedList<T>::DLinkedList(
        void (*removeData)(DLinkedList<T>*), 
        bool (*comparator)(T&, T&), 
        string (*item2str)(T&)) {
    head = new Node();
    tail = new Node();
    head->next = tail; tail->next = 0;
    tail->prev = head; head->prev = 0;
    count = 0;
    this->comparator = comparator;
    this->removeData = removeData;
    this->item2str = item2str;
}

template<class T>
DLinkedList<T>::~DLinkedList() {
    //Remove user's data
    if(removeData != 0) removeData(this);
    
    //Remove list's data (i.e. Node)
    if((head != 0) & (tail != 0)){
        Node* ptr = head->next;
        while(ptr != tail){
            Node* next = ptr->next;
            delete ptr;
            ptr = next;
        }
    }
    if(head != 0) delete head;
    if(tail != 0) delete tail;
}

template<class T>
void DLinkedList<T>::add(T e) {
    Node* node = new Node(e, tail, tail->prev);
    (tail->prev)->next = node;
    tail->prev = node;
    count += 1;
}
template<class T>
void DLinkedList<T>::add(int index, T e) {
    if((index < 0) || (index > count))
        throw std::out_of_range("The index is out of range!");
    //index in [0, count]
    Node* newNode = new Node(e, 0, 0);
    Node* prevNode = head;
    int cursor = -1;

    while(cursor < index - 1){ //cursor largest = 2; index = 3
        prevNode = prevNode->next;
        cursor += 1;
    }

    //
    Node* curNode = prevNode->next;
    //Add newNode
    newNode->next = curNode;
    newNode->prev = prevNode;
    prevNode->next = newNode;
    curNode->prev = newNode;

    //Change tail->next if needed
    if(index == count)
        tail->prev = newNode;
    count += 1;
}
template<class T>
T DLinkedList<T>::removeAt(int index){
    if((index < 0) || (index > count - 1))
        throw std::out_of_range("The index is out of range!");

    //from here: index valid
    Node* prevNode = head;
    int cursor = -1;

    while(cursor < index - 1){
        prevNode = prevNode->next;
        cursor += 1;
    }
    //here: prevNode points to previous item (at index - 1)
    Node* curNode = prevNode->next;
    prevNode->next = curNode->next;
    curNode->next->prev = prevNode;
    curNode->next = 0; curNode->prev = 0; //detach from list
    T data = curNode->data;
    delete curNode;
    
    if(index == count-1)
        tail->prev = prevNode;
    count -= 1;
    return data;
}


template<class T>
bool DLinkedList<T>::empty(){
    return count == 0;
}

template<class T>
int DLinkedList<T>::size(){
    return count;
}

template<class T>
void DLinkedList<T>::clear(){
    //Remove all nodes except head and tail
    Node* ptr = head->next;
    while(ptr != tail){
        //remove node pointed by ptr;
        //clients: must remove their data
        Node* next = ptr->next;
        delete ptr;
        ptr = next;
    }
    //put to the empty condition
    head->next = tail; tail->next = 0;
    tail->prev = head; head->prev = 0;
    this->count = 0;
}

template<class T>
T DLinkedList<T>::get(int index){
    if((index < 0) || (index > count - 1))
        throw std::out_of_range("The index is out of range!");
    //from here: index valid
    Node* prevNode = head;
    int cursor = -1;

    while(cursor < index - 1){ //cursor largest = 2; index = 3
        prevNode = prevNode->next;
        cursor += 1;
    }

    return prevNode->next->data;
}

template<class T>
int DLinkedList<T>::indexOf(T item){
    bool found = false;
    Node* pNode = head->next;
    int foundAt = 0;

    while(pNode != tail){
        found = DLinkedList<T>::equals(pNode->data, item, this->comparator);
        if(found) break;
        
        pNode = pNode->next;
        foundAt += 1;
    }
    if(found) return foundAt;
    else return -1;
}

template<class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T)){
    bool found = false;
    Node* pNode = head->next;

    while(pNode != tail){
        found = DLinkedList<T>::equals(pNode->data, item, this->comparator);
        if(found){
            //detach
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            
            //remove data
            if(removeItemData) removeItemData(pNode->data);
            delete pNode;
            this->count -= 1;
            return true;
        }
        
        pNode = pNode->next;
    }
    return false;
}


template<class T>
bool DLinkedList<T>::contains(T item){
    return indexOf(item) != -1;
}

template<class T>
string DLinkedList<T>::toString(){
    stringstream ss;
    ss << "[";
    Node* ptr = head->next;
    while(ptr != tail->prev){
        if(this->item2str != 0) ss << this->item2str(ptr->data) << ", ";
        else ss << ptr->data << ",";
        
        ptr = ptr->next;
    }
    
    if(count > 0){
        if(this->item2str != 0) ss << this->item2str(ptr->data) << "]";
        else ss << ptr->data << "]";
    }
    else 
        ss << "]";
    return ss.str();
}


template<class T>
void DLinkedList<T>::dump(){
    string line(50, '=');
    cout << line << endl;
    cout << "DLinkedList's information:" << endl;
    cout << "item's size: " << sizeof(T) << " (bytes)"<< endl;
    cout << "count: " << count << endl;
    cout << toString() << endl;
    cout << line << endl;
}



#endif /* DLINKEDLIST_H */

