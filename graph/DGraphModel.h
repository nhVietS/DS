/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
#include "hash/XHashMap.h"
#include "stacknqueue/PriorityQueue.h"


//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////


template<class T>
class DGraphModel: public AbstractGraph<T>{
private:
public:
    DGraphModel(
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
    }
    void disconnect(T from, T to){
        typename AbstractGraph<T>::VertexNode* nodeF = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* nodeT = this->getVertexNode(to);
        if(nodeF == 0) throw VertexNotFoundException(this->vertex2Str(*nodeF));
        if(nodeT == 0) throw VertexNotFoundException(this->vertex2Str(*nodeT));
        
        typename AbstractGraph<T>::Edge* edge = nodeF->getEdge(nodeT);
        if(edge == 0) throw EdgeNotFoundException(this->edge2Str(*edge));
        nodeF->removeTo(nodeT);
    }
    void remove(T vertex){
        typename AbstractGraph<T>::VertexNode* nodeA = this->getVertexNode(vertex);
        if(nodeA == 0) throw VertexNotFoundException(this->vertex2Str(*nodeA));
        //disconnect all
        
        typename DLinkedList<typename AbstractGraph<T>::VertexNode*>::Iterator nodeIt = this->nodeList.begin();
        while(nodeIt != this->nodeList.end()){
            typename AbstractGraph<T>::VertexNode* nodeB = *nodeIt;
            typename AbstractGraph<T>::Edge* edge = nodeB->getEdge(nodeA);
            if(edge != 0) nodeB->removeTo(nodeA);
            edge = nodeA->getEdge(nodeB);
            if(edge != 0) nodeA->removeTo(nodeB);
            
            nodeIt++;
        }
        
        //remove
        this->nodeList.removeItem(nodeA);
    }
};

//////////////////////////////////////////////////////////////////////
//////////////////// Algorithms with DGraphModel  ////////////////////
//////////////////////////////////////////////////////////////////////
template<class T>
class TopoSorter{
public:
    static int DFS;
    static int BFS; 
    
protected:
    DGraphModel<T>* graph;
    
public:
    TopoSorter(DGraphModel<T>* graph){
        this->graph = graph;
    }   
    DLinkedList<T> sort(int mode=0){
        if(mode == DFS) return dfsSort();
        else return bfsSort();
    }
    DLinkedList<T> bfsSort(){
        DLinkedList<T> topoOrder;
        XHashMap<T, int> indegreeMap = vertex2inDegree();
        DLinkedList<T> list = listOfZeroInDegrees();
        
        Queue<T> queue;
        for(typename DLinkedList<T>::Iterator it= list.begin(); it != list.end(); it++)
            queue.push(*it);
        while(!queue.empty()){
            T vertex = queue.pop();
            topoOrder.add(vertex);
            
            DLinkedList<T> neighbors = this->graph->getOutwardEdges(vertex);
            for(typename DLinkedList<T>::Iterator it=neighbors.begin(); it != neighbors.end(); it++){
                T v = *it;
                int prevDegree = indegreeMap.get(v);
                indegreeMap.put(v, prevDegree-1);
                if(prevDegree == 1) queue.push(v);
            }
        }
        return topoOrder;
    }
    DLinkedList<T> dfsSort(){
        DLinkedList<T> topoOrder;
        XHashMap<T, int> outdegreeMap = vertex2outDegree();
        DLinkedList<T> list = listOfZeroInDegrees();
        
        Stack<T> stack;
        for(typename DLinkedList<T>::Iterator it=list.begin(); it != list.end(); it++){
            stack.push(*it);
        }
        
        while(!stack.empty()){
            T vertex = stack.peek();
            int outDegree = outdegreeMap.get(vertex);
            if(outDegree == 0){
                stack.pop();
                topoOrder.add(0, vertex);
            }
            else{
                DLinkedList<T> neighbors = this->graph->getOutwardEdges(vertex);
                for(typename DLinkedList<T>::Iterator it=neighbors.begin(); it != neighbors.end(); it++){
                    T v = *it;
                    if(stack.contains(v)){
                        stack.remove(v);
                        stack.push(v);
                    }
                    if(!stack.contains(v) && !topoOrder.contains(v) )
                        stack.push(v);
                    outdegreeMap.put(vertex, outDegree - 1);    
                }
            }
        }//while: stack not empty
        return topoOrder;
    }
    
    
protected:
    XHashMap<T, int> vertex2inDegree(){
        XHashMap<T, int> map(&XHashMap<T, int>::simpleHash);
        typename AbstractGraph<T>::Iterator vertexIt = this->graph->begin();
        while(vertexIt != this->graph->end()){
            T vertex = *vertexIt;
            int inDegree = this->graph->inDegree(vertex);
            map.put(vertex, inDegree);
            
            vertexIt++;
        }
        return map;
    }
    XHashMap<T, int> vertex2outDegree(){
        XHashMap<T, int> map(&XHashMap<T, int>::simpleHash);
        typename AbstractGraph<T>::Iterator vertexIt = this->graph->begin();
        while(vertexIt != this->graph->end()){
            T vertex = *vertexIt;
            int outDegree = this->graph->outDegree(vertex);
            map.put(vertex, outDegree);
            
            vertexIt++;
        }
        return map;
    }
    
    DLinkedList<T> listOfZeroInDegrees(){
        DLinkedList<T> list;
        typename AbstractGraph<T>::Iterator vertexIt = this->graph->begin();
        while(vertexIt != this->graph->end()){
            T vertex = *vertexIt;
            int inDegree = this->graph->inDegree(vertex);
            if(inDegree == 0) list.add(vertex);
            
            vertexIt++;
        }
        return list;
    }
}; //TopoSorter
template<class T>
int TopoSorter<T>::DFS = 0;
template<class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////


template<class T>
class DGraphAlgorithm{
public:
    class Node;
    DGraphAlgorithm(){}
    static bool vertexEQ(T& lhs, T& rhs){
        return lhs==rhs;
    }
    static bool nodeEQ(Node*& lhs, Node*& rhs){
        return vertexEQ(lhs->vertex, rhs->vertex);
    }
    static string item2str(T& item){
        stringstream os;
        os << item;
        return os.str();
    }
    DLinkedList<Path<T>*> dijkstra(DGraphModel<T>* pGraph, T start){
        DLinkedList<Node*> explored(0, DGraphAlgorithm<T>::nodeEQ);
        PriorityQueue<Node*> frontier(&Node::compare);
        frontier.push(new Node(0, start, 0, &DGraphAlgorithm<T>::vertexEQ));
        while(!frontier.empty()){
            Node *node = frontier.pop();
            explored.add(node);
            DLinkedList<T> children = pGraph->getOutwardEdges(node->vertex);
            
            typename DLinkedList<T>::Iterator childrenIt = children.begin();
            while(childrenIt != children.end()){
                T child = *childrenIt;
                
                float stepCost = pGraph->weight(node->vertex, child);
                Node* childNode = new Node(node, child, node->pathCost + stepCost, &DGraphAlgorithm<T>::vertexEQ);
                if(!explored.contains(childNode)){
                    bool inFrontier = false;
                    
                    //
                    typename Heap<Node*>::Iterator frontierIt = frontier.begin();
                    while(frontierIt != frontier.end()){
                        Node* frontierNode = *frontierIt;
                        float oldPathCost = frontierNode->pathCost;
                        float newPathCost = childNode->pathCost;
                        if(vertexEQ(frontierNode->vertex, child) ){
                            inFrontier = true;
                            if(newPathCost < oldPathCost){
                                frontierIt.remove();
                                frontier.push(childNode);
                                break;
                            }
                        }
                        
                        frontierIt++;
                    }
                    if(!inFrontier) frontier.push(childNode);
                }//if
                
                childrenIt++;
            }//while: process each child
        }
        return constructPath(explored);
    }
    
    DLinkedList<Path<T>*> constructPath(DLinkedList<Node*> explored){
        
        DLinkedList<Path<T>*> list;
        typename DLinkedList<Node*>::Iterator it = explored.begin();
        while(it != explored.end()){
            Node* node = *it;
            
            Path<T>* path = new Path<T>(&DGraphAlgorithm<T>::item2str);
            Stack<Node*> stack; //more params
            
            path->setCost(node->pathCost);
            while(node != 0){
                stack.push(node);
                node = node->parent;
            }
            while(!stack.empty()){
                Node* item = stack.pop();
                T vertex = item->vertex;
                path->add(vertex);
            }
            list.add(path);
            
            it++;
        }
        return list;
    }
    
    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node{
    private:
        Node* parent;
        T vertex;
        float pathCost;
        bool (*vertexEQ_PTR)(T&,T&);
        
        friend class DGraphAlgorithm;
        
    public:
        Node(   Node* parent, T vertex, float pathCost, 
                bool (*vertexEQ_PTR)(T&,T&)){
            this->parent = parent;
            this->vertex = vertex;
            this->pathCost = pathCost;
            this->vertexEQ_PTR = vertexEQ_PTR;
        }
        bool equals(Node& node){
            return this->vertexEQ_PTR(this->vertex, node.vertex);
        }
        string toString(){
            stringstream os;
            os << this->vertex;
            return os.str();
        }
        static int compare(Node*& lhs, Node*& rhs){
            float diff = lhs->pathCost - rhs->pathCost;
            if(diff < 0) return -1;
            else if(diff > 0) return +1;
            else return 0;
        }
        
    };
};

#endif /* DGRAPHMODEL_H */

