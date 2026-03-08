
#include <bits/stdc++.h>
#include "contour.h"
#include "myGraph.h"
#include "parser.h"
#include "TREE.h"
using namespace std;

ContourList* contour;

bool sort_x(CoordinateStruct a, CoordinateStruct b)
{
    return a.x < b.x;
}
bool sort_y(CoordinateStruct a, CoordinateStruct b)
{
    return a.y < b.y;
}
void bubbleSort(vector<CoordinateStruct>& a)
{
      bool swapp = true;
      while(swapp){
        swapp = false;
        for (int i = 0; i < a.size()-1; i++) {
            if (a[i].x > a[i+1].x ){
                swap(a[i], a[i+1]);
                swapp = true;
            }
        }
    }
}

void create_contour()
{
    contour = new ContourList;
    contour->insertVertex_vec.push_back({0, 0});
    contour->insertVertex_vec.push_back({0, chip_height});
    contour->insertVertex_vec.push_back({chip_width, chip_height});
    contour->insertVertex_vec.push_back({chip_width, 0});
    for(auto& n : FixedModule_map) {
        for (short int i=0; i<4; i++) {
            contour->insertVertex_vec.push_back(n.second.coordinate[i]);
        }
    }

    contour->updateContour();
//    cout << contour->BottomLeft->coordinate.x << " " << contour->BottomLeft->coordinate.y << endl;
//    contour->PrintList();

}

ContourList::ContourList()
{
    head = NULL;
    BottomLeft = NULL;
}

void ContourList:: insertEnd(CoordinateStruct value)
{
    // If the list is empty, create a doubly list
    if (head == NULL) {
        struct ContourNode* new_node = new ContourNode;
        new_node->coordinate = value;
        new_node->next = new_node->prev = new_node;
        head = new_node;
        return;
    }

    // If list is not empty
    ContourNode* last = (head)->prev;
    struct ContourNode* new_node = new ContourNode;
    new_node->coordinate = value;

    new_node->next = head;     // Start is going to be next of new_node
    (head)->prev = new_node;   // Make new node previous of start
    new_node->prev = last;     // Make last previous of new node
    last->next = new_node;     // Make new node next of old last
}

void ContourList:: insertBegin(CoordinateStruct value)
{
    // Pointer points to last Node
    struct ContourNode* last = (head)->prev;
    struct ContourNode* new_node = new ContourNode;
    new_node->coordinate = value; // Inserting the data

    // setting up previous and next of new node
    new_node->next = head;
    new_node->prev = last;

    // Update next and previous pointers of start and last.
    last->next = (head)->prev = new_node;
    head = new_node; // Update start pointer
}

void ContourList:: insertAfter(CoordinateStruct insertValue, CoordinateStruct beforeValue) // beforeValue->insertValue
{
    struct ContourNode* new_node = new ContourNode;
    new_node->coordinate = insertValue; // Inserting the data

    // Find node having value2 and next node of it
    struct ContourNode* temp = head;
    while (temp->coordinate.x != beforeValue.x && temp->coordinate.y != beforeValue.y)
        temp = temp->next;
    struct ContourNode* next = temp->next;

    // insert new_node between temp and next.
    temp->next = new_node;
    new_node->prev = temp;
    new_node->next = next;
    next->prev = new_node;
}

void ContourList:: deleteNode(ContourNode* del) {
   if (head == NULL || del == NULL) {
      return;
   }
   if (head == del) { // 欲刪除的node為head
       head = del->next;
   }
   if (del->next != NULL) {
      del->next->prev = del->prev;
   }
   if (del->prev != NULL) {
      del->prev->next = del->next;
   }
   free(del);
   return;
}

void ContourList:: updateBoundary()
{
    ContourNode* temp = head;
    ContourNode* tempHead = head;
    left = INFINITY; right = 0;
    bottom = INFINITY; top = 0;
//    swap(left, right);
//    swap(top, bottom);
    if(temp != NULL) {
        do {
            if (temp->coordinate.x < left) {
                left = temp->coordinate.x;
                tempHead = temp;
                if (temp->coordinate.y < tempHead->coordinate.y)
                    tempHead = temp;
            }
            else if (temp->coordinate.x == left && temp->coordinate.y < tempHead->coordinate.y) {
                tempHead = temp;
            }
            if (temp->coordinate.x > right)
                right = temp->coordinate.x;
            if (temp->coordinate.y < bottom)
                bottom = temp->coordinate.y;
            if (temp->coordinate.x > top)
                top = temp->coordinate.y;
            temp = temp->next;
        } while(temp != head);
//            ofs << left << " " << right << " " << top << " " << bottom << endl;
        // 防止bug
        if (tempHead->next_direction == 3 || tempHead->next_direction == 4) {
            temp = tempHead;
            while (temp->next_direction != 1 && temp->next_direction != 2){
                temp = temp->prev; // counterclockwise traversal
            }
            tempHead = temp;
        }
        head = tempHead;
    } else { // empty list
        return;
    }
}

void ContourList:: updateContour()
{
    struct ContourNode *temp = head;
    struct ContourNode* new_node = new ContourNode;
    int flag=0;
    vector<CandidateStruct> candidate; // 拿來暫放候選的vertex
    vector<CoordinateStruct>::iterator itr_i = insertVertex_vec.begin();
    vector<CoordinateStruct>::iterator itr_j, itr_temp;

    // organize insertVertex_vec : delete duplicate vertex
    while (itr_i != insertVertex_vec.end()){
        itr_j = itr_i;
        itr_j++;
        while (itr_j != insertVertex_vec.end()){
            if (itr_j->x == itr_i->x && itr_j->y == itr_i->y) {
                flag = 1;
                itr_temp = itr_j;
                if (itr_j == insertVertex_vec.end()-1) {
                    insertVertex_vec.erase(itr_temp);
                    break;
                }
                else
                    insertVertex_vec.erase(itr_temp);
            }
            itr_j++;
        }
        if (flag == 1) {
            flag = 0;
            itr_temp = itr_i;
            insertVertex_vec.erase(itr_temp);
            itr_i--;
        }
        itr_i++;
    }

    // organize insertVertex_vec : sort
    sort(insertVertex_vec.begin(), insertVertex_vec.end(), sort_y);
    ::bubbleSort(insertVertex_vec);

    if (head==NULL){ // insert vertex to contour : fixed module contour
        insertEnd(insertVertex_vec[0]);
        insertVertex_vec.erase(insertVertex_vec.begin());
    }
    else { // Remove duplicate vertices from insertVertex_vec in contour
        temp = head;
        struct ContourNode *del;
        do { // traverse list
            itr_i = insertVertex_vec.begin();
            flag = 0;
            while (itr_i != insertVertex_vec.end()){
                if (temp->coordinate.x == itr_i->x && temp->coordinate.y == itr_i->y) {
                    itr_temp = itr_i;
                    insertVertex_vec.erase(itr_temp);
                    del = temp;
                    if (temp == head) flag = 1;
                    temp = temp->prev;
                    deleteNode(del);
                    break;
                }
                itr_i++;
            } // end traverse vec
            temp = temp->next;
        } while(temp != head || flag==1);
        updateBoundary();
    }
    head->next_direction = 1; // up
    temp = head;
    pre_direction = 2;
    direction = 1; // 下一個要放V

    // insert vertex to contour : other vertex
    struct ContourNode* next = temp->next;
    CoordinateStruct near; //離temp最近的座標
    int vec_erase_pos; // 為了定位要刪掉哪個在insertVertex_vec裡的座標
    BottomLeft = head; // initial Bottom Left Coordinate

    do { // traverse list
        candidate.clear();
        new_node = new ContourNode;
        flag = 0;
        if ( (temp->coordinate.x+temp->coordinate.y) < (BottomLeft->coordinate.x+BottomLeft->coordinate.y) && temp->next_direction==1)
            BottomLeft = temp;

        if (direction == 1) {
            for (int i=0; i<insertVertex_vec.size(); i++) { //遍歷vector，找與temp連線的vertex
                if (insertVertex_vec[i].x == temp->coordinate.x) {
                    candidate.push_back({insertVertex_vec[i], i});
                }
            }
            // 有與temp連線的vertex
            if (!candidate.empty()) {
                flag = 1; // need to insert
                near = candidate[0].coordinate;
                vec_erase_pos = candidate[0].vec_num;
                for (auto& n : candidate) { // 找尋候選裡面離temp較近的vertex
                    int dist_compare = n.coordinate.y-temp->coordinate.y;
                    int dist_near = near.y-temp->coordinate.y;
                    // 正負一致
                    if ( (dist_compare>=0 && dist_near>=0) || (dist_compare<=0 && dist_near<=0)) {
                        if ( abs(dist_compare)<=abs(dist_near) ) {
                            near = n.coordinate; // 更新near
                            vec_erase_pos = n.vec_num; // 更新vec_erase_pos
                        }
                    }
                    // 正負不一致
                    else {
                        if (n.coordinate.y > near.y) {
                            near = n.coordinate; // 更新near
                            vec_erase_pos = n.vec_num; // 更新vec_erase_pos
                        }
                    }
                }
                if (temp->next!=temp && temp->next!=head && temp->next->coordinate.x == temp->coordinate.x ){
                    // temp下一個還有vertex
                    int dist_compare = temp->next->coordinate.y-temp->coordinate.y;
                    int dist_near = near.y-temp->coordinate.y;
                    // 正負一致
                    if ( (dist_compare>=0 && dist_near>=0) || (dist_compare<=0 && dist_near<=0)) {
                        if ( abs(dist_compare)<abs(dist_near) ) { // 原list 的下一個vertex離temp較近
                            flag = 0;
                        }
                    }
                    // 正負不一致
                    else {
                        if (temp->next->coordinate.y > near.y) {
                            flag = 0;
                        }
                    }
                }
            }
            pre_direction = 1;
            direction = 2;  // 下一個要放H
        }
        else if (direction == 2) {
            for (int i=0; i<insertVertex_vec.size(); i++) { //遍歷vector，找與temp連線的vertex
                if (insertVertex_vec[i].y == temp->coordinate.y) {
                    candidate.push_back({insertVertex_vec[i], i});
                }
            }
            // 有與temp連線的vertex
            if (!candidate.empty()) {
                flag = 1; // need to insert
                near = candidate[0].coordinate;
                vec_erase_pos = candidate[0].vec_num;
                for (auto& n : candidate) { // 找尋候選裡面離temp較近的vertex
                    int dist_compare = n.coordinate.x-temp->coordinate.x;
                    int dist_near = near.x-temp->coordinate.x;
                    // 正負一致
//                    if ( (dist_compare>=0 && dist_near>=0) || (dist_compare<=0 && dist_near<=0)) {
                        if ( abs(dist_compare)<=abs(dist_near) ) {
                            near = n.coordinate; // 更新near
                            vec_erase_pos = n.vec_num; // 更新vec_erase_pos
                        }
//                    }
                    // 正負不一致
//                    else {
//                        if (n.coordinate.x > near.x) {
//                            near = n.coordinate; // 更新near
//                            vec_erase_pos = n.vec_num; // 更新vec_erase_pos
//                        }
//                    }
                }
                if (temp->next!=temp && temp->next->coordinate.y == temp->coordinate.y ){
                    // temp下一個還有vertex
                    int dist_compare = temp->next->coordinate.x-temp->coordinate.x;
                    int dist_near = near.x-temp->coordinate.x;
                    // 正負一致
//                    if ( (dist_compare>=0 && dist_near>=0) || (dist_compare<=0 && dist_near<=0)) {
                        if ( abs(dist_compare)<abs(dist_near) ) { // 原list 的下一個vertex離temp較近
                            flag = 0;
                        }
//                    }
                    // 正負不一致
//                    else {
//                        if (temp->next->coordinate.x > near.x) {
//                            flag = 0;
//                        }
//                    }
                }
            }
            pre_direction = 2;
            direction = 1;
        } // end if direction==2

        if (flag == 1) { // need to insert new_node between temp and next.
            insertVertex_vec.erase(insertVertex_vec.begin()+vec_erase_pos);
            next = temp->next;
            new_node->coordinate = near;
            new_node->next_direction = direction;
            temp->next = new_node; // insert after temp
            new_node->prev = temp;
            new_node->next = next;
            next->prev = new_node;
        }
        // update node's direction
        if (pre_direction == 1) {
            if (temp->coordinate.y<temp->next->coordinate.y) // temp較低
                temp->next_direction = 1; // up
            else
                temp->next_direction = 2; // down
            if (temp->coordinate.x<temp->prev->coordinate.x) // temp在左
                temp->prev_direction = 3; // left
            else
                temp->prev_direction = 4; // right
        }
        else if (pre_direction == 2) {
            if (temp->coordinate.x<temp->next->coordinate.x) // temp在左
                temp->next_direction = 4; // right
            else
                temp->next_direction = 3; // down
            if (temp->coordinate.y<temp->prev->coordinate.y) // temp較低
                temp->prev_direction = 2;
            else
                temp->prev_direction = 1;
        }
        temp = temp->next;
    } while(temp != head);

    if (!insertVertex_vec.empty()) {
        insertVertex_vec.clear();
    }


} // end functionn

bool ContourList:: checkClosedLoop()
{
    struct ContourNode *temp = head;
    struct ContourNode* new_node = new ContourNode;
    int flag=0;
    vector<CandidateStruct> candidate; // 拿來暫放候選的vertex
//    vector<CoordinateStruct>::iterator itr_i = insertVertex_vec.begin();
//    vector<CoordinateStruct>::iterator itr_temp;

    if (insertVertex_vec.size()<=3 ) { // 不成立
        return false;
    }

    // organize insertVertex_vec : sort
    sort(insertVertex_vec.begin(), insertVertex_vec.end(), sort_y);
    ::bubbleSort(insertVertex_vec);

    // insert vertex to contour : first vertex
    insertEnd(insertVertex_vec[0]);
    insertVertex_vec.erase(insertVertex_vec.begin());

    head->next_direction = 1; // up
    temp = head;
    pre_direction = 2;
    direction = 1; // 下一個要放V

    // insert vertex to contour : other vertex
    struct ContourNode* next = temp->next;
    CoordinateStruct near; //離temp最近的座標
    int vec_erase_pos; // 為了定位要刪掉哪個在insertVertex_vec裡的座標
    BottomLeft = head; // initial Bottom Left Coordinate

    do { // traverse list
        candidate.clear();
        new_node = new ContourNode;
        vec_erase_pos = 0;
        flag = 0;
        if ( (temp->coordinate.x+temp->coordinate.y) < BottomLeft->coordinate.x+BottomLeft->coordinate.y)
            BottomLeft = temp;

        if (direction == 1) { // 放直的
            for (int i=0; i<insertVertex_vec.size(); i++) { //遍歷vector，找與temp連線的vertex
                if (insertVertex_vec[i].x == temp->coordinate.x) {
                    candidate.push_back({insertVertex_vec[i], i});
                }
            }
            // 有與temp連線的vertex
            if (!candidate.empty()) {
                flag = 1; // need to insert
                near = candidate[0].coordinate;
                vec_erase_pos = candidate[0].vec_num;
                for (auto& n : candidate) { // 找尋候選裡面離temp較近的vertex
                    if ( abs(n.coordinate.y-temp->coordinate.y)<=abs(near.y-temp->coordinate.y) ) {
                        near = n.coordinate; // 更新near
                        vec_erase_pos = n.vec_num; // 更新vec_erase_pos
                    }
                }
            }
            pre_direction = 1;
            direction = 2;  // 下一個要放H
        }
        else if (direction == 2) { // 放橫的
            for (int i=0; i<insertVertex_vec.size(); i++) { //遍歷vector，找與temp連線的vertex
                if (insertVertex_vec[i].y == temp->coordinate.y) {
                    candidate.push_back({insertVertex_vec[i], i});
                }
            }
            // 有與temp連線的vertex
            if (!candidate.empty()) {
                flag = 1; // need to insert
                near = candidate[0].coordinate;
                vec_erase_pos = candidate[0].vec_num;
                for (auto& n : candidate) { // 找尋候選裡面離temp較近的vertex
                    if ( abs(n.coordinate.x-temp->coordinate.x)<=abs(near.x-temp->coordinate.x) ) {
                        near = n.coordinate; // 更新near
                        vec_erase_pos = n.vec_num; // 更新vec_erase_pos
                    }
                }
            }
            pre_direction = 2;
            direction = 1;
        } // end if direction==2

        if (flag == 1) { // need to insert new_node between temp and next.
            insertVertex_vec.erase(insertVertex_vec.begin()+vec_erase_pos);
            next = temp->next;
            new_node->coordinate = near;
            new_node->next_direction = direction;
            temp->next = new_node; // insert after temp
            new_node->prev = temp;
            new_node->next = next;
            next->prev = new_node;
        }
        // update node's direction
        if (pre_direction == 1) {
            if (temp->coordinate.y<temp->next->coordinate.y) // temp較低
                temp->next_direction = 1; // up
            else
                temp->next_direction = 2; // down
            if (temp->coordinate.x<temp->prev->coordinate.x) // temp在左
                temp->prev_direction = 3; // left
            else
                temp->prev_direction = 4; // right
        }
        else if (pre_direction == 2) {
            if (temp->coordinate.x<temp->next->coordinate.x) // temp在左
                temp->next_direction = 4; // right
            else
                temp->next_direction = 3; // down
            if (temp->coordinate.y<temp->prev->coordinate.y) // temp較低
                temp->prev_direction = 2;
            else
                temp->prev_direction = 1;
        }
        temp = temp->next;
    } while(temp != head);

    if (!insertVertex_vec.empty()) {

        insertVertex_vec.clear();
        return false; // 沒有切斷contour
    }
    else
        return true;
}

void ContourList:: checkSpace(int &Wspace, int &Hspace, int &cases, string &m_name, int widthSpace, int heightSpace)
{
    cases = 4;
    ContourNode* BL = contour->BottomLeft;
    ContourNode* itr_contour;
    Wspace = BL->prev->coordinate.x-BL->coordinate.x;
    Hspace = BL->next->coordinate.y-BL->coordinate.y;
    int area = SoftModule_map[m_name].minArea_restrict;

    float width_w, height_w, width_h, height_h;
    width_w = Wspace; height_w = ceil(area/width_w);
    height_h = Hspace; width_h = ceil(area/height_h);
    float ratio_w = height_w/width_w;
    float ratio_h = height_h/width_h;
    int tempWidthSpace, tempHeightSpace; // for check widthSpace : 記錄最靠近BottomLeft的x座標
    int tempSpace_h = heightSpace; int tempSpace_w = widthSpace;

    if (ratio_w<=2 && ratio_w>=0.5) {
        // initial
        heightSpace = tempSpace_h;
        widthSpace = tempSpace_w;
        int width = (int)width_w;
        int height = (int)height_w;
        // fix bug : confirm height again
        if (width<=widthSpace) {
            itr_contour = contour->BottomLeft->next;
            heightSpace = INFINITY;
            while (itr_contour->coordinate.x <= BL->coordinate.x+width && itr_contour!=BL) {
                tempHeightSpace = itr_contour->coordinate.y-BL->coordinate.y;
                if ( itr_contour->next_direction==4 && tempHeightSpace < heightSpace) {
                    heightSpace = tempHeightSpace;
                }
                itr_contour = itr_contour->next; // clockwise traversal
            }
        }

        // fix bug : confirm width again
        if (height<=heightSpace) {
            itr_contour = contour->BottomLeft->prev;
            widthSpace = INFINITY; // initial
            while (itr_contour->coordinate.y <= BL->coordinate.y+height && itr_contour!=BottomLeft) {
                tempWidthSpace = itr_contour->coordinate.x-BL->coordinate.x;
                if ( itr_contour->prev_direction==2 && tempWidthSpace < widthSpace) {
                    widthSpace = tempWidthSpace;
                }
                itr_contour = itr_contour->prev; // counterclockwise traversal
            }
        }

        if (width<=widthSpace && height<=heightSpace)  {// can place module
            cases = 1;
        }
    }
    if (ratio_h<=2 && ratio_h>=0.5) {
        // initial
        heightSpace = tempSpace_h;
        widthSpace = tempSpace_w;
        int width = (int)width_h;
        int height = (int)height_h;
        // fix bug : confirm height again
        if (width<=widthSpace) {
            itr_contour = contour->BottomLeft->next;
            heightSpace = INFINITY;
            while (itr_contour->coordinate.x <= BL->coordinate.x+width && itr_contour!=BL) {
                tempHeightSpace = itr_contour->coordinate.y-BL->coordinate.y;
                if ( itr_contour->next_direction==4 && tempHeightSpace < heightSpace) {
                    heightSpace = tempHeightSpace;
                }
                itr_contour = itr_contour->next; // clockwise traversal
            }
        }

        // fix bug : confirm width again
        if (height<=heightSpace) {
            itr_contour = contour->BottomLeft->prev;
            widthSpace = INFINITY; // initial
            while (itr_contour->coordinate.y <= BL->coordinate.y+height && itr_contour!=BottomLeft) {
                tempWidthSpace = itr_contour->coordinate.x-BL->coordinate.x;
                if ( itr_contour->prev_direction==2 && tempWidthSpace < widthSpace) {
                    widthSpace = tempWidthSpace;
                }
                itr_contour = itr_contour->prev; // counterclockwise traversal
            }
        }

        if (width<=widthSpace && height<=heightSpace)  {// can place module
            if (cases == 1) cases = 3; // 固定高跟寬都可
            else cases = 2; // 固定高
        }
    }
//    if (cases == 0){
//
//    }
    switch (cases) {
        case 1:
            Wspace = (int)width_w;
            Hspace = (int)height_w;
            break;
        case 2:
            Wspace = (int)width_h;
            Hspace = (int)height_h;
            break;
        case 3:
            if (Wspace < Hspace) {
                Wspace = (int)width_w;
                Hspace = (int)height_w;
            }
            else {
                Wspace = (int)width_h;
                Hspace = (int)height_h;
            }
            break;
        default:
            break;
    }
}

void ContourList:: bubbleSort()
{
    int swapped;
    struct ContourNode *temp;
    struct ContourNode *lptr = head;

    /* Checking for empty list */
    if (head == NULL)
        return;

    do { // sort y
        swapped = 0;
        temp = head;

         do {
            if (temp->coordinate.y > temp->next->coordinate.y) {
                swap(temp->coordinate, temp->next->coordinate);
                swapped = 1;
            }
            temp = temp->next;
         } while (temp->next != lptr);
        lptr = temp;
    }
    while (swapped);

    lptr = head;
    do { // sort x
        swapped = 0;
        temp = head;

        do {
           if (temp->coordinate.x > temp->next->coordinate.x) {
               swap(temp->coordinate, temp->next->coordinate);
               swapped = 1;
           }
           temp = temp->next;
        } while (temp->next != lptr);
       lptr = temp;
    }
    while (swapped);

}

void ContourList:: PrintList()
{
//    ContourNode* temp = head;
//    if(temp != NULL) {
//        ofs << endl << "contour list: ";
//        while(temp->next != head) {
//          ofs << "(" << temp->coordinate.x << ", " << temp->coordinate.y << ") -> ";
//          temp = temp->next;
//        }
//        ofs << "(" << temp->coordinate.x << ", " << temp->coordinate.y << ")";
//        ofs << endl;
//    } else {
//        ofs << "The list is empty.\n";
//    }

    ContourNode* temp = head;
    if(temp != NULL) {
        cout << endl << "contour list: ";
        while(temp->next != head) {
          cout << "(" << temp->coordinate.x << ", " << temp->coordinate.y << ") -> ";
          temp = temp->next;
        }
        cout << "(" << temp->coordinate.x << ", " << temp->coordinate.y << ")";
        cout << endl;
    } else {
        cout << "The list is empty.\n";
    }
}
