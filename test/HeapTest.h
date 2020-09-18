/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HeapTest.h
 * Author: LTSACH
 *
 * Created on 27 August 2020, 11:32
 */

#ifndef HEAPTEST_H
#define HEAPTEST_H

#include "doctest.h"

#include "heap/Heap.h"
#include "util/Point.h"


int minComparator(int& a, int& b){
    if (a < b) return -1;
    else if(a > b) return +1;
    else return 0;
}
int maxComparator(int& a, int& b){
    if (a < b) return +1;
    else if(a > b) return -1;
    else return 0;
}

TEST_CASE( "Heap<int>: min and max heap" ) {
    Heap<int> minHeap; //need not passing &minComparator, default is min-heap
    Heap<int> maxHeap(&maxComparator);
    REQUIRE(minHeap.size() == 0);
    REQUIRE(maxHeap.size() == 0);
    
    int item[] = {50, 20, 15, 10, 8, 6, 7, 23}; 
    
    SUBCASE("min-heap: heapify"){
        minHeap.heapify(item, 8);
        REQUIRE(minHeap.size() == 8);
        
        //check values
        int minexp[] = {6, 10, 7, 23, 15, 20, 8, 50};
        
        Heap<int>::Iterator it;
        int idx = 0;
        for(it = minHeap.begin(); it != minHeap.end(); it++){
            REQUIRE(minexp[idx++] == *it);
        }
        
        minHeap.clear();
        REQUIRE(minHeap.empty() == true);
        minHeap.heapify(item, 8);
        REQUIRE(minHeap.size() == 8);
    }
    SUBCASE("min-heap: heapify/peep"){
        minHeap.heapify(item, 8);
        REQUIRE(minHeap.size() == 8);
        
        //check values
        int peek_exp[] = {6, 7, 8, 10, 15, 20, 23, 50};
        
        Heap<int>::Iterator it;
        int idx = 0;
        while(!minHeap.empty()){
            REQUIRE(minHeap.peek() == peek_exp[idx]);
            REQUIRE(minHeap.pop() == peek_exp[idx]);
            idx += 1;
        }
    }
    SUBCASE("min-heap: heapify/pop"){
        minHeap.heapify(item, 8);
        REQUIRE(minHeap.size() == 8);
        
        //check values
        int minexp[] = {6, 10, 7, 23, 15, 20, 8, 50};
        int exp1[] = {7, 10, 8, 23, 15, 20, 50}; //pop 1st
        int exp2[] = {8, 10, 20, 23, 15, 50}; // pop 2nd
        int exp3[] = {10, 15, 20, 23, 50}; //pop 3rd
        
        Heap<int>::Iterator it;
        int idx;
        
        REQUIRE(minHeap.pop() == 6);
        idx = 0;
        for(it = minHeap.begin(); it != minHeap.end(); it++){
            REQUIRE(exp1[idx++] == *it);
        }
        
        REQUIRE(minHeap.pop() == 7);
        idx = 0;
        for(it = minHeap.begin(); it != minHeap.end(); it++){
            REQUIRE(exp2[idx++] == *it);
        }
        
        REQUIRE(minHeap.pop() == 8);
        idx = 0;
        for(it = minHeap.begin(); it != minHeap.end(); it++){
            REQUIRE(exp3[idx++] == *it);
        }
        
        REQUIRE(minHeap.size() == 5);
        
    }
    SUBCASE("min-heap: remove"){
        minHeap.heapify(item, 8);
        REQUIRE(minHeap.size() == 8);
        
        int exp1[] = {6, 10, 7, 15, 20, 8, 50}; //after removing 23
        int exp2[] = {6, 8, 7, 15, 10, 50}; //after removing 23, then 20
        
        minHeap.remove(23);
        Heap<int>::Iterator it;
        int idx = 0;
        for(it = minHeap.begin(); it != minHeap.end(); it++){
            REQUIRE(exp1[idx++] == *it);
        }
        
        minHeap.remove(20);
        idx = 0;
        for(it = minHeap.begin(); it != minHeap.end(); it++){
            REQUIRE(exp2[idx++] == *it);
        }
    }
    
    SUBCASE("max-heap: heapify"){
        maxHeap.heapify(item, 8);
        REQUIRE(maxHeap.size() == 8);
        
        //check values
        int maxexp[] = {50, 23, 15, 20, 8, 6, 7, 10};
        Heap<int>::Iterator it;
        int idx = 0;
        for(it = maxHeap.begin(); it != maxHeap.end(); it++){
            REQUIRE(maxexp[idx++] == *it);
        }
        
        maxHeap.clear();
        REQUIRE(maxHeap.empty() == true);
        maxHeap.heapify(item, 8);
        REQUIRE(maxHeap.size() == 8);
    }
    SUBCASE("min-heap: heapify/peep"){
        maxHeap.heapify(item, 8);
        REQUIRE(maxHeap.size() == 8);
        
        //check values
        int peek_exp[] = {50, 23, 20, 15, 10, 8, 7, 6}; 
        
        Heap<int>::Iterator it;
        int idx = 0;
        while(!maxHeap.empty()){
            REQUIRE(maxHeap.peek() == peek_exp[idx]);
            REQUIRE(maxHeap.pop() == peek_exp[idx]);
            idx += 1;
        }
    }
    
    SUBCASE("max-heap: heapify/pop"){
        maxHeap.heapify(item, 8);
        REQUIRE(maxHeap.size() == 8);
        
        //check values
        int maxexp[] = {50, 23, 15, 20, 8, 6, 7, 10};
        int exp1[] = {23, 20, 15, 10, 8, 6, 7}; //pop 1st
        int exp2[] = {20, 10, 15, 7, 8, 6}; // pop 2nd
        int exp3[] = {15, 10, 6, 7, 8}; //pop 3rd
        
        Heap<int>::Iterator it;
        int idx;
        
        REQUIRE(maxHeap.pop() == 50);
        idx = 0;
        for(it = maxHeap.begin(); it != maxHeap.end(); it++){
            REQUIRE(exp1[idx++] == *it);
        }
        
        REQUIRE(maxHeap.pop() == 23);
        idx = 0;
        for(it = maxHeap.begin(); it != maxHeap.end(); it++){
            REQUIRE(exp2[idx++] == *it);
        }
        
        REQUIRE(maxHeap.pop() == 20);
        idx = 0;
        for(it = maxHeap.begin(); it != maxHeap.end(); it++){
            REQUIRE(exp3[idx++] == *it);
        }
        
        REQUIRE(maxHeap.size() == 5);
        
    }
    
    SUBCASE("max-heap: remove"){
        maxHeap.heapify(item, 8);
        REQUIRE(maxHeap.size() == 8);
        
 
        int exp1[] = {50, 23, 20, 8, 6, 7, 10}; //after removing 15
        int exp2[] = {23, 20, 10, 6, 7, 8}; //after removing 15, then 50
        
        maxHeap.remove(15);
        Heap<int>::Iterator it;
        int idx = 0;
        for(it = maxHeap.begin(); it != maxHeap.end(); it++){
            REQUIRE(exp1[idx++] == *it);
        }
        
        maxHeap.remove(50);
        idx = 0;
        for(it = maxHeap.begin(); it != maxHeap.end(); it++){
            REQUIRE(exp2[idx++] == *it);
        }
    }
}

#endif /* HEAPTEST_H */

