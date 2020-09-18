/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   XArrayList.h
 * Author: LTSACH
 *
 * Created on 13 August 2020, 14:09
 */

#ifndef XARRAYLIST_H
#define XARRAYLIST_H
#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template<class T>
class XArrayList: IList<T> {
public:
    class Iterator; //forward declaration
    
protected:
    T* data;
    int capacity;
    int count;
    bool (*comparator)(T& lhs, T& rhs);
    void (*removeData)(XArrayList<T>*);
    
public:
    XArrayList(
            void (*removeData)(XArrayList<T>*)=0, 
            bool (*comparator)(T&, T&)=0);
    virtual ~XArrayList();
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
    virtual Iterator begin();
    virtual Iterator end();

private:
    void checkIndex(int index); //check validity of index for accessing
    void ensureCapacity(int index); //auto-allocate if needed
    /* if T: primitive type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     * if T: object type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     *      Therefore, class of type T MUST override operator ==
     * if T: pointer type:
     *      indexOf, contains: will use function pointer "comparator"
     *      to: compare two items of T type
     *      Therefore:
     *      (1): must pass comparator to the constructor of XArrayList
     *      (2): must define a method for comparing 
     *           the content pointed by two pointers of type T
     *          See: definition of "equals" of class Point for more detail
     */
    static bool equals(T& lhs, T& rhs, bool (*comparator)(T&, T& )){
        if(comparator == 0) return lhs == rhs;
        else return comparator(lhs, rhs);
    }
    
public:    
    /* if T is pointer type:
     *     pass THE address of method "free" to XArrayList<T>'s constructor:
     *     to:  remove the user's data (if needed)
     * Example:
     *  XArrayList<Point*> list(&XArrayList<Point*>::free);
     *  => Destructor will call free via function pointer "removeData"
     */
    static void free(XArrayList<T> *list){
        typename XArrayList<T>::Iterator it = list->begin();
        while(it != list->end()){
            delete *it;
            it++;
        }
    }
 
    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
    
    //Iterator: BEGIN
    class Iterator{
    private:
        int cursor;
        XArrayList<T>* pList;
    public:
        Iterator(XArrayList<T>* pList=0, int index=0){
            this->pList = pList;
            this->cursor = index;
        }
        Iterator& operator=(const Iterator& iterator){
            cursor = iterator.cursor;
            pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T)=0){
            T item = pList->removeAt(cursor);
            if(removeItemData != 0) removeItemData(item);
            cursor -= 1; //MUST keep index of previous, for ++ later
        }
        
        T& operator*(){
            return pList->data[cursor];
        }
        bool operator!=(const Iterator& iterator){
            return cursor != iterator.cursor;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            this->cursor++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
    };
    //Iterator: END
};


//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
XArrayList<T>::XArrayList(
        void (*removeData)(XArrayList<T>*), 
        bool (*comparator)(T&, T&)) {
    capacity = 10; //initial size = 10; auto-inc if needed
    data = new T[capacity];
    count = 0;
    this->comparator = comparator;
    this->removeData = removeData;
}

template<class T>
XArrayList<T>::~XArrayList() {
    if(removeData != 0) removeData(this);
    delete []data;
}

template<class T>
typename XArrayList<T>::Iterator XArrayList<T>::begin(){
    return XArrayList<T>::Iterator(this);
}

template<class T>
typename XArrayList<T>::Iterator XArrayList<T>::end(){
    return XArrayList<T>::Iterator(this, count);
}

template<class T>
void XArrayList<T>::add(T e) {
    ensureCapacity(count);
    
    data[count] = e;
    count += 1;
}
template<class T>
void XArrayList<T>::add(int index, T e) {
    ensureCapacity(index);

    for(int idx=count-1; idx >= index; idx-- ){
        data[idx + 1] = data[idx];
    }
    data[index] = e;
    count += 1;
}

template<class T>
T XArrayList<T>::removeAt(int index){
    checkIndex(index);
    T backup = this->data[index];
    //shift data to the left
    for(int idx=index; idx < count-1; idx++){
        this->data[idx] = this->data[idx+1];
    }
    this->count -= 1;
    return backup;
}

template<class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T)){
    bool found = false;
    int cursor = 0;
    while(cursor != count){
        found = XArrayList<T>::equals(this->data[cursor], item, this->comparator);
        if(found){
            //remove data at data[cursor]
            if(removeItemData) removeItemData(this->data[cursor]);
            //shift data
            for(int idx=cursor; idx < count-1; idx++){
                this->data[idx] = this->data[idx+1];
            }
            this->count -= 1;
            return true;
        }
        cursor++;
    }
    return false;
}

template<class T>
bool XArrayList<T>::empty(){
    return count == 0;
}

template<class T>
int XArrayList<T>::size(){
    return count;
}

template<class T>
void XArrayList<T>::clear(){
    if(removeData != 0) removeData(this);
    //delete the dynamic array
    delete []data;
    
    //reset
    capacity = 10; //initial size = 10; auto-inc if needed
    data = new T[capacity];
    count = 0;
}

template<class T>
T XArrayList<T>::get(int index){
    checkIndex(index);
    return data[index];
}

template<class T>
int XArrayList<T>::indexOf(T item){
    bool found = false;
    int foundAt = -1;
    for(int idx=0; idx < count; idx++){
        found = XArrayList<T>::equals(data[idx], item, this->comparator);
        if(found){
            foundAt = idx;
            break;
        }
    }
    return foundAt;
}
template<class T>
bool XArrayList<T>::contains(T item){
    return indexOf(item) != -1;
}

template<class T>
string XArrayList<T>::toString(){
    stringstream ss;
    ss << "[";
    for(int idx=0; idx < count-1; idx++){
        ss << data[idx] << ",";
    }
    if(count > 0)
        ss << data[count - 1] << "]";
    else 
        ss << "]";
    return ss.str();
}


template<class T>
void XArrayList<T>::dump(){
    string line(50, '=');
    cout << line << endl;
    cout << "XArrayList's information:" << endl;
    cout << "capacity: " << capacity << endl;
    cout << "Item type: " << typeid(T).name() << endl;
    cout << "item's size: " << sizeof(T) << " (bytes)"<< endl;
    cout << "count: " << count << endl;
    cout << toString() << endl;
    cout << line << endl;
}


//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////
template<class T>
void XArrayList<T>::checkIndex(int index){
    if((index < 0) || (index >= count))
        throw std::out_of_range("index is out of range");

}
template<class T>
void XArrayList<T>::ensureCapacity(int index){
    if((index < 0) || (index > count) ) //illegal
        throw std::out_of_range("the input index is out of range!");

    if(index >= capacity){
        //re-allocate 
        int old_capacity = capacity;
        capacity = old_capacity + (old_capacity >> 2);
        try{
            T* new_data = new T[capacity];
            memcpy(new_data, data, capacity*sizeof(T));
            delete []data;
            data = new_data;
        }
        catch(std::bad_alloc e){
            e.what();
        }
    }
}


#endif /* XARRAYLIST_H */

