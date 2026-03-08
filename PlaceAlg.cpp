
#include <bits/stdc++.h>
#include "contour.h"
#include "myGraph.h"
#include "parser.h"
#include "TREE.h"
#include "PlaceAlg.h"
using namespace std;
string outputfileName;
void PlacePosition(wire_graph const &subGraph)
{
//    ofstream ofs_matlab(outputfileName,ios::out);
    float ratio, confirm_ratio=2;
    int area, height, width, widthSpace, heightSpace, cases;
    int tempWidthSpace, tempHeightSpace; // for check widthSpace : 記錄最靠近BottomLeft的x座標
    int levelFinish; // 這層有幾個module完成擺置
    for (auto& level : subGraph.MST->mapLevelOrder) { // traverse 每個level
        levelFinish = 0; // initial
        vector<TreeNode*> vec_insideLevel;
        vec_insideLevel.assign(level.second.begin(), level.second.end()); // copy to temp vector, 為了方便操作
        while (levelFinish < level.second.size()) { // level內module都擺置完,即結束迴圈

            /* 確認有足夠空間放 */
            // confirm width
            ContourNode* itr_contour = contour->BottomLeft->prev;
            ContourNode* BottomLeft = contour->BottomLeft;
            widthSpace = INFINITY; // initial
            while (itr_contour->coordinate.x > BottomLeft->coordinate.x && itr_contour!=contour->BottomLeft) {
                if ( itr_contour->next_direction==2 && itr_contour->coordinate.y > BottomLeft->coordinate.y) {
                    widthSpace = itr_contour->coordinate.x-BottomLeft->coordinate.x;
                    break;
//                    tempWidthSpace = itr_contour->coordinate.x-BottomLeft->coordinate.x;
//                    if ( tempWidthSpace < widthSpace) {
//                        widthSpace = tempWidthSpace;
//                    }
                }
                itr_contour = itr_contour->prev; // counterclockwise traversal
            }
            int tempSpace_w = widthSpace;
//            while (itr_contour->coordinate.x > BottomLeft->coordinate.x && itr_contour!=contour->BottomLeft) {
//                if (itr_contour->coordinate.y > BottomLeft->coordinate.y) {
//                    tempWidthSpace = itr_contour->coordinate.x-BottomLeft->coordinate.x;
//                    if ( tempWidthSpace < widthSpace) {
//                        widthSpace = tempWidthSpace;
//                    }
//                }
//                itr_contour = itr_contour->prev; // counterclockwise traversal
//            }

            // confirm height
            itr_contour = contour->BottomLeft->next;
            while (itr_contour->coordinate.x <= BottomLeft->coordinate.x) {
                itr_contour = itr_contour->next; // clockwise traversal
            }
            heightSpace = itr_contour->coordinate.y-BottomLeft->coordinate.y;
            int tempSpace_h = heightSpace;

            bool flag_place = false;
            int erase_pos=0;
            bool tryRatio;
            for (auto& n : vec_insideLevel) {
                width = 0; height = 0;
                contour->checkSpace(width, height, cases, n->m_nNodeName, widthSpace, heightSpace);
                if (cases == 1 || cases == 2 || cases == 3) {
                    flag_place = true;
                }
                else {
                    tryRatio = true; confirm_ratio = INFINITY;
                    area = SoftModule_map[n->m_nNodeName].minArea_restrict;
//                    cout<<endl<<"area:"<<area<<endl;
//                    cout << n->m_nNodeName<<endl;

                    for(int i=0; i<16; i++){
                        heightSpace = tempSpace_h;
                        widthSpace = tempSpace_w;
                        if (widthSpace<heightSpace) // 寬的空間較小, 從ratio=2試
                            ratio = 2-i*0.1;
                        else  // 高的空間較小, 從ratio=0.5試
                            ratio = 0.5+i*0.1;
//                        cout<<"ratio:"<<ratio<<" ";

                        width = ratio*10*ceil(sqrt(area/(ratio*100)));
                        height = 10*ceil(sqrt(area/(ratio*100)));
//                        cout<<"width:"<<width<<" height:"<<height<<endl;

                        // fix bug : confirm height again
                        if (width<=widthSpace) {
                            itr_contour = contour->BottomLeft->next;
                            heightSpace = INFINITY;
                            while (itr_contour->coordinate.x <= BottomLeft->coordinate.x+width && itr_contour!=BottomLeft) {
                                tempHeightSpace = itr_contour->coordinate.y-BottomLeft->coordinate.y;
                                if ( itr_contour->next_direction==4 && tempHeightSpace < heightSpace) {
                                    heightSpace = tempHeightSpace;
                                }
                                itr_contour = itr_contour->next; // clockwise traversal
                            }
                        }

                        // fix bug : confirm width again
                        if (height<=heightSpace) {
                            ContourNode* itr_contour = contour->BottomLeft->prev;
                            widthSpace = INFINITY; // initial
                            while (itr_contour->coordinate.y <= BottomLeft->coordinate.y+height && itr_contour!=contour->BottomLeft) {
                                tempWidthSpace = itr_contour->coordinate.x-BottomLeft->coordinate.x;
                                if ( itr_contour->prev_direction==2 && tempWidthSpace < widthSpace) {
                                    widthSpace = tempWidthSpace;
                                }
                                itr_contour = itr_contour->prev; // counterclockwise traversal
                            }
                        }

                        if (width<=widthSpace && height<=heightSpace)  {// can place module
                            if (abs(ratio-1.2)<abs(confirm_ratio-1.2)) { // 取接近中間值
                                confirm_ratio = ratio;
                            }
                            flag_place = true;
                            tryRatio = false;
                        }
                    } // for loop: ratio
                    if(!tryRatio) {
                        ratio = confirm_ratio;
                        width = ceil(ratio*10*sqrt(area/(ratio*100)));
                        height = ceil(10*sqrt(area/(ratio*100)));
//                        break;
                    }

                } // end cases=4,5

                if (flag_place == true) { // can place module
//                    cout<<"can place module"<<endl;
                    levelFinish++;
                    /* 記錄module狀態 */
                    SoftModule_map[n->m_nNodeName].placed = true;
                    LegalTypeStruct tempType;
                    BottomLeft = contour->BottomLeft;
                    // 記錄轉角座標
                    tempType.position = new ContourList;
                    tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x, BottomLeft->coordinate.y});
                    tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x+width, BottomLeft->coordinate.y});
                    tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x+width, BottomLeft->coordinate.y+height});
                    tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x, BottomLeft->coordinate.y+height});
                    // 在vec清空前copy
                    contour->insertVertex_vec.assign(tempType.position->insertVertex_vec.begin(), tempType.position->insertVertex_vec.end());
                    tempType.position->updateContour(); // 照順時針排序座標

                    /* check legal contour */
                    /*
                    itr_contour = contour->BottomLeft->next;
                    ContourNode* BL = contour->BottomLeft;
                    // 建立contour, 用來確認有沒有切割contour
                    ContourList* leftEdge = new ContourList;
                    ContourList* rightEdge = new ContourList;
                    ContourList* topEdge = new ContourList;
                    ContourList* bottomEdge = new ContourList;

                    while(itr_contour != BottomLeft) { // traversal whole contour
                        if (itr_contour->coordinate.y>=BL->coordinate.y && itr_contour->coordinate.y<=BL->coordinate.y+height) {
                            // left edge
                            if (itr_contour->coordinate.x<=BL->coordinate.x) {
                                leftEdge->insertVertex_vec.push_back(itr_contour->coordinate);

                            }
                            // right edge
                            else if (itr_contour->coordinate.x>=BL->coordinate.x+width) {
                                rightEdge->insertVertex_vec.push_back(itr_contour->coordinate);

                            }
                        }
                        if (itr_contour->coordinate.x>=BL->coordinate.x && itr_contour->coordinate.x<=BL->coordinate.x+width) {
                            // top edge
                            if (itr_contour->coordinate.y>=BL->coordinate.y+height) {
                                topEdge->insertVertex_vec.push_back(itr_contour->coordinate);

                            }
                            // bottom edge
                            else if (itr_contour->coordinate.y<=BL->coordinate.y) {
                                bottomEdge->insertVertex_vec.push_back(itr_contour->coordinate);

                            }
                        }
                        itr_contour = itr_contour->next;
                    } // end traversal whole contour

                    if (leftEdge->checkClosedLoop()) { //有切斷
                        cout << "left edge" << endl;
                        // 刪除舊點
                        tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x+width, BottomLeft->coordinate.y+height});
                        tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x, BottomLeft->coordinate.y+height});
                        // 增加新點
                        height = height-5;
                        tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x+width, BottomLeft->coordinate.y+height});
                        tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x, BottomLeft->coordinate.y+height});
                        contour->insertVertex_vec.assign(tempType.position->insertVertex_vec.begin(), tempType.position->insertVertex_vec.end()); // 在vec清空前copy
                        tempType.position->updateContour(); // 照順時針排序座標

                    }
                    if (rightEdge->checkClosedLoop()) { //有切斷
                        cout << "right edge" << endl;
                    }
                    if (topEdge->checkClosedLoop()) { //有切斷
                        cout << "top edge" << endl;
                    }
                    if (bottomEdge->checkClosedLoop()) { //有切斷
                        cout << "bottom edge" << endl;
                    }
                    */
                    // 記錄其他
                    tempType.area = width*height;
                    tempType.h = height;
                    tempType.w = width;
                    tempType.aspect_ratio = height/width;
                    tempType.centerX = tempType.position->head->coordinate.x + width/2.0;
                    tempType.centerY = tempType.position->head->coordinate.y + height/2.0;
                    SoftModule_map[n->m_nNodeName].LegalType.push_back(tempType);

                    SoftModule_map[n->m_nNodeName].chooseLegalType++;
//                    cout << n->m_nNodeName << " " << tempType.position->head->coordinate.x << " " << tempType.position->head->coordinate.y << " " << width << " " << height << endl;
//                    ofs_matlab << n->m_nNodeName << " " << tempType.position->head->coordinate.x << " " << tempType.position->head->coordinate.y << " " << width << " " << height << endl;

//                    ofs_matlab << n->m_nNodeName<<" "<<"4"<<endl;
//                    ofs_matlab << tempType.position->head->coordinate.x << " " << tempType.position->head->coordinate.y<<endl;
//                    ofs_matlab << tempType.position->head->coordinate.x << " " << tempType.position->head->coordinate.y + height<<endl;
//                    ofs_matlab << tempType.position->head->coordinate.x + width<< " " << tempType.position->head->coordinate.y + height<<endl;
//                    ofs_matlab << tempType.position->head->coordinate.x + width<< " " << tempType.position->head->coordinate.y<<endl;


                    matlab_module_num++;
                    /* 更新contour狀態 */
                    contour->updateContour();
//                    cout << contour->BottomLeft->coordinate.x << " " << contour->BottomLeft->coordinate.y;
//                    contour->PrintList();
//                    cout << endl;
                    vec_insideLevel.erase(vec_insideLevel.begin()+erase_pos); // 擺置完即可刪除
                    break;
                } // end can place module
                erase_pos++;
            } // end traversal vec_insideLevel

            if (flag_place == false) { // 沒有任一個module可擺置
                PlaceSqare();
            } // end if no module place
        } // end while level內module都擺置完
    }
//    ofs_matlab.close();
}

void PlaceSqare()
{
    int widthSpace, heightSpace;
    ContourNode* BL = contour->BottomLeft;
    ContourNode* BR = contour->BottomLeft->prev; // node of BottomRight
    ContourNode* TR = contour->BottomLeft->prev->prev; // node of TopRight, if填平處為向下凹
    ContourNode* tempTR = contour->BottomLeft->next->next;
    if ( tempTR->coordinate.x>BL->coordinate.x ) { // if 填平處為向左凹
        TR = contour->BottomLeft->next->next;
    }
    ContourNode* TL = contour->BottomLeft->next; // node of TopLeft

    int xB = BR->coordinate.x-BL->coordinate.x; // x length of Bottom
    int xT = TR->coordinate.x-TL->coordinate.x; // x length of Top
    int yL = TL->coordinate.y-BL->coordinate.y; // y height of Left
    int yR = TR->coordinate.y-BR->coordinate.y; // x height of Right

//                if (xB==xT && yL==yR) { // fix bug : if 填平處為向左凹
//                    TR = contour->BottomLeft->next->next;
//                    xB = BR->coordinate.x-BottomLeft->coordinate.x; // x length of Bottom
//                    xT = TR->coordinate.x-TL->coordinate.x; // x length of Top
//                    yL = TL->coordinate.y-BottomLeft->coordinate.y; // y height of Left
//                    yR = TR->coordinate.y-BR->coordinate.y; // x height of Right
//                }

    if (xB<=xT && xB>=0)
        widthSpace = xB;
    else
        widthSpace = xT;
    if (yL <= yR || yR<0)
        heightSpace = yL;
    else
        heightSpace = yR;

    nonPlaceNum++; // debug記錄用
//    cout << "non place num:" << nonPlaceNum << endl;
//    cout << " " << BL->coordinate.x << " " << BL->coordinate.y << " " << widthSpace << " " << heightSpace << endl;
//    cout << "non" << nonPlaceNum << " " << BL->coordinate.x << " " << BL->coordinate.y << " " << widthSpace << " " << heightSpace << endl;
    contour->insertVertex_vec.push_back(BL->coordinate);
    contour->insertVertex_vec.push_back({BL->coordinate.x+widthSpace, BL->coordinate.y});
    contour->insertVertex_vec.push_back({BL->coordinate.x+widthSpace, BL->coordinate.y+heightSpace});
    contour->insertVertex_vec.push_back({BL->coordinate.x, BL->coordinate.y+heightSpace});
    contour->updateContour();

//    cout << contour->BottomLeft->coordinate.x << " " << contour->BottomLeft->coordinate.y;
//    contour->PrintList();
//    cout << endl;
}

void PlaceRestPosition()
{
//    ifstream ofs_in(outputfileName,ios::in);
//    queue<string> tmp;
//    string t;
//    while(getline(ofs_in, t))
//    {
//        tmp.push(t);
//    }
//    ofs_in.close();
//    ofstream ofs_out(outputfileName,ios::out);
//    while(!tmp.empty())
//    {
//        t = tmp.front();
//        tmp.pop();
//        ofs_out<<t<<endl;
//    }
    float ratio, confirm_ratio=2;
    int area, height, width, widthSpace, heightSpace, cases;
    int tempWidthSpace, tempHeightSpace; // for check widthSpace : 記錄最靠近BottomLeft的x座標
    int levelFinish; // 這層有幾個module完成擺置

    // 建vector為了後續traverse
    map<int, vector<TreeNode*>> map_RestLevelOrder;
    int nNodeID=0;
    // 剩餘module
    for (const auto& n : SoftModule_map) {
        if (n.second.placed == false) { // 不在graph中
            TreeNode* TreeNode_temp = new TreeNode(nNodeID);
            TreeNode_temp->m_nNodeName = n.first;
            map_RestLevelOrder[0].push_back(TreeNode_temp);
            nNodeID++;
        }
    }

    for (auto& level : map_RestLevelOrder) { // traverse 每個level
        levelFinish = 0; // initial
        vector<TreeNode*> vec_insideLevel;
        vec_insideLevel.assign(level.second.begin(), level.second.end()); // copy to temp vector, 為了方便操作
        while (levelFinish < level.second.size()) { // level內module都擺置完,即結束迴圈

            /* 確認有足夠空間放 */
            // confirm width
            ContourNode* itr_contour = contour->BottomLeft->prev;
            ContourNode* BottomLeft = contour->BottomLeft;
            widthSpace = INFINITY; // initial
            while (itr_contour->coordinate.x > BottomLeft->coordinate.x && itr_contour!=contour->BottomLeft) {
                if ( itr_contour->next_direction==2 && itr_contour->coordinate.y > BottomLeft->coordinate.y) {
                    widthSpace = itr_contour->coordinate.x-BottomLeft->coordinate.x;
                    break;
//                    tempWidthSpace = itr_contour->coordinate.x-BottomLeft->coordinate.x;
//                    if ( tempWidthSpace < widthSpace) {
//                        widthSpace = tempWidthSpace;
//                    }
                }
                itr_contour = itr_contour->prev; // counterclockwise traversal
            }
            int tempSpace_w = widthSpace;
//            while (itr_contour->coordinate.x > BottomLeft->coordinate.x && itr_contour!=contour->BottomLeft) {
//                if (itr_contour->coordinate.y > BottomLeft->coordinate.y) {
//                    tempWidthSpace = itr_contour->coordinate.x-BottomLeft->coordinate.x;
//                    if ( tempWidthSpace < widthSpace) {
//                        widthSpace = tempWidthSpace;
//                    }
//                }
//                itr_contour = itr_contour->prev; // counterclockwise traversal
//            }

            // confirm height
            itr_contour = contour->BottomLeft->next;
            while (itr_contour->coordinate.x <= BottomLeft->coordinate.x) {
                itr_contour = itr_contour->next; // clockwise traversal
            }
            heightSpace = itr_contour->coordinate.y-BottomLeft->coordinate.y;
            int tempSpace_h = heightSpace;

            bool flag_place = false;
            int erase_pos=0;
            bool tryRatio;
            for (auto& n : vec_insideLevel) {
                width = 0; height = 0;
                contour->checkSpace(width, height, cases, n->m_nNodeName, widthSpace, heightSpace);
                if (cases == 1 || cases == 2 || cases == 3) {
                    flag_place = true;
                }
                else {
                    tryRatio = true; confirm_ratio = INFINITY;
                    area = SoftModule_map[n->m_nNodeName].minArea_restrict;
//                    cout<<endl<<"area:"<<area<<endl;
//                    cout << n->m_nNodeName<<endl;

                    for(int i=0; i<16; i++){
                        heightSpace = tempSpace_h;
                        widthSpace = tempSpace_w;
                        if (widthSpace<heightSpace) // 寬的空間較小, 從ratio=2試
                            ratio = 2.0-i*0.1;
                        else  // 高的空間較小, 從ratio=0.5試
                            ratio = 0.5+i*0.1;
//                        cout<<"ratio:"<<ratio<<" ";

                        width = ratio*10*ceil(sqrt(area/(ratio*100)));
                        height = 10*ceil(sqrt(area/(ratio*100)));
//                        cout<<"width:"<<width<<" height:"<<height<<endl;

                        // fix bug : confirm height again
                        if (width<=widthSpace) {
                            itr_contour = contour->BottomLeft->next;
                            heightSpace = INFINITY;
                            while (itr_contour->coordinate.x <= BottomLeft->coordinate.x+width && itr_contour!=BottomLeft) {
                                tempHeightSpace = itr_contour->coordinate.y-BottomLeft->coordinate.y;
                                if ( itr_contour->next_direction==4 && tempHeightSpace < heightSpace) {
                                    heightSpace = tempHeightSpace;
                                }
                                itr_contour = itr_contour->next; // clockwise traversal
                            }
                        }

                        // fix bug : confirm width again
                        if (height<=heightSpace) {
                            ContourNode* itr_contour = contour->BottomLeft->prev;
                            widthSpace = INFINITY; // initial
                            while (itr_contour->coordinate.y <= BottomLeft->coordinate.y+height && itr_contour!=contour->BottomLeft) {
                                tempWidthSpace = itr_contour->coordinate.x-BottomLeft->coordinate.x;
                                if ( itr_contour->prev_direction==2 && tempWidthSpace < widthSpace) {
                                    widthSpace = tempWidthSpace;
                                }
                                itr_contour = itr_contour->prev; // counterclockwise traversal
                            }
                        }

                        if (width<=widthSpace && height<=heightSpace)  {// can place module
                            if (abs(ratio-1.2)<abs(confirm_ratio-1.2)) { // 取接近中間值
                                confirm_ratio = ratio;
                            }
                            flag_place = true;
                            tryRatio = false;
                        }
                    } // for loop: ratio
                    if(!tryRatio) {
                        ratio = confirm_ratio;
                        width = ceil(ratio*10*sqrt(area/(ratio*100)));
                        height = ceil(10*sqrt(area/(ratio*100)));
                    }

                } // end cases=4,5

                if (flag_place == true) { // can place module
//                    cout<<"can place module"<<endl;
                    levelFinish++;
                    /* 記錄module狀態 */
                    SoftModule_map[n->m_nNodeName].placed = true;
                    LegalTypeStruct tempType;
                    BottomLeft = contour->BottomLeft;
                    // 記錄轉角座標
                    tempType.position = new ContourList;
                    tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x, BottomLeft->coordinate.y});
                    tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x+width, BottomLeft->coordinate.y});
                    tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x+width, BottomLeft->coordinate.y+height});
                    tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x, BottomLeft->coordinate.y+height});
                    // 在vec清空前copy
                    contour->insertVertex_vec.assign(tempType.position->insertVertex_vec.begin(), tempType.position->insertVertex_vec.end());
                    tempType.position->updateContour(); // 照順時針排序座標

                    /* check legal contour */
                    /*
                    itr_contour = contour->BottomLeft->next;
                    ContourNode* BL = contour->BottomLeft;
                    // 建立contour, 用來確認有沒有切割contour
                    ContourList* leftEdge = new ContourList;
                    ContourList* rightEdge = new ContourList;
                    ContourList* topEdge = new ContourList;
                    ContourList* bottomEdge = new ContourList;

                    while(itr_contour != BottomLeft) { // traversal whole contour
                        if (itr_contour->coordinate.y>=BL->coordinate.y && itr_contour->coordinate.y<=BL->coordinate.y+height) {
                            // left edge
                            if (itr_contour->coordinate.x<=BL->coordinate.x) {
                                leftEdge->insertVertex_vec.push_back(itr_contour->coordinate);

                            }
                            // right edge
                            else if (itr_contour->coordinate.x>=BL->coordinate.x+width) {
                                rightEdge->insertVertex_vec.push_back(itr_contour->coordinate);

                            }
                        }
                        if (itr_contour->coordinate.x>=BL->coordinate.x && itr_contour->coordinate.x<=BL->coordinate.x+width) {
                            // top edge
                            if (itr_contour->coordinate.y>=BL->coordinate.y+height) {
                                topEdge->insertVertex_vec.push_back(itr_contour->coordinate);

                            }
                            // bottom edge
                            else if (itr_contour->coordinate.y<=BL->coordinate.y) {
                                bottomEdge->insertVertex_vec.push_back(itr_contour->coordinate);

                            }
                        }
                        itr_contour = itr_contour->next;
                    } // end traversal whole contour

                    if (leftEdge->checkClosedLoop()) { //有切斷
                        cout << "left edge" << endl;
                        // 刪除舊點
                        tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x+width, BottomLeft->coordinate.y+height});
                        tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x, BottomLeft->coordinate.y+height});
                        // 增加新點
                        height = height-5;
                        tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x+width, BottomLeft->coordinate.y+height});
                        tempType.position->insertVertex_vec.push_back({BottomLeft->coordinate.x, BottomLeft->coordinate.y+height});
                        contour->insertVertex_vec.assign(tempType.position->insertVertex_vec.begin(), tempType.position->insertVertex_vec.end()); // 在vec清空前copy
                        tempType.position->updateContour(); // 照順時針排序座標

                    }
                    if (rightEdge->checkClosedLoop()) { //有切斷
                        cout << "right edge" << endl;
                    }
                    if (topEdge->checkClosedLoop()) { //有切斷
                        cout << "top edge" << endl;
                    }
                    if (bottomEdge->checkClosedLoop()) { //有切斷
                        cout << "bottom edge" << endl;
                    }
                    */
                    // 記錄其他
                    tempType.area = width*height;
                    tempType.h = height;
                    tempType.w = width;
                    tempType.aspect_ratio = height/width;
                    tempType.centerX = tempType.position->head->coordinate.x + width/2.0;
                    tempType.centerY = tempType.position->head->coordinate.y + height/2.0;
                    SoftModule_map[n->m_nNodeName].LegalType.push_back(tempType);
                    SoftModule_map[n->m_nNodeName].chooseLegalType++;
//                    cout << n->m_nNodeName << " " << tempType.position->head->coordinate.x << " " << tempType.position->head->coordinate.y << " " << width << " " << height << endl;

//                    ofs_out << n->m_nNodeName << " " << tempType.position->head->coordinate.x << " " << tempType.position->head->coordinate.y << " " << width << " " << height << endl;
//                    ofs_out << n->m_nNodeName<<" "<<"4"<<endl;
//                    ofs_out<< tempType.position->head->coordinate.x << " " << tempType.position->head->coordinate.y<<endl;
//                    ofs_out<< tempType.position->head->coordinate.x << " " << tempType.position->head->coordinate.y + height<<endl;
//                    ofs_out<< tempType.position->head->coordinate.x + width<< " " << tempType.position->head->coordinate.y + height<<endl;
//                    ofs_out<< tempType.position->head->coordinate.x + width<< " " << tempType.position->head->coordinate.y<<endl;

                    matlab_module_num++;
                    /* 更新contour狀態 */
                    contour->updateContour();
//                    cout << contour->BottomLeft->coordinate.x << " " << contour->BottomLeft->coordinate.y;
//                    contour->PrintList();
//                    cout << endl;
                    vec_insideLevel.erase(vec_insideLevel.begin()+erase_pos); // 擺置完即可刪除
                    break;
                } // end can place module
                erase_pos++;
            } // end traversal vec_insideLevel

            if (flag_place == false) { // 沒有任一個module可擺置
                PlaceSqare();
            } // end if no module place
        } // end while level內module都擺置完
    }
//    ofs_out.close();
}
