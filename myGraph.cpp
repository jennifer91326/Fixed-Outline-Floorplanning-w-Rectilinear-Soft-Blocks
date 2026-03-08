#include "myGraph.h"
#include "parser.h"
#include <bits/stdc++.h>
#include <stdio.h>
using namespace std;
vector<connection_struct> connectOverAvgWL_vec;
vector<wire_graph*> subGraph;
vector<connection_struct> connectLowerAvgWL_vec;

void create_graph() {
    wire_graph* overall = new wire_graph;

    for (auto& n : SoftModule_map){
        overall->add_vertex(n.first);
    }

    // 建立總graph：添加edge & 分割 connection_vec
    for (auto& k : softConnection_vec){ // 遍歷connection_vec
        if (k.num > avgWL) {        // 如果連線數目大於平均值
            connectOverAvgWL_vec.push_back(k);
            overall->add_edge(k.name1, k.name2, k.num);
        }
        else connectLowerAvgWL_vec.push_back(k);
    }

    // 建立總graph：刪無相連的vertex & 編號（為了BFS）
    overall->deleteUselessVertex();

//    ofs << "total graph: " << endl;
//    printGraph(*overall);

    // 建立sub graph
    overall->CCBFS(0);

}

void printGraph(wire_graph const &graph)
{
//    for (auto& n :graph.adjList)
//    {
//        // 印出當前頂點名字&編號
//        ofs << n.first << " " << n.second.vertex_data << " --> ";
//
//        // 印出當前頂點的所有相鄰頂點
//        Edge_node *temp = n.second.first_edge_node;
//        while (temp != nullptr) {
//          ofs << "( " << temp->vertex_name << ", " << temp->edge_data << " ) ";
//          temp = temp->next;
//        }
//        ofs << endl;
//    }
}

/* 添加vertex */
void wire_graph:: add_vertex(string vertex_name)
{
    adjList[vertex_name] = Vertex_node(vertex_name);
}

/* 添加edge */
bool wire_graph:: add_edge( string v1, string v2, int edge_data)
{
    // 添加edge前需確保兩個vertex已添加
    // 為v1添加與v2的edge
    Edge_node* p = new Edge_node(v2, edge_data);
    if (!p) {
        return false;
    }
    // 插在最前面
    p->next = adjList[v1].first_edge_node;
    adjList[v1].first_edge_node = p;

    // 為v2添加與v1的edge
    p = new Edge_node(v1, edge_data);
    if (!p) {
        return false;
    }
    p->next = adjList[v2].first_edge_node;
    adjList[v2].first_edge_node = p;
    return true;
}

/* 刪無相連的vertex */
void wire_graph:: deleteUselessVertex()
{
    int id_count=0;
    for (auto iter=adjList.begin(); iter!=adjList.end();) {
        if (iter->second.first_edge_node == nullptr) { //無相連的vertex
            iter = adjList.erase(iter);
        }
        else { // 有相連的vertex
            adjList[iter->first].vertex_data = id_count; // 編號
            iter++;
            id_count++;
        }
    }
    num_vertex = id_count;
}

/* 編號 */
void wire_graph:: serialVertex()
{
    int id_count=0;
    for (auto iter=adjList.begin(); iter!=adjList.end();) {
        adjList[iter->first].vertex_data = id_count; // 編號
        id_count++;
    }
    num_vertex = id_count;
}

/* BFS */
void wire_graph:: BFS(int Start)
{
    visit = new Vertex_node[num_vertex];
    predecessor = new Vertex_node[num_vertex];
    distance = new Vertex_node[num_vertex];

    auto itr_track = adjList.begin();
    for (int i = 0; i < num_vertex; i++) {  // 初始化
        visit[i].vertex_data = 0;                       // 0:沒有被找過;
        visit[i].vertex_name = itr_track->first;
        predecessor[i].vertex_data = -1;                // -1表示沒有predecessor
        predecessor[i].vertex_name = itr_track->first;
        distance[i].vertex_data = num_vertex+1;         // 最長距離 distance = num_vertex -1條edge
        itr_track++;
    }

    queue<Vertex_node> q;
    int i = Start;

    for (int j = 0; j < num_vertex; j++) {              // 遍歷graph中所有vertex
        if (visit[i].vertex_data == 0) {                // 第一次i會是起點vertex
            visit[i].vertex_data = 1;                   // 1:已經被某個vertex找過
            distance[i].vertex_data = 0;                // 每一個connected component 的起點之距離設成0
            predecessor[i].vertex_data = -1;            // 每一個connected component 的起點沒有predecessor
            Vertex_node temp; temp.vertex_data=i; temp.vertex_name=visit[i].vertex_name;
            q.push(temp);

            while (!q.empty()) {
                Vertex_node u = q.front();              // u 為新的搜尋起點
                // 遍歷 vertex u 的 list
                Edge_node *temp_list = adjList[u.vertex_name].first_edge_node;
                while (temp_list != nullptr) {
                    Vertex_node iter_node = adjList[temp_list->vertex_name];
                    int itr = iter_node.vertex_data;
                    if (visit[itr].vertex_data == 0) {  // 若被「找到」的vertex沒有被找過
                        visit[itr].vertex_data = 1;     // 將其標示已經被「找到」
                        distance[itr].vertex_data = distance[u.vertex_data].vertex_data + 1;  // 距離是predecessor之距離加一
                        predecessor[itr].vertex_data = u.vertex_data;             // 更新被「找到」的vertex的predecessor
                        q.push(iter_node);              // 把vertex推進queue
                    } // end if
                    temp_list = temp_list->next;
                } // end list while
                q.pop();        // 把u移出queue
                visit[u.vertex_data].vertex_data = 2;   // 將u標示已經從queue中移除
            }
        }
        // 若一次回圈沒有把所有vertex走過, 表示graph有多個connected component
        // 就把i另成j, 繼續檢查graph中的其他vertex是否仍是白色, 若是, 重複while loop
        i = j;
    }
}

/* Print Predecessor(記錄某個vertex是被哪一個vertex找到) */
void wire_graph:: PrintPredecessor()
{
//    ofs << endl << "predecessor:" << endl;
//
//    for (int i = 0; i < num_vertex; i++)
//        ofs << setw(4) << visit[i].vertex_name;
//    ofs << endl;
//
//    for (int i = 0; i < num_vertex; i++)
//        ofs << setw(4) << predecessor[i].vertex_data ;
//    ofs << endl;
}

/* 讓尋找connected component變得更簡單 */
void wire_graph:: SetCollapsing(int current)
{
    int root;
    for (root = current; predecessor[root].vertex_data >= 0; root = predecessor[root].vertex_data);

    while (current != root) {
        int parent = predecessor[current].vertex_data ;
        predecessor[current].vertex_data = root;
        current = parent;
    }
}

/* 找 connected component */
void wire_graph:: CCBFS(int vertex){
    BFS(vertex);
//    PrintPredecessor();
    for (int i = 0; i< num_vertex; i++){
        SetCollapsing(i);
    }
//    PrintPredecessor();

    // 創 subGraph
    for (int i = 0; i < num_vertex; i++) {
        if (predecessor[i].vertex_data < 0) {
            wire_graph* tempGraph = new wire_graph;
            // 添加vertex
            tempGraph->add_vertex(predecessor[i].vertex_name);
            for (int j = 0; j < num_vertex; j++) {
                if (predecessor[j].vertex_data == i) {
                    tempGraph->add_vertex(predecessor[j].vertex_name);
                }
            }
            // 添加edge
            for (auto& n : connectOverAvgWL_vec) {
                auto itr = tempGraph->adjList.find(n.name1); // 找此連線是否在此子graph
                if (itr != tempGraph->adjList.end()) { // 有找到
                    tempGraph->add_edge(n.name1, n.name2, n.num);
                }
            }
            tempGraph->deleteUselessVertex();
            subGraph.push_back(tempGraph);
        }
    }
}

/* Prim's alg 生成最大生成树 */
void wire_graph::create_MST()
{
    int max;        //最大權重
    int maxIndex;   //最大權重邊的index
    Vertex_node* isAdded = new Vertex_node[num_vertex];
    vector<Edge_node*> maxCost;
    Vertex_node* adjVex = new Vertex_node[num_vertex];
    MST = new Tree();

    auto itr_track = adjList.begin();
    for (int i=0; i<num_vertex; i++) { //初始化 isAdded 0:還沒加入 1:加入了
        isAdded[i].vertex_data = 0;
        isAdded[i].vertex_name = itr_track->first;
        maxCost.push_back(nullptr);
        itr_track++;
    }

    Edge_node* temp = adjList.begin()->second.first_edge_node; //第一個vertex開始
    isAdded[0].vertex_data = 1;
    for(auto& n : adjList) {              //adjVex & maxCost初始化
        adjVex[n.second.vertex_data] = adjList.begin()->second; //指向第一個vertex
        if(temp!=nullptr) {
            maxCost[ adjList[temp->vertex_name].vertex_data ] = temp;
            temp = temp->next;
        }
    }

    for(int i=0; i<num_vertex-1; i++) {
        max = 0;
        //首先找到maxCost中權重最大的邊以及對應的非mst vertex id
        for(int k = 0; k < num_vertex; k++) {
            if(maxCost[k] != nullptr && maxCost[k]->edge_data > max) {
                max = maxCost[k]->edge_data;
                maxIndex = k;
            }
        }

        //打印
//        printf("(%s, %s)%d - ", adjVex[maxIndex].vertex_name.c_str(), isAdded[maxIndex].vertex_name.c_str(), max);
//        printf("(%d, %d)%d - ", adjVex[maxIndex].vertex_data, maxIndex, max);

        /* 建樹 */

        string v1_name = adjVex[maxIndex].vertex_name.c_str(); int v1_id = adjVex[maxIndex].vertex_data;
        string v2_name = isAdded[maxIndex].vertex_name.c_str(); int v2_id = maxIndex;
        MST->AddChild(v1_id, v2_id);
        MST->SetName(v1_id, v1_name);
        MST->SetName(v2_id, v2_name);
        MST->AddEdge(v1_name, v2_name, max);


        isAdded[maxIndex].vertex_data = 1; //標示v_maxIndex已經加入到MST中
        maxCost[maxIndex] = nullptr; //v_maxIndex已經加入到MST中，其與mst的距離為0（標示為NULL)
        //MST中新加了一個vertex，所有的更新都與這個vertex相關，因此用與依附於這個vertex的所有edge進行更新
        temp = adjList[ isAdded[maxIndex].vertex_name ].first_edge_node;
        while(temp) {
            int temp_nextVertexID = adjList[temp->vertex_name].vertex_data;
            //更新: 找到一條更大的路徑，因此對方vertex不在MST中並且權值更大就可以更新
            if(isAdded[temp_nextVertexID].vertex_data != 1
               && (maxCost[temp_nextVertexID] == nullptr
                   || temp->edge_data > maxCost[temp_nextVertexID]->edge_data)) {
                maxCost[temp_nextVertexID] = temp; //更新maxCost
                adjVex[temp_nextVertexID] = adjList[ isAdded[maxIndex].vertex_name ];  //同時更新adjVex
            }
            temp = temp->next;
        } // end while
    }
//    ofs << endl;
} // end function
