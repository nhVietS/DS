/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:48
 */

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H
#include "graph/IGraph.h"
#include <string>
#include <sstream>
using namespace std;


template<class T>
class AbstractGraph{
public:
    class Edge; //forward declaration
    class VertexNode; //forward declaration
    class Iterator; //forward declaration
    
private:
protected:
    //Using the adjacent list technique, so need to store list of nodes (nodeList)
    DLinkedList<VertexNode*> nodeList; 
    
    //Function pointers:
    bool (*vertexEQ_PTR)(T&, T&); //to compare two vertices
    string (*vertex2str_PTR)(T&); //to obtain string representation of vertices
    
    
    VertexNode* getVertexNode(T& vertex){
        typename DLinkedList<VertexNode*>::Iterator it = nodeList.begin();
        while(it != nodeList.end()){
            VertexNode* node = *it;
            if(vertexEQ_PTR(node->vertex, vertex) ) return node;
            it++;
        }
        return 0;
    }
    string vertex2Str(VertexNode& node){
        return vertex2str_PTR(node.vertex);
    }
    string edge2Str(Edge& edge){
        stringstream os;
        os << "E("
                << vertex2str_PTR(edge.from->vertex)
                << ","
                << vertex2str_PTR(edge.to->vertex)
                << ")";
        return os.str();
    }
    
public:
    AbstractGraph(
            bool (*vertexEQ_PTR)(T&, T&)=0, 
            string (*vertex2str_PTR)(T&)=0){
        
        this->vertexEQ_PTR = vertexEQ_PTR;
        this->vertex2str_PTR = vertex2str_PTR;
    }
    virtual ~AbstractGraph(){}
    
    typedef bool (*vertexEQFunc)(T&, T&);
    typedef string (*vertex2strFunc)(T&);
    vertexEQFunc getVertexEQ(){
        return this->vertexEQ_PTR;
    }
    vertex2strFunc getVertex2Str(){
        return this->vertex2str_PTR;
    }

    
//////////////////////////////////////////////////////////////////////
//////////////////// IMPLEMENTATION of IGraph API ////////////////////
//////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight=0) =0;
    virtual void disconnect(T from, T to)=0;
    virtual void remove(T vertex)=0;
    
    
    
    /* The following are common methods for UGraphModel and DGraphModel
     */
    virtual void add(T vertex) {
        this->nodeList.add(new VertexNode(vertex, vertexEQ_PTR));
    }
    virtual bool contains(T vertex){
        VertexNode* node = getVertexNode(vertex);
        return node != 0;
    }
    virtual float weight(T from, T to){
        VertexNode* nodeF = getVertexNode(from);
        VertexNode* nodeT = getVertexNode(to);
        if(nodeF == 0) throw VertexNotFoundException(vertex2Str(*nodeF));
        if(nodeT == 0) throw VertexNotFoundException(vertex2Str(*nodeT));
        
        Edge* edge = nodeF->getEdge(nodeT);
        if(edge == 0) throw EdgeNotFoundException(edge2Str(*edge));
        
        return edge->weight;
    }
    virtual DLinkedList<T> getOutwardEdges(T from){
        VertexNode* node = getVertexNode(from);
        if(node == 0) throw VertexNotFoundException(vertex2Str(*node));
        return node->getOutwardEdges();
    }
    
    virtual DLinkedList<T>  getInwardEdges(T to){
        VertexNode* nodeT = getVertexNode(to);
        if(nodeT == 0) throw VertexNotFoundException(vertex2Str(*nodeT));
        
        DLinkedList<T> list;
        
        typename DLinkedList<VertexNode*>::Iterator nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            
            typename DLinkedList<Edge*>::Iterator edgeIt = node->adList.begin();
            while(edgeIt != node->adList.end()){
                Edge* edge = *edgeIt;
                if(edge->to->equals(nodeT)) list.add(edge->from->vertex);
                //next
                edgeIt++;
            }
            //next
            nodeIt++;
        }
        return list;
    }
    
    virtual int size() {
        return this->nodeList.size();
    }
    virtual bool empty(){
        return this->size() == 0;
    };
    virtual void clear(){
        while(!this->nodeList.empty()){
            remove(this->nodeList.get(0)->vertex);
        }
    }
    virtual int inDegree(T vertex){
        VertexNode* node = getVertexNode(vertex);
        if(node == 0) throw VertexNotFoundException(vertex2Str(*node));
        return node->inDegree();
    }
    virtual int outDegree(T vertex){
        VertexNode* node = getVertexNode(vertex);
        if(node == 0) throw VertexNotFoundException(vertex2Str(*node));
        return node->outDegree();
    }
    void println(){
        cout << this->toString() << endl;
    }
    virtual string toString(){
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;
        typename DLinkedList<VertexNode*>::Iterator nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }
        string mark2(30, '-');
        os << mark2 << endl;
        
        os << left << setw(12) << "Edges:" << endl;
        
        nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()){
            VertexNode* node = *nodeIt;
            
            typename DLinkedList<Edge*>::Iterator edgeIt = node->adList.begin();
            while(edgeIt != node->adList.end()){
                Edge* edge = *edgeIt;
                os << edge->toString() << endl;
                
                edgeIt++;
            }
            
            nodeIt++;
        }
        os << mark << endl;
        return os.str();
    }
    
    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it; 
     *          //continue to process vertex here!
     *      }
     */
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }

//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////

public:
//BEGIN of VertexNode    
    class VertexNode{
    private:
        T vertex;
        int inDegree_, outDegree_;
        DLinkedList<Edge*> adList; 
        friend class Edge;
        friend class AbstractGraph;
        
        bool (*vertexEQ_PTR)(T&, T&);
    public:
        VertexNode(){}
        VertexNode(T vertex, bool (*vertexEQ)(T&, T&)){
            this->vertex = vertex;
            this->outDegree_ = this->inDegree_ = 0;
            this->vertexEQ_PTR = vertexEQ;
        }
        T& getVertex(){
            return vertex;
        }
        static bool edgeEQ(Edge* edge1, Edge* edge2){
            return edge1->from->equals(edge2->from) && 
                    edge1->to->equals(edge2->to);
        }
        void connect(VertexNode* to, float weight=0){
            Edge* edge = getEdge(to);
            if(edge == 0){
                edge = new Edge(this, to, weight);
                this->adList.add(edge);
                edge->from->outDegree_ += 1;
                edge->to->inDegree_ += 1;
            }
            else edge->weight = weight;
        }
        DLinkedList<T> getOutwardEdges(){
            DLinkedList<T> list;
            typename DLinkedList<Edge*>::Iterator it = this->adList.begin();
            while(it != this->adList.end()){
                Edge *edge = *it;
                list.add(edge->to->vertex);
                it++;
            }
            return list;
        }

        Edge* getEdge(VertexNode* to){
            typename DLinkedList<Edge*>::Iterator it = this->adList.begin();
            while(it != adList.end()){
                Edge* edge = *it;
                if(edge->from->equals(this) && edge->to->equals(to) ) return edge;
                it++;
            }
            return 0;
        }
        bool equals(VertexNode* node){
            return this->vertexEQ_PTR(this->vertex, node->vertex);
        }
        
        void removeTo(VertexNode* to){
            Edge* pEdge = new Edge(this, to);
            this->adList.removeItem(pEdge);
            this->outDegree_ -= 1;
            to->inDegree_ -= 1;
                
            delete pEdge;
        }
        int inDegree(){
            return this->inDegree_;
        }
        int outDegree(){
            return this->outDegree_;
        }
        string toString(){
            stringstream os;
            os << "V("
                    << this->vertex << ", "
                    << "in: " << this->inDegree_ << ", "
                    << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };
//END of VertexNode    
    
//BEGIN of Edge
    class Edge{
    private:
        VertexNode* from;
        VertexNode* to;
        float weight;
        friend class VertexNode;
        friend class AbstractGraph;
        
        
    public:
        Edge(){}
        Edge(VertexNode* from, VertexNode* to, float weight=0){
            this->from = from;
            this->to = to;
            this->weight = weight;
        }
        bool equals(Edge* edge){
            return this->from->equals(edge->from) && this->to->equals(edge->to);
        }
        string toString(){
            stringstream os;
            os << "E("
                    << this->from->vertex
                    << ","
                    << this->to->vertex
                    << ","
                    << this->weight
                    << ")";
            return os.str();
        }
    };
 //END of Edge
    

//BEGIN of Iterator
public:
    class Iterator{
    private:
        typename DLinkedList<VertexNode*>::Iterator nodeIt;
        
    public:
        Iterator(AbstractGraph<T>* pGraph=0, bool begin=true){
            if(begin) {
                if(pGraph != 0) nodeIt = pGraph->nodeList.begin();
            }
            else{
                if(pGraph != 0) nodeIt = pGraph->nodeList.end();
            }
        }
        Iterator& operator=(const Iterator& iterator){
            this->nodeIt = iterator.nodeIt;
            return *this;
        }
        
        T& operator*(){
            return (*nodeIt)->vertex;
        }
        
        bool operator!=(const Iterator& iterator){
            return nodeIt != iterator.nodeIt;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            nodeIt++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
    };
//END of Iterator
};

#endif /* ABSTRACTGRAPH_H */

