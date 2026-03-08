#ifndef parser_h
#define parser_h
#include <bits/stdc++.h>
#include "contour.h"
using namespace std;
extern string inputfileName;

//extern string inputfileNumber;
//extern ofstream ofs;

extern int matlab_module_num, nonPlaceNum;
////////////////////////////////////////////////////////////////////////////////////

struct LegalTypeStruct // 記錄合法的外形
{
    int area; // 模組面積
    int h, w;
    float aspect_ratio; // 高寬比
    float rect_ratio; // 矩形比
    float centerX, centerY; // 中心座標
    ContourList* position; // 轉角座標
};

struct WireConnectStruct
{
    string ModuleName; //連到哪個module
    int connectNum; // 連線數目
};

struct SoftModule_struct
{
    string name;
    int minArea_restrict; // 最小面積限制
    vector<LegalTypeStruct> LegalType; // 合法的模組外形
    int chooseLegalType=-1; // 選哪個合法的模組外形
    vector<WireConnectStruct> WireConnect;
    int total_con;
    bool grouped;
    int vertex_id; // 記錄在graph中的id
    bool placed = false;
};

extern map<string, SoftModule_struct> SoftModule_map;

struct FixedModule_struct
{
    string name;
    int x;
    int y;
    vector<CoordinateStruct> coordinate;
    int width;
    int height;
    vector<WireConnectStruct> WireConnect;
};

extern map<string, FixedModule_struct> FixedModule_map;

struct connection_struct
{
    string name1;
    string name2;
    int num;
    bool grouped;
};

extern vector<connection_struct> connection_vec;
extern vector<connection_struct> softConnection_vec;
extern vector<connection_struct> connectOverAvgWL_vec;
extern vector<connection_struct> connectLowerAvgWL_vec;
void parser();

////////////////////////////////////////////////////*
extern int avgWL;

struct GroupVecStruct
{
    string neighborModule;
    vector<string> ModuleNameVec;
    int minarea;
};

extern vector<GroupVecStruct> GroupVec;
extern vector<pair<string,SoftModule_struct>> SoftModule_vec;

bool sort_f(connection_struct a,connection_struct b);
bool sort_f1(WireConnectStruct a,WireConnectStruct b);
bool sort_f2(pair<string,SoftModule_struct> a,pair<string,SoftModule_struct> b);
void weightofwire();
void groupmodule(int quan);
extern float HPWL;
void compute_HPWL();
float rounding(float num, int index);
#endif /* parser_h */

/*
 //////////////////////////////////////////////////////////////////////////////////// LinkedList class 宣告
 // Node class
 class Node
 {
     public:
       int x, y;
       Node *next;

       Node(int x, int y) {
         this->x = x;
         this->y = y;
         this->next = nullptr;
       }
 };

 // Linked list class
 class LinkedList
 {
     private:
       Node *head;

     public:
       LinkedList() {
         this->head = nullptr;
       }

       void insertAtBeginning(int x, int y) {
         Node *newNode = new Node(x, y);
         newNode->next = head;
         head = newNode;
       }

       void insertAtEnd(int x, int y) {
         Node *newNode = new Node(x, y);
         if (head == nullptr) {
           head = newNode;
           return;
         }
         Node *temp = head;
         while (temp->next != nullptr) {
           temp = temp->next;
         }
         temp->next = newNode;
       }

       void deleteAtBeginning() {
         if (head == nullptr) {
           return;
         }
         Node *temp = head;
         head = head->next;
         delete temp;
       }

       void deleteAtEnd() {
       if (head == nullptr) {
         return;
       }
       if (head->next == nullptr) {
         delete head;
         head = nullptr;
         return;
       }
       Node *temp = head;
       while (temp->next->next != nullptr) {
         temp = temp->next;
       }
       delete temp->next;
       temp->next = nullptr;
     }

     void printList() {
       Node *temp = head;
       while (temp != nullptr) {
         ofs << "( " << temp->x << ", " << temp->y << " ) ";
         temp = temp->next;
       }
       ofs << endl;
     }
 };
 */
