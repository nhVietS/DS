/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BSTTest.h
 * Author: LTSACH
 *
 * Created on 25 August 2020, 16:54
 */

#ifndef BSTTEST_H
#define BSTTEST_H

#include "doctest.h"
#include "tree/BST.h"
#include "util/Point.h"
#include "list/DLinkedList.h"


/*
 IMPORTANT NOTE:
 * BST<K,V>:
 *  K: Keytype, required to support operato "<", can be:
 *      + a primitive type: int, float, ...
 *      + class type (e.g. Point): override operator <
 *  V: Valuetype: required to be a pointer type
 *      + example: int*, float*, double*, Point*, etc
 */

//////////////////////////////////////////////////////////////////////
///////////////// BST: ONLY Key Provided            //////////////////
//////////////////////////////////////////////////////////////////////


/* USE TREE:
            50
          /    \
        20      80
       /       /  \
      15      60   90
     /  \          /
    7   18        85
     \
      12
 */

TEST_CASE( "BST<int, int*>[1]: with null value" ) {
    BST<int, int*> tree;
    REQUIRE(tree.size() == 0);
    
    int values[] = {50, 20, 80, 15, 60, 90, 7, 12, 18, 85};
    int size = 10;
    for(int idx=0; idx <size; idx++)
        tree.add(values[idx]);
    //////////////////////////////////////////////////
    
    SUBCASE("Check size(), height(): in combination with add() and clear()"){
        REQUIRE(tree.size() == size);
        REQUIRE(tree.height() == 5);
        
        tree.clear();
        REQUIRE(tree.size() == 0);
        REQUIRE(tree.height() == 0);
        
        /*
                15
         */
        tree.add(15);
        REQUIRE(tree.size() == 1);
        REQUIRE(tree.height() == 1);
        
        /*
                15
                  \
                   15
         */
        
        tree.add(15);
        REQUIRE(tree.size() == 2);
        REQUIRE(tree.height() == 2);
        
        /*
                15
                  \
                   15
                     \
                      25
         */
        
        tree.add(25);
        REQUIRE(tree.size() == 3);
        REQUIRE(tree.height() == 3);
        
        /*
                15
               /  \
              10    15
                     \
                      25
         */
        
        tree.add(10);
        REQUIRE(tree.size() == 4);
        REQUIRE(tree.height() == 3);
        
        /*
                15
               /  \
              10    15
               \     \
               14     25
         */
        
        tree.add(14);
        REQUIRE(tree.size() == 5);
        REQUIRE(tree.height() == 3);
        
        /*
         empty tree
         */
        tree.clear();
        REQUIRE(tree.size() == 0);
        REQUIRE(tree.height() == 0);
        
    }
    SUBCASE("Check size(), height(): in combination with add() and remove()"){
        /* USE TREE:
                    50
                  /    \
                20      80
               /       /  \
              15      60   90
             /  \          /
            7   18        85
             \
              12
         =>
                    50
                  /    \
                20      80
               /       /  \
              12      60   90
             /  \          /
            7   18        85
         */
        int* v;
        v = tree.remove(15);
        REQUIRE(tree.size() == 9);
        REQUIRE(tree.height() == 4);
        REQUIRE(v == (int*)0); //v is nullptr
        
        /* USE TREE:
                    50
                  /    \
                20      80
               /       /  \
              12      60   90
             /  \          /
            7   18        85
         =>
                    20
                  /   \
                12     80
               /  \   /  \
              7  18  60   90
                          /
                         85
         */     
        v = tree.remove(50);
        REQUIRE(tree.size() == 8);
        REQUIRE(tree.height() == 4);
        REQUIRE(v == (int*)0); //v is nullptr
        
        /* USE TREE:
                    20
                  /   \
                12     80
               /  \   /  \
              7  18  60   90
                          /
                         85
         =>
                    20
                  /   \
                12     80
               /  \   /  \
              7  18  60   85
         */
        v = tree.remove(90);
        REQUIRE(tree.size() == 7);
        REQUIRE(tree.height() == 3);
        REQUIRE(v == (int*)0); //v is nullptr
        
        /* USE TREE:
                    20
                  /   \
                12     80
               /  \   /  \
              7  18  60   85
         =>
                    20
                  /   \
                12     80
               /  \   /  \
              7  18  60   85
                   \
                    19
         */
        tree.add(19);
        REQUIRE(tree.size() == 8);
        REQUIRE(tree.height() == 4);
    }
    SUBCASE("Check search(): in combination with add/remove"){
        /* USE TREE:
                    50
                  /    \
                20      80
               /       /  \
              15      60   90
             /  \          /
            7   18        85
             \
              12
         */
        bool found;
        int* v;
        
        v = tree.search(20, found);
        REQUIRE(found == true); //found
        REQUIRE(v == (int*)0); //but value is nullptr, bc does not have value
        
        v = tree.search(120, found);
        REQUIRE(found == false); //found
        REQUIRE(v == (int*)0); //but value is nullptr, must return nullptr
        
        v = tree.search(12, found);
        REQUIRE(found == true); //found
        REQUIRE(v == (int*)0); //but value is nullptr, bc does not have value
        
        v = tree.search(85, found);
        REQUIRE(found == true); //found
        REQUIRE(v == (int*)0); //but value is nullptr, bc does not have value
        
        v = tree.search(50, found);
        REQUIRE(found == true); //found
        REQUIRE(v == (int*)0); //but value is nullptr, bc does not have value
        
        v = tree.search(200, found);
        REQUIRE(found == false); //found
        REQUIRE(v == (int*)0); //but value is nullptr, must return nullptr
        
        tree.add(200);
        v = tree.search(200, found);
        REQUIRE(found == true); //found
        REQUIRE(v == (int*)0); //but value is nullptr, bc does not have value
        
        tree.remove(50);
        v = tree.search(50, found);
        REQUIRE(found == false); //found
        REQUIRE(v == (int*)0); //but value is nullptr, must return nullptr
        
    }
    
}

//////////////////////////////////////////////////////////////////////
//////////BST: Key - primitive type; V - pointer to primitive ////////
//////////////////////////////////////////////////////////////////////

/* USE TREE:
                              (50, 100)
                 -----------------------------------
                /                                   \
            (20, 40)                             (80, 160)
       -------------------                ------------------------
      /                                  /                         \
    (15, 30)                        (60, 120)                   (90, 180)
     /     \                                                     /
  (7, 14) (18, 36)                                          (85, 170)
     \
     (12, 24)
 */


TEST_CASE( "BST<int, int*>[2]: using value of pointer to primitive type" ) {
    BST<int, int*> tree;
    int values[] = {50, 20, 80, 15, 60, 90, 7, 12, 18, 85};
    int size = 10;
    for(int idx=0; idx <size; idx++)
        tree.add(values[idx], new int(2*values[idx]) ); // value = 2*key
    
    SUBCASE("Check search(): with add/remove"){
        REQUIRE(tree.size() == 10);
        int* item;
        bool found;
        
        for(int idx=0; idx <size; idx++){
            item = tree.search(values[idx], found);
            REQUIRE(found == true); //found
            REQUIRE(*item == 2*values[idx]); //and value = 2*key
        }
        item = tree.search(100, found);
        REQUIRE(found == false); //not found
        REQUIRE(item == (int*)0); //must be nullptr


/* USE TREE:
                              (50, 100)
                 -----------------------------------
                /                                   \
            (20, 40)                             (80, 160)
       -------------------                ------------------------
      /                                  /                         \
    (15, 30)                        (60, 120)                   (90, 180)
     /     \                                                    /
  (7, 14) (18, 36)                                         (85, 170)
     \
     (12, 24)
 
 =>
                               (50, 100)
                 -----------------------------------
                /                                   \
            (20, 40)                             (80, 160)
       -------------------                ------------------------
      /                                  /                         \
    (15, 30)                        (60, 120)                   (90, 180)
     /     \                                                    /        \
  (7, 14) (18, 36)                                         (85, 170)  (100, 200)      
     \
     (12, 24)
 
 */
        
        tree.add(100, new int(200));
        REQUIRE(tree.size() == 11); //found
        item = tree.search(100, found);
        REQUIRE(found == true); //found
        REQUIRE(*item == 200); //and value = 200
        
/* USE TREE:
                               (50, 100)
                 -----------------------------------
                /                                   \
            (20, 40)                             (80, 160)
       -------------------                ------------------------
      /                                  /                         \
    (15, 30)                        (60, 120)                   (90, 180)
     /     \                                                    /        \
  (7, 14) (18, 36)                                         (85, 170)  (100, 200)      
     \
     (12, 24)
 
 =>
                               (50, 100)
                 -----------------------------------
                /                                   \
            (20, 40)                             (80, 160)
       -------------------                ------------------------
      /                                                           \
    (15, 30)                                                   (90, 180)
     /     \                                                         \
  (7, 14) (18, 36)                                               (100, 200)      
     \
     (12, 24)
 
 */        
        item = tree.remove(60);
        REQUIRE(*item == 120); delete item; //and value = 120
        REQUIRE(tree.size() == 10); //found
        item = tree.search(60, found);
        REQUIRE(found == false); //not found
        REQUIRE(item == (int*)0); //and must be nullptr
    }
}


//////////////////////////////////////////////////////////////////////
//////////BST: Key -primitive type; Value-pointer to Point////////////
//////////////////////////////////////////////////////////////////////

TEST_CASE( "BST<float, Point*>[3]: using value of pointer to class" ) {
    BST<float, Point*> tree;
    REQUIRE(tree.size() == 0); 
    
    Point* p1 = new Point(62.1, 23.2);
    Point* p2 = new Point(12.1, 13.22);
    Point* p3 = new Point(72.21, 13.62);
    Point* p4 = new Point(2.51, 3.25);
    tree.add(p1->getX(), p1);
    tree.add(p2->getX(), p2);
    tree.add(p3->getX(), p3);
    tree.add(p4->getX(), p4);
    //cout << tree.toString() << endl;
    REQUIRE(tree.size() == 4); 
    
    SUBCASE("Searching: FOUND CASE"){
        Point* p;
        bool found;
        p = tree.search(p1->getX(), found);
        REQUIRE(found == true);
        REQUIRE(p != (Point*)0);
        REQUIRE(p->getY() == p1->getY());
        
        p = tree.search(p2->getX(), found);
        REQUIRE(found == true);
        REQUIRE(p != (Point*)0);
        REQUIRE(p->getY() == p2->getY());
        
        p = tree.search(p3->getX(), found);
        REQUIRE(found == true);
        REQUIRE(p != (Point*)0);
        REQUIRE(p->getY() == p3->getY());
    }
    
    SUBCASE("Searching: NOT FOUND CASE"){
        Point* p;
        bool found;
        p = tree.search(100, found);
        REQUIRE(found == false);
        REQUIRE(p == (Point*)0);
        
        p = tree.search(200, found);
        REQUIRE(found == false);
        REQUIRE(p == (Point*)0);
    }
    
    SUBCASE("Searching: with add- then- found"){
        Point* p;
        bool found;
        p = tree.search(100, found);
        REQUIRE(found == false);
        REQUIRE(p == (Point*)0);
        
        
        tree.add(100, new Point(1,2));
        REQUIRE(tree.size() == 5);
        
        p = tree.search(100, found);
        REQUIRE(found == true);
        REQUIRE(p != (Point*)0);
        REQUIRE(p->getX() == 1);
        REQUIRE(p->getY() == 2);
    }
    SUBCASE("Searching: with remove- then- not found"){
        Point* p;
        bool found;
        //p1->getX() => 62.1
        p = tree.search(62.1, found);
        REQUIRE(found == true);
        REQUIRE(p != (Point*)0);
        REQUIRE(p->getY() == p1->getY());
        
        p = tree.remove(62.1);
        REQUIRE(tree.size() == 3);
        REQUIRE(p != (Point*)0); 
        REQUIRE(p->getY() == p1->getY()); delete p;
        
        //then search with 62.1
         p = tree.search(62.1, found);
        REQUIRE(found == false); //not found
        REQUIRE(p == (Point*)0); //nullptr
    }
}


//////////////////////////////////////////////////////////////////////
//////////BST: Key - primitive type; V - pointer to primitive ////////
//////////////////////////////////////////////////////////////////////

/* USE TREE:
                              (50, 50)
                 -----------------------------------
                /                                   \
            (20, 20)                             (80, 80)
       -------------------                ------------------------
      /                                  /                         \
    (15, 15)                        (60, 60)                   (90, 90)
     /     \                                                   /
  (7, 7) (18, 18)                                         (85, 85)
     \
     (12, 12)
 * 
 * 
 * lnr: {7, 12, 15, 18, 20, 50, 60, 80, 85, 90}
 * nlr: {50, 20, 15, 7, 12, 18, 80, 60, 90, 85}
 * lrn: {12, 7, 18, 15, 20, 60, 85, 90, 80, 50}
 * dfs=nlr: {50, 20, 15, 7, 12, 18, 80, 60, 90, 85} //theory: maybe other result
 * bfs: {50, 20, 80, 15, 60, 90, 7, 18, 85, 12} //theory: maybe other result
 */

TEST_CASE( "BST<int, int*>[4]: ascending/descending/lnr ..." ) {
    BST<int, int*> tree;
    int values[] = {50, 20, 80, 15, 60, 90, 7, 12, 18, 85};
    int size = 10;
    for(int idx=0; idx <size; idx++)
        tree.add(values[idx], new int(values[idx])); //key=value
    //cout << tree.toString() << endl;
    
    SUBCASE("BST: Ascending"){
        int expvalues[] = {7, 12, 15, 18, 20, 50, 60, 80, 85, 90};
        DLinkedList<int*> list = tree.ascendingList();
        
        DLinkedList<int*>::Iterator it;
        int index=0;
        for(it = list.begin(); it != list.end(); it++){
            REQUIRE(expvalues[index] == **it);
            index += 1;
        }
    }
    SUBCASE("BST: Descending"){
        int expvalues[] = {90, 85, 80, 60, 50, 20, 18, 15, 12, 7};
        DLinkedList<int*> list = tree.descendingList();
        
        DLinkedList<int*>::Iterator it;
        int index=0;
        for(it = list.begin(); it != list.end(); it++){
            REQUIRE(expvalues[index] == **it);
            index += 1;
        }
    }
    SUBCASE("BST: lnr"){
        int expvalues[] = {7, 12, 15, 18, 20, 50, 60, 80, 85, 90};
        DLinkedList<int*> list = tree.lnr();
        
        DLinkedList<int*>::Iterator it;
        int index=0;
        for(it = list.begin(); it != list.end(); it++){
            REQUIRE(expvalues[index] == **it);
            index += 1;
        }
    }
    
    SUBCASE("BST: nlr"){
        int expvalues[] = {50, 20, 15, 7, 12, 18, 80, 60, 90, 85};
        DLinkedList<int*> list = tree.nlr();
        
        DLinkedList<int*>::Iterator it;
        int index=0;
        for(it = list.begin(); it != list.end(); it++){
            REQUIRE(expvalues[index] == **it);
            index += 1;
        }
    }
    SUBCASE("BST: lrn"){
        int expvalues[] = {12, 7, 18, 15, 20, 60, 85, 90, 80, 50};
        DLinkedList<int*> list = tree.lrn();
        
        DLinkedList<int*>::Iterator it;
        int index=0;
        for(it = list.begin(); it != list.end(); it++){
            REQUIRE(expvalues[index] == **it);
            index += 1;
        }
    }
    SUBCASE("BST: dfs"){
        int expvalues[] = {50, 20, 15, 7, 12, 18, 80, 60, 90, 85};
        DLinkedList<int*> list = tree.dfs();
        
        DLinkedList<int*>::Iterator it;
        int index=0;
        for(it = list.begin(); it != list.end(); it++){
            REQUIRE(expvalues[index] == **it);
            index += 1;
        }
    }
    SUBCASE("BST: bfs"){
        int expvalues[] = {50, 20, 80, 15, 60, 90, 7, 18, 85, 12};
        DLinkedList<int*> list = tree.bfs();
        
        DLinkedList<int*>::Iterator it;
        int index=0;
        for(it = list.begin(); it != list.end(); it++){
            REQUIRE(expvalues[index] == **it);
            index += 1;
        }
    }
}

#endif /* BSTTEST_H */

