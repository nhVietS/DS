/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HeapDemo.h
 * Author: LTSACH
 *
 * Created on 22 August 2020, 18:30
 */

#ifndef HEAPDEMO_H
#define HEAPDEMO_H

#include <math.h>
#include <string>
#include <sstream>


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

int pointMinComparator(Point& a, Point& b){
    float ra = sqrt(a.getX()*a.getX() + a.getY()*a.getY());
    float rb = sqrt(b.getX()*b.getX() + b.getY()*b.getY());
    if (ra < rb) return -1;
    else if(ra > rb) return +1;
    else return 0;
}
int pointMaxComparator(Point& a, Point& b){
    float ra = sqrt(a.getX()*a.getX() + a.getY()*a.getY());
    float rb = sqrt(b.getX()*b.getX() + b.getY()*b.getY());
    if (ra < rb) return +1;
    else if(ra > rb) return -1;
    else return 0;
}
string pointstr(Point& p){
    stringstream os;
    os << "(" 
       << p.getX() << ","
       << p.getY()
       << ")";
    return os.str();
}
int pointPtrMinComparator(Point*& a, Point*& b){
    float ra = sqrt(a->getX()*a->getX() + a->getY()*a->getY());
    float rb = sqrt(b->getX()*b->getX() + b->getY()*b->getY());
    if (ra < rb) return -1;
    else if(ra > rb) return +1;
    else return 0;
}
int pointPtrMaxComparator(Point*& a, Point*& b){
    float ra = sqrt(a->getX()*a->getX() + a->getY()*a->getY());
    float rb = sqrt(b->getX()*b->getX() + b->getY()*b->getY());
    if (ra < rb) return +1;
    else if(ra > rb) return -1;
    else return 0;
}

string pointPtrStr(Point*& p){
    stringstream os;
    os << "(" 
       << p->getX() << ","
       << p->getY()
       << ")";
    return os.str();
}

void heapDemo1(){
    int array[] = {50, 20, 15, 10, 8, 6, 7, 23}; 
    //min heap: [6, 10, 7, 23, 15, 20, 8, 50]
    //max heap: [50, 23, 15, 20, 8, 6, 7, 10]
    Heap<int> minHeap1;
    cout << "Min Heap: ";
    minHeap1.heapify(array, 8);
    cout << minHeap1.toString() << endl;
    
    Heap<int> minHeap2(minComparator);
    cout << "Min Heap: ";
    minHeap2.heapify(array, 8);
    cout << minHeap2.toString() << endl;
    
    Heap<int> maxHeap(maxComparator);
    cout << "Max Heap: ";
    cout << maxHeap.toString() << endl;
    maxHeap.heapify(array, 8);
    cout << maxHeap.toString() << endl;
}

void heapDemo2(){
    Point array[] = {
        Point(13.1, 12.4), Point(5.5, 4.5), Point(15.5, 14.5),
        Point(23.1, 12.4), Point(35.5, 14.5), Point(5.5, 34.5)
    };
    Heap<Point> minHeap1(pointMinComparator, 0, pointstr);
    cout << "Min Heap: ";
    minHeap1.heapify(array, 6);
    cout << minHeap1.toString() << endl;
    
    Heap<Point> maxHeap(pointMaxComparator, 0, pointstr);
    cout << "Max Heap: ";
    maxHeap.heapify(array, 6);
    cout << maxHeap.toString() << endl;
}

void heapDemo3(){
    Point* array[] = {
        new Point(13.1, 12.4), new Point(5.5, 4.5), new Point(15.5, 14.5),
        new Point(23.1, 12.4), new Point(35.5, 14.5), new Point(5.5, 34.5)
    };
    Heap<Point*> minHeap1(pointPtrMinComparator, Heap<Point*>::free, pointPtrStr);
    cout << "Min Heap: ";
    minHeap1.heapify(array, 6);
    cout << minHeap1.toString() << endl;
    
    Heap<Point*> maxHeap(pointPtrMaxComparator, 0 , pointPtrStr); //minHeap1 has Heap<Point*>::free => pass 0 here
    cout << "Max Heap: ";
    maxHeap.heapify(array, 6);
    cout << maxHeap.toString() << endl;
}


#endif /* HEAPDEMO_H */

