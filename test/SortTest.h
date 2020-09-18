/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SortTest.h
 * Author: LTSACH
 *
 * Created on 27 August 2020, 13:03
 */

#ifndef SORTTEST_H
#define SORTTEST_H



#include "doctest.h"

#include "sorting/StraightInsertionSort.h"
#include "sorting/ShellSort.h"
#include "sorting/StraightSelectionSort.h"
#include "sorting/BubbleSort.h"
#include "util/Point.h"

TEST_CASE( "Sorting ascending" ) {
    int shell_segments[] = {1, 3, 7};
    ISort<int>* pAlgorithm[] ={
        new StraightInsertionSort<int>(),
        new ShellSort<int>(shell_segments, 3),
        new StraightSelectionSort<int>(),
        new BubbleSort<int>()
    };
    
    for(int algIdx = 0; algIdx < 4; algIdx++){
        int values[] = {45, 97, 12,  2,  39,  3,  37, 87};
        
        pAlgorithm[algIdx]->sort(values, 8, &SortSimpleOrder<int>::compare4Asending);
        
        //Then check values: ascending
        int exp[] =  {2,  3,  12,  37, 39,  45, 87, 97};
        for(int idx=0; idx < 8; idx++){
            REQUIRE(values[idx] == exp[idx]);
        }
    }
    
    //Delete sorters
    for(int algIdx = 0; algIdx < 4; algIdx++){
        delete pAlgorithm[algIdx];
    }
}

TEST_CASE( "Sorting ascending" ) {
    int shell_segments[] = {1, 3, 7};
    ISort<int>* pAlgorithm[] ={
        new StraightInsertionSort<int>(),
        new ShellSort<int>(shell_segments, 3),
        new StraightSelectionSort<int>(),
        new BubbleSort<int>()
    };
    
    for(int algIdx = 0; algIdx < 4; algIdx++){
        int values[] = {45, 97, 12,  2,  39,  3,  37, 87};
        
        pAlgorithm[algIdx]->sort(values, 8, &SortSimpleOrder<int>::compare4Desending);
        
        //Then check values: descending 
        int exp[] =  {97,  87, 45, 39,  37,  12,  3,  2}; 
        for(int idx=0; idx < 8; idx++){
            REQUIRE(values[idx] == exp[idx]);
        }
    }
    
    //Delete sorters
    for(int algIdx = 0; algIdx < 4; algIdx++){
        delete pAlgorithm[algIdx];
    }
}

#endif /* SORTTEST_H */

