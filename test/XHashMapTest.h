/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   XHashMapTest.h
 * Author: LTSACH
 *
 * Created on 27 August 2020, 12:40
 */

#ifndef XHASHMAPTEST_H
#define XHASHMAPTEST_H


#include "doctest.h"

#include "hash/XHashMap.h"
#include "util/Point.h"

TEST_CASE( "XHashMap<int, i" ) {
    XHashMap<int, int> map(&XHashMap<int, int>::simpleHash);
    REQUIRE(map.size() == 0);
    
    SUBCASE("put then size"){
        int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
        int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
        for(int idx=0; idx < 8; idx++){
            map.put(keys[idx], values[idx]);
        }
        REQUIRE(map.size() == 8);
    }
    SUBCASE("put/clear then size"){
        int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
        int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
        for(int idx=0; idx < 8; idx++){
            map.put(keys[idx], values[idx]);
        }
        REQUIRE(map.size() == 8);
        map.clear();
        REQUIRE(map.size() == 0);
        
        //put again, then check size
        for(int idx=0; idx < 8; idx++){
            map.put(keys[idx], values[idx]);
        }
        REQUIRE(map.size() == 8);
    }
    SUBCASE("put => containsKey, containsValue"){
        int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
        int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
        for(int idx=0; idx < 8; idx++){
            map.put(keys[idx], values[idx]);
        }
        REQUIRE(map.size() == 8);
        //
        for(int idx=0; idx < 8; idx++){
            REQUIRE(map.containsKey(keys[idx]) == true);
            REQUIRE(map.containsValue(values[idx]) == true);
        }
        
        REQUIRE(map.containsKey(1000) == false);
        REQUIRE(map.containsKey(1000) == false);
        
    }
    
    SUBCASE("put => get"){
        int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
        int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
        for(int idx=0; idx < 8; idx++){
            map.put(keys[idx], values[idx]);
        }
        REQUIRE(map.size() == 8);
        //
        for(int idx=0; idx < 8; idx++){
            REQUIRE(map.get(keys[idx]) == values[idx]);
        }
    }
    
    SUBCASE("put => remove"){
        int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
        int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
        for(int idx=0; idx < 8; idx++){
            map.put(keys[idx], values[idx]);
        }
        REQUIRE(map.size() == 8);
        //
        REQUIRE(map.get(2) == 35);
        map.remove(2);
        REQUIRE(map.containsKey(2) == false);
        REQUIRE(map.containsKey(35) == false);
    }
    
    SUBCASE("put => remove(key, value)"){
        int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
        int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
        for(int idx=0; idx < 8; idx++){
            map.put(keys[idx], values[idx]);
        }
        REQUIRE(map.size() == 8);
        //
        REQUIRE(map.remove(30, 45) == true);
        REQUIRE(map.remove(2, 35) == true);
        REQUIRE(map.remove(2, 45) == false);
        REQUIRE(map.remove(1000, 45) == false);
    }
    SUBCASE("put with new value "){
        int keys[]      = {2,  12, 42,  72, 3,  45, 76, 30};
        int values[]    = {35, 67, 100, 23, 68, 68, 72, 45};
        for(int idx=0; idx < 8; idx++){
            map.put(keys[idx], values[idx]);
        }
        REQUIRE(map.size() == 8);
        //
        REQUIRE(map.containsKey(42) == true);
        REQUIRE(map.containsValue(100) == true);
        
        map.put(42, 9999);
        REQUIRE(map.containsKey(42) == true);
        REQUIRE(map.containsValue(100) == false);
        REQUIRE(map.containsValue(9999) == true);
        REQUIRE(map.get(42) == 9999);
    }
    
}

    


#endif /* XHASHMAPTEST_H */

