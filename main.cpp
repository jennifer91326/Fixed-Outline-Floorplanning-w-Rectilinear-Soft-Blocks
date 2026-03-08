#include <bits/stdc++.h>
#include "contour.h"
#include "parser.h"
#include "TREE.h"
#include "myGraph.h"
#include "PlaceAlg.h"
#include <cstdio>
using namespace std;
//PlaceAlg 一次要改兩個fnc.!!!
int main(int argc, char *argv[]) {//工作站: int argc, char *argv[]
    inputfileName = argv[1];//工作站: argv[1];
    outputfileName =argv[2];//工作站: argv[2];
    parser();
    weightofwire();
    create_graph();

    int i=0;
    for (auto& n : subGraph) {
//        ofs << endl << "sub graph#" << i << endl;
        printGraph(*n);
//        ofs << "\tMST: " << endl << "\t";
        n->create_MST();
        n->MST->PrintTree( n->MST->GetRoot() );
        n->MST->GetLevelOrder();
//        ofs << "leaf num=" << n->MST->GetLeafNum() << endl;
//        n->MST->PrintLeafNode();
//        PlacePosition(*n);
        i++;
    }

    create_contour();
    for (auto& n : subGraph) {
        PlacePosition(*n);
    }
    // placed剩餘module
    PlaceRestPosition();


//    ofs_matlab << "0 0 " << chip_width << " " << chip_height << endl;
//    ofs_matlab << matlab_module_num << endl;
//    PlacePosition(*subGraph[0]);


//    ofs << endl << "chip 長寬:" << chip_width << " " << chip_height << endl;
//    ofs << "SoftModule[] size: " << SoftModule_map.size() << endl;
    /*for (const auto& n : SoftModule_map) {
        ofs << "key(name): " << n.first << " minArea_restrict: " << n.second.minArea_restrict << endl ;
        ofs << "\t" << "Connection_vec Size: " << n.second.WireConnect.size() << endl;
        for (int i=0; i<n.second.WireConnect.size(); i++){
            ofs << "\t\t" << "connect module: " << n.second.WireConnect[i].ModuleName << " wire num: " << n.second.WireConnect[i].connectNum << endl;
        }
        ofs << "\t************************************total_con num: " << n.second.total_con <<endl;

    }
    ofs  << endl << "FixedModule[] size: " << FixedModule_map.size() << endl << endl;
    for (const auto& n : FixedModule_map) {
        ofs << "key(name): " << n.first << " x: " << n.second.x <<  " y: " <<  n.second.y <<  " width: " <<  n.second.width << " height: " <<  n.second.height <<  endl ;

    }*/
//    ofs << endl << "connection_vec (for Graphviz):" << endl;
//    for(int i=0; i<softConnection_vec.size(); i++) {
//        if (softConnection_vec[i].num > avgWL) {
//            ofs << softConnection_vec[i].name1 << " -> " << softConnection_vec[i].name2  << " [dir=\"none\", label=\"" << softConnection_vec[i].num << "\"]" << endl;
//        }
//
//        //ofs << i+1 << ".   N1: " << connection_vec[i].name1 << "        N2: " << connection_vec[i].name2 << "       num: " << connection_vec[i].num << endl;
//    }

    /*ofs<<"GroupVec  num: "<<GroupVec.size()<< endl << endl;
    for(int i=0;i<GroupVec.size();i++)
    {
        ofs<<i+1<<".\t";
        for(int k=0;k<GroupVec[i].ModuleNameVec.size();k++)
        {
            ofs<<"#"<<k+1<<" "<<GroupVec[i].ModuleNameVec[k]<<"\t";
        }
        ofs<<endl;
    }*/
//    ofs_matlab.close();
//        int result ;
//        if(inputfileNumber == "1")  {result  = remove("case01_output.txt"); result  = rename("output_m.txt","case01_output.txt");}
//        else if(inputfileNumber == "2")  {result  = remove("case02_output.txt"); result  = rename("output_m.txt","case02_output.txt");}
//        else if(inputfileNumber == "3")  {result  = remove("case03_output.txt"); result  = rename("output_m.txt","case03_output.txt");}
//        else if(inputfileNumber == "4")  {result  = remove("case04_output.txt"); result  = rename("output_m.txt","case04_output.txt");}
//        else if(inputfileNumber == "5")  {result  = remove("case05_output.txt"); result  = rename("output_m.txt","case05_output.txt");}
//        else if(inputfileNumber == "6")  {result  = remove("case06_output.txt"); result  = rename("output_m.txt","case06_output.txt");}
    compute_HPWL();
//    ifstream ofs_in1(outputfileName,ios::in);
//    queue<string> tmp1;
//    string t1;
//    while(getline(ofs_in1, t1))
//    {
//        tmp1.push(t1);
//    }
//    ofs_in1.close();
//    ofstream ofs_out1(outputfileName,ios::out);
//    ofs_out1<<"HPWL "<<(int)HPWL<<endl;
//    ofs_out1<<"SOFTMODULE "<<SoftModule_map.size()<<endl;
//    while(!tmp1.empty())
//    {
//        t1 = tmp1.front();
//        tmp1.pop();
//        ofs_out1<<t1<<endl;
//    }
    ofstream ofs(outputfileName,ios::out);
    ofs<<fixed;
    ofs << setprecision(1) <<"HPWL = "<<HPWL<<endl;
    ofs<<"SOFTMODULE "<<SoftModule_map.size()<<endl;
    for (auto& n : SoftModule_map) {
        ofs <<n.first << " 4" << endl ;
        LegalTypeStruct temp;
        temp = n.second.LegalType.back();
        ofs << temp.position->head->coordinate.x << " " << temp.position->head->coordinate.y << endl;
        ofs << temp.position->head->coordinate.x << " " << temp.position->head->coordinate.y + temp.h<<endl;
        ofs << temp.w<< " " <<  temp.h<<endl;
        ofs << temp.position->head->coordinate.x + temp.w<< " " << temp.position->head->coordinate.y<<endl;
    }

    return 0;
}
