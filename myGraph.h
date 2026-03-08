
#ifndef myGraph_h
#define myGraph_h
#include <bits/stdc++.h>
#include "TREE.h"
using namespace std;

// edge節點
class Edge_node {
public:
    string vertex_name;         // 這條edge連接的vertex name 像module名稱
    int edge_data;                 // edge的數據 像權重、距離？
    Edge_node* next{ nullptr };
    Edge_node() = default;
    Edge_node(string v_name, int edge_info)
    {
        this->vertex_name = v_name;
        this->edge_data = edge_info;
        next = nullptr;
    }
};

// vertex節點
class Vertex_node {
public:
    string vertex_name;  // 當前vertex name
    int vertex_data;
    Edge_node* first_edge_node{ nullptr };
    Vertex_node() = default;
    Vertex_node(string v_name)
    {
        this->vertex_name = v_name;
    }
};

// graph
class wire_graph {
public:
    map<string, Vertex_node> adjList;
    Vertex_node *visit,             // 0:沒有被找過, 1:已經被某個vertex找過, 2:已經從queue中移除
                *distance,          // 0:起點, 無限大:從起點走不到的vertex
                *predecessor;       // 記錄某個vertex是被哪一個vertex找到 -1:沒有predecessor, 表示為起點vertex
    Tree* MST;
    int num_vertex;
    wire_graph() = default;

    /* 添加vertex */
    void add_vertex(string vertex_name);
    
    /* 添加edge */
    bool add_edge( string v1, string v2, int edge_data);
    
    /* 刪無相連的vertex */
    void deleteUselessVertex();

    /* 編號 */
    void serialVertex();
    
    /* BFS */
    void BFS(int Start);
    
    /* Print Predecessor(記錄某個vertex是被哪一個vertex找到) */
    void PrintPredecessor();

    /* 讓尋找connected component變得更簡單 */
    void SetCollapsing(int current);

    /* 找 connected component */
    void CCBFS(int vertex);

    /* Prim's alg 生成最大生成树 */
    void create_MST();

}; // end class wire_graph

extern vector<wire_graph*> subGraph;
void printGraph(wire_graph const &graph);
void PlacePosition(wire_graph const &subGraph);
void create_graph();
#endif /* myGraph_h */
