#ifndef contour_h
#define contour_h
#include <bits/stdc++.h>
//#include "parser.h"

using namespace std;

struct CoordinateStruct
{
    int x, y;
};

extern int chip_width, chip_height, softModule_num, fixedModule_num, connection_num, ttlcon;
//extern vector<CoordinateStruct> insertVertex_vec;
bool sort_x(CoordinateStruct a, CoordinateStruct b);
bool sort_y(CoordinateStruct a, CoordinateStruct b);
void bubbleSort(vector<CoordinateStruct>& a);

struct CandidateStruct
{
//    int x, y;
    CoordinateStruct coordinate;
    int vec_num;
};

//node structure
struct ContourNode {
    CoordinateStruct coordinate;
    ContourNode* next;
    ContourNode* prev;
    short int next_direction=0; // show now need to put up(1), down(2) or left(3), right(4)
    short int prev_direction=0; // show now need to put up(1), down(2) or left(3), right(4)
};

class ContourList {
  public:
    ContourNode* head;
    ContourNode* BottomLeft; // Bottom Left Coordinate
    short int direction=1, pre_direction=2; // show now need to put vertical(1) or horizontal(2)
    int left=0, right=chip_width, top=chip_height, bottom=0;
    vector<CoordinateStruct> insertVertex_vec;
  public:
    //constructor to create an empty LinkedList
    ContourList();
    
    void insertEnd(CoordinateStruct value);
    
    void insertBegin(CoordinateStruct value);
    
    void insertAfter(CoordinateStruct insertValue, CoordinateStruct beforeValue);
    
    void deleteNode(ContourNode* del);
    
    void updateBoundary();
    
    void updateContour();
    
    void checkSpace(int &Wspace, int &Hspace, int &cases, string &m_name, int widthSpace, int heightSpace);
    
    bool checkClosedLoop();
    
    void bubbleSort();
    
    void PrintList();
};

extern ContourList* contour;
void create_contour();
#endif /* contour_h */

