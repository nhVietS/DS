/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
#include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////


template<class T>
class UGraphModel: public AbstractGraph<T>{
private:
public:
    class UGraphAlgorithm;
    friend class UGraphAlgorithm;
    
    UGraphModel(
            bool (*vertexEQ_PTR)(T&, T&)=0, 
            string (*vertex2str_PTR)(T&)=0): 
        AbstractGraph<T>(vertexEQ_PTR, vertex2str_PTR){
    }
    
    void connect(T from, T to, float weight=0){
        typename AbstractGraph<T>::VertexNode* nodeF = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* nodeT = this->getVertexNode(to);
        if(nodeF == 0) throw VertexNotFoundException(this->vertex2Str(*nodeF));
        if(nodeT == 0) throw VertexNotFoundException(this->vertex2Str(*nodeT));
        
        nodeF->connect(nodeT, weight);
        nodeT->connect(nodeF, weight);
    }
    void disconnect(T from, T to){
        typename AbstractGraph<T>::VertexNode* nodeF = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* nodeT = this->getVertexNode(to);
        if(nodeF == 0) throw VertexNotFoundException(this->vertex2Str(*nodeF));
        if(nodeT == 0) throw VertexNotFoundException(this->vertex2Str(*nodeT));
        
        typename AbstractGraph<T>::Edge* edge = nodeF->getEdge(nodeT);
        if(edge == 0) throw EdgeNotFoundException(this->edge2Str(*edge));
        nodeF->removeTo(nodeT);
        nodeT->removeTo(nodeF); 
    }
    void remove(T vertex){
        typename AbstractGraph<T>::VertexNode* nodeF = this->getVertexNode(vertex);
        if(nodeF == 0) throw VertexNotFoundException(this->vertex2Str(*nodeF));
        //disconnect all
        
        typename DLinkedList<typename AbstractGraph<T>::VertexNode*>::Iterator nodeIt;
        nodeIt = this->nodeList.begin();
        while(nodeIt != this->nodeList.end()){
            typename AbstractGraph<T>::VertexNode* nodeT = *nodeIt;
            typename AbstractGraph<T>::Edge* edge = nodeF->getEdge(nodeT);
            if(edge != 0){
                nodeF->removeTo(nodeT);
                nodeT->removeTo(nodeF);
            }
            //next
            nodeIt++;
        }
        
        //remove
        this->nodeList.removeItem(nodeF);
    }
};


//////////////////////////////////////////////////////////////////////
//////////////////// Algorithms with UGraphModel  ////////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
class UGraphAlgorithm{
private:
public:
    UGraphAlgorithm(){
    }
    
    UGraphModel<T> minSpanningTree(UGraphModel<T>* pGraph){
        //(1) Obtain the list ot vertices being processed
        DLinkedList<T> vertexList;
        typename AbstractGraph<T>::Iterator vertexIt = pGraph->begin();
        while(vertexIt != pGraph->end()){
            T vertex = *vertexIt;
            vertexList.add(vertex);
            
            vertexIt++;
        }
        
        //(2) Process each vertex in vertexList
        UGraphModel<T> mst(pGraph->getVertexEQ(), pGraph->getVertex2Str());
        while(!vertexList.empty()){
            T vertex = vertexList.removeAt(0);

            mst.add(vertex);
            bool hasChildren = true;
            do{
                PriorityQueue<Edge*> crossEdges(&Edge::edgeComparator);
                typename AbstractGraph<T>::Iterator mstIt = mst.begin();
                while(mstIt != mst.end()){
                    T parent = *mstIt;
                    
                    DLinkedList<T> children = pGraph->getOutwardEdges(parent);
                    typename DLinkedList<T>::Iterator childrenIt = children.begin();
                    while(childrenIt != children.end() ){
                        T child = *childrenIt;
                        
                        if(!mst.contains(child)){
                            float weight = pGraph->weight(parent, child);
                            Edge* edge = new Edge(parent, child, weight);
                            crossEdges.push(edge);
                        }
                        //next
                        childrenIt++;
                    }//while:childrenIt
                    
                    //next
                    mstIt++;
                }//while: mstIt
                
                hasChildren = crossEdges.size() > 0;
                if(hasChildren){
                    Edge* smallest = crossEdges.pop();
                    mst.add(smallest->to);
                    mst.connect(smallest->from, smallest->to, smallest->weight);
                    vertexList.removeItem(smallest->to);
                }
            } while(hasChildren);
        }

        return mst;
    }
    
    
    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
    
    class Edge{
    private:
        T from;
        T to;
        float weight;
        
        //friend class UGraphModel;
        friend class UGraphAlgorithm;
        
    public:
        Edge(T from, T to, float weight){
            this->from = from;
            this->to = to;
            this->weight = weight;
        }
        
        static int edgeComparator(Edge*& edge1, Edge*& edge2){
            float diff = edge1->weight - edge2->weight;
            if(diff < 0) return -1;
            else if(diff > 0) return +1;
            else return 0;
        }
    };
    
};

#endif /* UGRAPHMODEL_H */

