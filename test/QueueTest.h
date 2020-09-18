/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   QueueTest.h
 * Author: LTSACH
 *
 * Created on 27 August 2020, 11:18
 */

#ifndef QUEUETEST_H
#define QUEUETEST_H

#include "doctest.h"

#include "stacknqueue/Queue.h"
#include "util/Point.h"


TEST_CASE( "Queue<int>:" ) {
    Queue<int> queue;
    REQUIRE(queue.empty() == true);
    REQUIRE(queue.size() == 0);
    
    int item[] = {3, 1, 4, 5, 2, 8, 10, 12};
        //index:  0  1  2  3  4  5  6   7
    for(int idx=0; idx < 8; idx++) queue.push(item[idx]);
    REQUIRE(queue.empty() == false);
    REQUIRE(queue.size() == 8);
    
    SUBCASE("Queue: clear/empty/size"){
        queue.clear();
        REQUIRE(queue.empty() == true);
        REQUIRE(queue.size() == 0);
    }
    SUBCASE("Queue: peek/pop"){
        for(int idx=0; idx < 8; idx++){
            REQUIRE(queue.peek() == item[idx]);
            REQUIRE(queue.pop() == item[idx]);
        }
    }
    SUBCASE("Queue: contains"){
        for(int idx=0; idx < 8; idx++){
            REQUIRE(queue.contains(item[idx]) == true);
            queue.pop();
            REQUIRE(queue.contains(item[idx]) == false);
        }
        REQUIRE(queue.contains(100) == false);
        queue.push(100);
        REQUIRE(queue.contains(100) == true);
    }
    SUBCASE("Queue: remove"){
        REQUIRE(queue.contains(12) == true);
        queue.remove(12);
        REQUIRE(queue.contains(12) == false);
        
        REQUIRE(queue.contains(3) == true);
        queue.remove(3);
        REQUIRE(queue.contains(3) == false);
        
        REQUIRE(queue.contains(5) == true);
        queue.remove(5);
        REQUIRE(queue.contains(5) == false);
    }
    SUBCASE("Queue: iterator for reading"){
        Queue<int>::Iterator it;
        int idx = 0;
        for(it = queue.front(); it != queue.rear(); it++){
            REQUIRE(*it == item[idx++]);
        }
    }
    SUBCASE("Queue: iterator for removing"){
        queue.clear();
        REQUIRE(queue.empty() == true);
        
        for(int idx =0; idx < 10; idx++) queue.push(idx);
        REQUIRE(queue.empty() == false);
        REQUIRE(queue.size() == 10);
        
        Queue<int>::Iterator it;
        for(it = queue.front(); it != queue.rear(); it++){
            if(*it % 2 == 1) it.remove(); //remove odd
        }
        REQUIRE(queue.size() == 5); //from top: 0,2,4,6,8
        
        //check values
        int exp[] = {0,2,4,6,8};
        int idx = 0;
        while(!queue.empty()){
            REQUIRE(queue.pop() == exp[idx]);
            idx += 1;
        }
    }
}

#endif /* QUEUETEST_H */

