/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   XHapMap.h
 * Author: LTSACH
 *
 * Created on 22 August 2020, 21:57
 */

#ifndef XHASHMAP_H
#define XHASHMAP_H
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

#include "hash/IMap.h"

template<class K, class V>
class XHashMap: public IMap<K,V>{
public:
    class Entry; //forward declaration
    
protected:
    Entry** table;
    int capacity;
    int count;
    float loadFactor;
    int (*hashCode)(K,int); //tableSize = size of hashMap (capacity)
    bool (*keyEqual)(K,K);
    bool (*valueEqual)(V,V);
    void (*removeKey)(XHashMap<K,V>*);
    void (*removeValue)(XHashMap<K,V>*);
    string (*key2str)(K);
    string (*value2str)(V);
    
    
public:
    XHashMap(
            int (*hashCode)(K,int),
            float loadFactor=0.75f,
            bool (*valueEqual)(V, V)=0,
            string (*value2str)(V)=0,
            void (*removeValue)(XHashMap<K,V>*)=0,
            bool (*keyEqual)(K, K)=0,
            string (*key2str)(K)=0,
            void (*removeKey)(XHashMap<K,V>*)=0);
    ~XHashMap();
    
    V put(K key, V value);
    V get(K key);
    V remove(K key);
    bool remove(K key, V value);
    bool containsKey(K key);
    bool containsValue(V value);
    bool empty();
    int size();
    void clear();
    string toString();
    void println(){
        cout << this->toString() << endl;
    }
    static int simpleHash(K key, int capacity){
        return key%capacity;
    }
    static void freeKey(XHashMap<K,V> *pMap){
        for(int idx=0; idx < pMap->capacity; idx++){
            Entry* entry = pMap->table[idx];
            while(entry != 0){
                delete entry->key;
                entry = entry->next;
            }
        }
    }
    static void freeValue(XHashMap<K,V> *pMap){
        for(int idx=0; idx < pMap->capacity; idx++){
            Entry* entry = pMap->table[idx];
            while(entry != 0){
                delete entry->value;
                entry = entry->next;
            }
        }
    }
    
private:
    void ensureLoadFactor(int minCapacity);
    void rehash(int newCapacity);
    
    bool keyEQ(K lhs, K rhs){
        if(keyEqual != 0) return keyEqual(lhs, rhs);
        else return lhs==rhs;
    }
    bool valueEQ(V lhs, V rhs){
        if(valueEqual != 0) return valueEqual(lhs, rhs);
        else return lhs==rhs;
    }
    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Entry{
    private:
        K key;
        V value;
        Entry* prev;
        Entry* next;
        friend class XHashMap<K,V>;
        
    public:
        Entry(){
            this->prev = 0;
            this->next = 0;
        }
        Entry(K key, V value, Entry* prev=0, Entry* next=0){
            this->key = key;
            this->value = value;
            this->prev = prev;
            this->next = next;
        }
    };
};


//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template<class K, class V>
XHashMap<K,V>::XHashMap(
                int (*hashCode)(K,int),
                float loadFactor,
                bool (*valueEqual)(V lhs, V rhs),
                string (*value2str)(V value),
                void (*removeValue)(XHashMap<K,V>*),
                bool (*keyEqual)(K lhs, K rhs),
                string (*key2str)(K key),
                void (*removeKey)(XHashMap<K,V>* pMap) ){
    this->capacity = 10;
    this->count = 0;
    this->table = new Entry*[capacity];
    this->loadFactor = loadFactor;
    this->valueEqual = valueEqual;
    this->keyEqual = keyEqual;
    this->hashCode = hashCode;
    this->removeKey = removeKey;
    this->removeValue = removeValue;
    this->key2str = key2str;
    this->value2str = value2str;
    //reset table to nullptr
    for(int idx =0; idx < capacity; idx++){
        this->table[idx] = 0;
    }
}

template<class K, class V>
XHashMap<K,V>::~XHashMap(){
    //Remove user's data
    if(removeKey != 0) removeKey(this);
    if(removeValue != 0) removeValue(this);
        
    //Remove table
    delete []table;
}

template<class K, class V>
V XHashMap<K,V>::put(K key, V value){
    int index = this->hashCode(key, capacity);
    Entry* entry = table[index];
    
    while(entry != 0){
        if(keyEQ(entry->key, key)){
            V oldValue = entry->value;
            entry->value = value; //replaced with new value
            return oldValue;
        }
        entry = entry->next;
    }
    //add to bucket, at head
    
    Entry* newEntry = new Entry(key, value, 0, table[index]);
    if(table[index] != 0) table[index]->prev = newEntry;
    table[index] = newEntry;
    count += 1;
    ensureLoadFactor(count);
    return value;
}

template<class K, class V>
V XHashMap<K,V>::get(K key){
    V retValue; 
    int index = hashCode(key, capacity);
    Entry* entry = table[index];
    while(entry != 0){
        if(keyEQ(entry->key, key)){
            retValue = entry->value;
            break;
        }
        entry = entry->next;
    }
    return retValue; 
    //key not exist => returned value does not having any meaning
    //=> check contains(key) before get(key)
}

template<class K, class V>
V XHashMap<K,V>::remove(K key){
    int index = hashCode(key, capacity);
    Entry* entry = table[index];
    while(entry != 0){
        if(keyEQ(entry->key, key)){
            count -= 1;
            if(entry->prev == 0){ 
                //remove head
                Entry* newHead = entry->next;
                if(newHead != 0) newHead->prev = 0;
                table[index] = newHead;
                entry->next = 0; //detach from the chain
                return entry->value;
            }
            else if(entry->next == 0){
                //remove tail
                Entry* prev = entry->prev;
                prev->next = 0;
                entry->prev = 0; //detach
                return entry->value;
            }
            else{
                //remove node in-between head and tail
                entry->prev->next = entry->next;
                entry->next->prev = entry->prev;
                entry->prev = entry->next = 0;//detach
                return entry->value;
            }
        }
        entry = entry->next;
    }
    V v;
    return v; 
}

template<class K, class V>
bool XHashMap<K,V>::remove(K key, V value){
    int index = hashCode(key, capacity);
    Entry* entry = table[index];
    while(entry != 0){
        if(keyEQ(entry->key, key) && valueEQ(entry->value, value) ){
            count -= 1;
            if(entry->prev == 0){ 
                //remove head
                Entry* newHead = entry->next;
                if(newHead != 0) newHead->prev = 0;
                table[index] = newHead;
                entry->next = 0; //detach from the chain
                return true;
            }
            else if(entry->next == 0){
                //remove tail
                Entry* prev = entry->prev;
                prev->next = 0;
                entry->prev = 0; //detach
                return true;
            }
            else{
                //remove node in-between head and tail
                entry->prev->next = entry->next;
                entry->next->prev = entry->prev;
                entry->prev = entry->next = 0;//detach
                return true;
            }
        }
        entry = entry->next;
    }
    return false;
}

template<class K, class V>
bool XHashMap<K,V>::containsKey(K key){
    int index = hashCode(key, capacity);
    Entry* entry = table[index];
    while(entry != 0){
        if(keyEQ(entry->key, key)) return true;
        entry = entry->next;
    }
    return false;
}

template<class K, class V>
bool XHashMap<K,V>::containsValue(V value){
    for(int idx=0; idx < capacity; idx++){
        Entry* entry = table[idx];
        while(entry != 0){
            if(valueEQ(entry->value, value)) return true;
            entry = entry->next;
        }
    }
    return false;
}
template<class K, class V>
bool XHashMap<K,V>::empty(){
    return count == 0;
}

template<class K, class V>
int XHashMap<K,V>::size(){
    return count;
}

template<class K, class V>
void XHashMap<K,V>::clear(){
    //Remove user's data
    if(removeKey != 0) removeKey(this);
    if(removeValue != 0) removeValue(this);
        
    //Remove table
    delete []table;
    
    //initialize
    this->capacity = 10;
    this->count = 0;
    this->table = new Entry*[capacity];
    
    //reset table to nullptr
    for(int idx =0; idx < capacity; idx++){
        this->table[idx] = 0;
    }
}

template<class K, class V>
string XHashMap<K,V>::toString(){
    stringstream os;
    string mark(50, '=');
    os << mark << endl;
    os << setw(12) << left << "capacity: "  << capacity << endl;
    os << setw(12) << left << "size: " << count << endl;
    for(int idx=0; idx < capacity; idx++){
        Entry* entry = table[idx];
        os << setw(4) << left << idx << ": ";
        stringstream valueos;
        while(entry != 0){
            valueos << " (";
            
            if(key2str != 0) valueos << key2str(entry->key);
            else valueos << entry->key;
            valueos << ",";
            if(value2str != 0) valueos << value2str(entry->value);
            else valueos << entry->value;
            
            valueos << ");";
            
            //next entry:
            entry = entry->next;
        }
        string valuestr = valueos.str();
        if(valuestr.length() > 0) valuestr = valuestr.substr(0, valuestr.length()-1);
        os << valuestr  << endl;
    }
    os << mark << endl;
  
    return os.str();
}

//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////

template<class K, class V>
void XHashMap<K,V>::ensureLoadFactor(int minCapacity){
    int maxSize = (int)(loadFactor*capacity);
   
    if(minCapacity >= maxSize){
        //grow: oldCapacity >> 1 (= oldCapacity/2)
        int oldCapacity = capacity;
        int newCapacity = oldCapacity + (oldCapacity >> 1);
        rehash(newCapacity);
    }   
}

template<class K, class V>
void XHashMap<K,V>::rehash(int newCapacity){
    Entry** oldTable = this->table;
    int oldCapacity = capacity;
    
    this->table = new Entry*[newCapacity];
    this->count = 0;
    this->capacity = newCapacity;
    
    //reset table to nullptr
    for(int idx =0; idx < capacity; idx++){
        this->table[idx] = 0;
    }

    for(int idx=0; idx < oldCapacity; idx++) {
        Entry* pEntry = oldTable[idx];
        while(pEntry != 0){
            this->put(pEntry->key, pEntry->value);
            pEntry = pEntry->next;
        }
    }
}


#endif /* XHASHMAP_H */

