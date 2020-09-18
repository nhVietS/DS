/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StackTest.h
 * Author: LTSACH
 *
 * Created on 27 August 2020, 10:00
 */

#ifndef STACKTEST_H
#define STACKTEST_H
#include "doctest.h"

#include "stacknqueue/Stack.h"
#include "util/Point.h"


TEST_CASE( "Stack<int>:" ) {
    Stack<int> stack;
    REQUIRE(stack.empty() == true);
    REQUIRE(stack.size() == 0);
    
    int item[] = {3, 1, 4, 5, 2, 8, 10, 12};
        //index:  0  1  2  3  4  5  6   7
    for(int idx=0; idx < 8; idx++) stack.push(item[idx]);
    REQUIRE(stack.empty() == false);
    REQUIRE(stack.size() == 8);
    
    SUBCASE("Stack: clear/empty/size"){
        stack.clear();
        REQUIRE(stack.empty() == true);
        REQUIRE(stack.size() == 0);
    }
    SUBCASE("Stack: peek/pop"){
        for(int idx=7; idx >= 0; idx--){
            REQUIRE(stack.peek() == item[idx]);
            REQUIRE(stack.pop() == item[idx]);
        }
    }
    SUBCASE("Stack: contains"){
        for(int idx=7; idx >= 0; idx--){
            REQUIRE(stack.contains(item[idx]) == true);
            stack.pop();
            REQUIRE(stack.contains(item[idx]) == false);
        }
        REQUIRE(stack.contains(100) == false);
        stack.push(100);
        REQUIRE(stack.contains(100) == true);
    }
    SUBCASE("Stack: remove"){
        REQUIRE(stack.contains(12) == true);
        stack.remove(12);
        REQUIRE(stack.contains(12) == false);
        
        REQUIRE(stack.contains(3) == true);
        stack.remove(3);
        REQUIRE(stack.contains(3) == false);
        
        REQUIRE(stack.contains(5) == true);
        stack.remove(5);
        REQUIRE(stack.contains(5) == false);
    }
    SUBCASE("Stack: iterator for reading"){
        Stack<int>::Iterator it;
        int top = 7;
        for(it = stack.top(); it != stack.bottom(); it++){
            REQUIRE(*it == item[top--]);
        }
    }
    SUBCASE("Stack: iterator for removing"){
        stack.clear();
        REQUIRE(stack.empty() == true);
        
        for(int idx =0; idx < 10; idx++) stack.push(idx);
        REQUIRE(stack.empty() == false);
        REQUIRE(stack.size() == 10);
        
        Stack<int>::Iterator it;
        for(it = stack.top(); it != stack.bottom(); it++){
            if(*it % 2 == 1) it.remove(); //remove odd
        }
        REQUIRE(stack.size() == 5); //from top: 8, 6, 4, 2, 0
        
        //check values
        int exp[] = {8, 6, 4, 2, 0};
        int idx = 0;
        while(!stack.empty()){
            REQUIRE(stack.pop() == exp[idx]);
            idx += 1;
        }
    }
}

#endif /* STACKTEST_H */

