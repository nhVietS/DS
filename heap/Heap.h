/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Heap.h
 * Author: LTSACH
 *
 * Created on 22 August 2020, 18:18
 */

#ifndef HEAP_H
#define HEAP_H
//#include <stdlib.h>

#include "heap/IHeap.h"
#include <sstream>

template<class T>
class Heap: public IHeap<T>{
public:
    class Iterator;
    
protected:
    T *elements;
    int capacity;
    int count;
    int (*comparator)(T& lhs, T& rhs);
    void (*removeData)(Heap<T>* pHeap);
    string (*itemstr)(T&);
    
public:
    Heap(
            int (*comparator)(T& , T&)=0, 
            void (*removeData)(Heap<T>*)=0, 
            string (*itemstr)(T&)=0);
    ~Heap();
    void push(T item);
    T pop();
    T peek();
    void remove(T item);
    bool contains(T item);
    int size();
    void heapify(T array[], int size);
    void clear();
    bool empty();
    string toString();
    
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }
    
public:
    /* if T is pointer type:
     *     pass the address of method "free" to Heap<T>'s constructor:
     *     to:  remove the user's data (if needed)
     * Example:
     *  Heap<Point*> heap(&Heap<Point*>::free);
     *  => Destructor will call free via function pointer "removeData"
     */
    static void free(Heap<T> *pHeap){
        for(int idx=0; idx < pHeap->count; idx++) delete pHeap->elements[idx];
    }
    
    
private:
    bool aLTb(T& a, T& b){
        return compare(a, b) < 0;
    }
    int compare(T& a, T& b){
        if(comparator != 0) return comparator(a, b);
        else{
            if (a < b) return -1;
            else if(a > b) return 1;
            else return 0;
        }
    }
    
    void ensureCapacity(int minCapacity); 
    void swap(int a, int b);
    void reheapUp(int position);
    void reheapDown(int position);
    int getItem(T item);
    
    
//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
    
public:
    
    //Iterator: BEGIN
    class Iterator{
    private:
        Heap<T>* heap;
        int cursor;
    public:
        Iterator(Heap<T>* heap=0, bool begin=0){
            this->heap = heap;
            if(begin && (heap !=0)) cursor = 0;
            if(!begin && (heap !=0)) cursor = heap->size();
        }
        Iterator& operator=(const Iterator& iterator){
            this->heap = iterator.heap;
            this->cursor = iterator.cursor;
            return *this;
        }
        
        T& operator*(){
            return this->heap->elements[cursor];
        }
        bool operator!=(const Iterator& iterator){
            return this->cursor != iterator.cursor;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            cursor++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
        void remove(){
            this->heap->remove(this->heap->elements[cursor]);
        }
    };
    //Iterator: END
};


//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
Heap<T>::Heap(
        int (*comparator)(T&, T&), 
        void (*removeData)(Heap<T>* ), 
        string (*itemstr)(T&)){
    capacity = 10;
    count = 0;
    elements = new T[capacity];
    this->comparator = comparator;
    this->removeData = removeData;
    this->itemstr = itemstr;
}

template<class T>
Heap<T>::~Heap(){
    if(this->removeData != 0) removeData(this); //clear users's data if they want
    delete []elements;
}

template<class T>
void Heap<T>::push(T item){
    ensureCapacity(count + 1);
    elements[count] = item;
    count += 1;
    reheapUp(count - 1);
}

template<class T>
T Heap<T>::pop(){
    T item = elements[0];
    elements[0] = elements[count - 1];
    count -= 1;
    reheapDown(0);
    return item;
}

template<class T>
T Heap<T>::peek(){
    if(count == 0) 
        throw std::underflow_error("Calling to peek with the empty heap.");
    return elements[0];
}

template<class T>
void Heap<T>::remove(T item){
    int foundIdx = this->getItem(item);
    if(foundIdx == -1) return;

    //Remove item
    int copyCount = (count - 1) - (foundIdx + 1) + 1;
    memcpy(&elements[foundIdx], &elements[foundIdx + 1], copyCount*sizeof(T));
    count -= 1;

    //Determine valid heap [0-> (new size -1)]
    int startOldData = foundIdx;
    int lastOldData = count - 1;
    count = foundIdx;

    //reheapify from startOldData
    for(int idx=startOldData; idx <= lastOldData; idx++)
        push(elements[idx]);
}

template<class T>
bool Heap<T>::contains(T item){
    bool found = false;
    for(int idx=0; idx < count; idx++){
        if(compare(elements[idx], item) == 0){
            found = true;
            break;
        }
    }
    return found;
}

template<class T>
int Heap<T>::size(){
    return count;
}

template<class T>
void Heap<T>::heapify(T array[], int size){
    for(int idx=0; idx < size; idx++) push(array[idx]);
}

template<class T>
void Heap<T>::clear(){
    if(removeData != 0) removeData(this);
    delete []elements;
    capacity = 10;
    count = 0;
    elements = new T[capacity];
}

template<class T>
bool Heap<T>::empty(){
    return count == 0;
}

template<class T>
string Heap<T>::toString(){
    stringstream os;
    if(itemstr != 0){
        os << "[";
        for(int idx=0; idx < count -1; idx++)
            os << itemstr(elements[idx]) << ",";
        if(count > 0) os << itemstr(elements[count - 1]);
        os << "]";
    }
    else{
        os << "[";
        for(int idx=0; idx < count -1; idx++)
            os << elements[idx] << ",";
        if(count > 0) os << elements[count - 1];
        os << "]";
    }
    return os.str();
}


//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
void Heap<T>::ensureCapacity(int minCapacity){
    if(minCapacity >= capacity){
        //re-allocate 
        int old_capacity = capacity;
        capacity = old_capacity + (old_capacity >> 2);
        try{
            T* new_data = new T[capacity];
            memcpy(new_data, elements, capacity*sizeof(T));
            delete []elements;
            elements = new_data;
        }
        catch(std::bad_alloc e){
            e.what();
        }
    }
}

template<class T>
void Heap<T>::swap(int a, int b){
    T temp = this->elements[a];
    this->elements[a] = this->elements[b];
    this->elements[b] = temp;
}

template<class T>
void Heap<T>::reheapUp(int position){
    if(position <= 0) return;
    int parent = (position-1)/2;
    if(aLTb(this->elements[position], this->elements[parent])){
        this->swap(position, parent);
        reheapUp(parent);
    }
}

template<class T>
void Heap<T>::reheapDown(int position){
    int leftChild = position*2 + 1;
    int rightChild = position*2 + 2;
    int lastPosition = this->count - 1;

    if(leftChild <= lastPosition){
        int smallChild = leftChild; //by default => leftChild valid but rightChild invalid
        if(rightChild <= lastPosition){
            if(aLTb(this->elements[leftChild], this->elements[rightChild])) 
                smallChild = leftChild;
            else smallChild = rightChild;
        }

        if(aLTb(this->elements[smallChild], this->elements[position])){
            this->swap(smallChild, position);
            reheapDown(smallChild);
        }
    }
}

template<class T>
int Heap<T>::getItem(T item){
    int foundIdx = -1;
    for(int idx=0; idx < this->count; idx++){
        if(compare(elements[idx], item) == 0){
            foundIdx = idx;
            break;
        }
    }
    return foundIdx;
}

#endif /* HEAP_H */

