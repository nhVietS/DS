/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Point.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 21:03
 */

#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <iomanip>
#include <math.h>
#include <random>
#include <sstream>
using namespace std;

class Point{
private:
    float x, y;
    friend ostream &operator<<( ostream &os, const Point& point);
    
public:
    Point(float x=0.0, float y=0.0){
        this->x = x;
        this->y = y;
    }

    void setY(float y) {
        this->y = y;
    }

    float getY() const {
        return y;
    }

    void setX(float x) {
        this->x = x;
    }

    float getX() const {
        return x;
    }
    ////
    //with point object
    bool operator==(Point rhs){
        return (x == rhs.x) && (y == rhs.y);
    }
    //with pointer to point
    static bool pointPtrEQ(Point*& lhs, Point*& rhs){
        return (lhs->x == rhs->x) && (lhs->y == rhs->y);
    }
    //with pointer to point
    static bool pointEQ(Point& lhs, Point& rhs){
        return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }
    
    operator float(){
        return sqrt(x*x + y*y);
    }
    
    static Point* genPoints(int size, float minValue=0, float maxValue=1, 
                            bool manualSeed=false, int seedValue=0){
        
        Point* head = new Point[size];
        
        std::default_random_engine* engine;
        if(manualSeed)
            engine = new std::default_random_engine(static_cast<long unsigned int>(seedValue));
        else
            engine = new std::default_random_engine(static_cast<long unsigned int>(time(0)));
        uniform_real_distribution<double> dist(minValue, maxValue);
        
        //
        for(int idx=0; idx < size; idx++){
            float x = dist(*engine);
            float y = dist(*engine);
            head[idx] = Point(x,y);
        }
        delete engine;
        return head;
    }
    static void println(Point* head, int size){
        stringstream os;
        os << "[";
        for(int idx=0; idx < size-1; idx++)
            os << head[idx] << ", ";
        if(size > 0) os << head[size-1] << "]";
        else os << "]";
        cout << os.str() << endl;
        return;
    }
   
};
ostream &operator<<( ostream &os, const Point& point){
    os << "(" << left << setw(4) << setprecision(3) << point.x 
       << "," << right << setw(4) << setprecision(3) << point.y 
       << ")";
    return os;
}





#endif /* POINT_H */

