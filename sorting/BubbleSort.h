/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BubbleSort.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:16
 */

#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "sorting/ISort.h"

template<class T>
class BubbleSort: public ISort<T>{
public:
    void sort(T array[], int size, int (*comparator)(T&,T&)){
        int current, walker;
        bool flag;
        
        current = 0; 
        flag = false;
        while((current < size-1) && (flag == false)){
            walker = size - 1; //start from the last and backward
            flag = true; //for testing if the input already in ascending order
            while(walker > current){
                if(comparator(array[walker], array[walker-1]) < 0 ){
                    flag = false;
                    //swap:
                    T temp = array[walker];
                    array[walker] = array[walker-1];
                    array[walker-1] = temp;
                }
                walker -= 1;
            }
            current += 1;
        }
    }
};


#endif /* BUBBLESORT_H */

