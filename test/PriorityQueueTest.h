/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PriorityQueueTest.h
 * Author: LTSACH
 *
 * Created on 27 August 2020, 12:33
 */

#ifndef PRIORITYQUEUETEST_H
#define PRIORITYQUEUETEST_H


#include "doctest.h"

#include "stacknqueue/PriorityQueue.h"
#include "util/Point.h"

TEST_CASE( "PriorityQueue<int>: min and max heap" ) {
    PriorityQueue<int> queue; //need not passing &minComparator, default is min-heap
    REQUIRE(queue.size() == 0);
    REQUIRE(queue.size() == 0);
    
    int item[] = {50, 20, 15, 10, 8, 6, 7, 23}; 
    
    SUBCASE("min better queue"){
        queue.heapify(item, 8);
        REQUIRE(queue.size() == 8);
        
        //check values
        int minexp[] = {6, 10, 7, 23, 15, 20, 8, 50};
        
        Heap<int>::Iterator it;
        int idx = 0;
        for(it = queue.begin(); it != queue.end(); it++){
            REQUIRE(minexp[idx++] == *it);
        }
        
        queue.clear();
        REQUIRE(queue.empty() == true);
        queue.heapify(item, 8);
        REQUIRE(queue.size() == 8);
    }
}


#endif /* PRIORITYQUEUETEST_H */

