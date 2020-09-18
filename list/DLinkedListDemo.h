/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DLinkedListDemo.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 21:34
 */

#ifndef DLINKEDLISTDEMO_H
#define DLINKEDLISTDEMO_H

#include <iostream>
#include <iomanip>
#include "list/DLinkedList.h"
#include "util/Point.h"
using namespace std;

void intDListDemo1(){
    List<int> dlist;
    for(int i = 0; i< 20 ; i++)
        dlist.add(i, i*i);
    //dlist.dump();
    
    for(List<int>::Iterator it=dlist.begin(); it != dlist.end(); it++ )
        cout << *it << " ";
    cout << endl;
}
void pointDListDemo1(){
    DLinkedList<Point*> list1(&DLinkedList<Point*>::free, &Point::pointPtrEQ);
    list1.add(new Point(23.2f, 25.4f));
    list1.add(new Point(24.6f, 23.1f));  
    list1.add(new Point(12.5f, 22.3f)); 
    
    for(DLinkedList<Point*>::Iterator it = list1.begin(); it != list1.end(); it++)
        cout << **it << endl;
    
    Point* p1 = new Point(24.6f, 23.1f); //found in list
    Point* p2 = new Point(124.6f, 23.1f); //not found
    cout << *p1 << "=> " << (list1.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list1.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list1.indexOf(p2) << endl;
    
    ///Different results if not pass &Point::equals
    cout << endl << endl;
    DLinkedList<Point*> list2(&DLinkedList<Point*>::free);
    list2.add(new Point(23.2f, 25.4f));
    list2.add(new Point(24.6f, 23.1f));  
    list2.add(new Point(12.5f, 22.3f)); 
    
    for(DLinkedList<Point*>::Iterator it = list2.begin(); it != list2.end(); it++)
        cout << **it << endl;
    
    cout << *p1 << "=> " << (list2.contains(p1)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p1) << endl;
    cout << *p2 << "=> " << (list2.contains(p2)? "found; " : "not found; ")
                << " indexOf returns: " << list2.indexOf(p2) << endl;
    
    delete p1; delete p2;
}

void pointList1(){
    DLinkedList<Point> dList;
    dList.add(Point(1.5, 3.5));
    dList.add(Point(2.5, 4.5));
    dList.add(Point(1.6, 3.1));
    dList.dump();
    
    cout << "test for indexOf: " << endl;
    Point p(1.6, 3.1);
    cout << p << " at: " << dList.indexOf(p);
}
bool pointComparator(Point*& p1, Point*& p2){
    return (p1->getX() == p2->getX()) && (p1->getY() == p2->getY());
}
void pointList2(){
    DLinkedList<Point*> dList(&DLinkedList<Point*>::free, &pointComparator);
    dList.add(new Point(1.5, 3.5));
    dList.add(new Point(2.5, 4.5));
    dList.add(new Point(1.6, 3.1));
    dList.dump();
    
    DLinkedList<Point*>::Iterator it = dList.begin();
    cout << "[";
    while(it != dList.end()){
        cout << **it << ",";
        it++;
    }
    cout << "]" << endl;
    
    cout << "test for indexOf: " << endl;
    Point* p = new Point(1.6, 3.1);
    cout << *p << " at: " << dList.indexOf(p);
    delete p;
}
void pointList3(){
    DLinkedList<float> dList;
    dList.add(3.2);
    dList.add(5.5);
    dList.dump();
    cout << "index of 5.5: " << dList.indexOf(5.5) << endl;
    cout << "index of 15.5: " << dList.indexOf(15.5) << endl;
}
void pointList4(){
    DLinkedList<float*> dList;
}
#endif /* DLINKEDLISTDEMO_H */

