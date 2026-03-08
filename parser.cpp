#include <bits/stdc++.h>
#include "contour.h"
#include "myGraph.h"
#include "parser.h"
#include "TREE.h"
using namespace std;

int chip_width, chip_height, softModule_num, fixedModule_num, connection_num, ttlcon, matlab_module_num=0, nonPlaceNum=0;
//ofstream ofs("output.txt",ios::out);

map<string, SoftModule_struct> SoftModule_map;
map<string, FixedModule_struct> FixedModule_map;
vector<connection_struct> connection_vec;
vector<connection_struct> softConnection_vec;
//string inputfileNumber;
string inputfileName;

void parser()
{
//    ofs << "START parser \n";
//    string inputFile;
//    cout<<"which file you want to input? ";
//    cin>>inputfileNumber;
//    inputFile.append("case0");
//    inputFile.append(inputfileNumber);
//    inputFile.append("-input.txt");
    ifstream inFile (inputfileName, ios::in);

    string fileLine; //檔案一行一行讀進來

    if (!inFile)
        cout << "Failed to open inputfile.\n";
    // 讀檔案
    inFile >> fileLine >> chip_width >> chip_height;

    inFile >> fileLine >> softModule_num;
        // 存 SoftModule資訊
    for (int i=0; i<softModule_num; i++) {
        SoftModule_struct newModule;
        inFile >> newModule.name >> newModule.minArea_restrict;
        newModule.total_con=0;//*
        SoftModule_map[newModule.name] = newModule;
    }

    inFile >> fileLine >> fixedModule_num;
    for(int i=0; i<fixedModule_num; i++){
        FixedModule_struct newModule;
        inFile >> newModule.name >> newModule.x >> newModule.y >> newModule.width >> newModule.height;
//        ofs_matlab << newModule.name <<" "<< newModule.x <<" "<<  newModule.y << " "<< newModule.width <<" "<<  newModule.height<<endl;
        matlab_module_num++;
        newModule.coordinate.push_back({newModule.x, newModule.y});
        newModule.coordinate.push_back({newModule.x+newModule.width, newModule.y});
        newModule.coordinate.push_back({newModule.x+newModule.width, newModule.y+newModule.height});
        newModule.coordinate.push_back({newModule.x, newModule.y+newModule.height});
        FixedModule_map[newModule.name] = newModule;
    }

    inFile >> fileLine >> connection_num;
    for(int i=0; i<connection_num; i++){
        connection_struct newConnection;
        inFile >> newConnection.name1 >> newConnection.name2 >> newConnection.num;
        connection_vec.push_back(newConnection);

        // find in map
        auto iter = SoftModule_map.find(newConnection.name1);
        int flag=0; // 0: default, 1: 一個module是soft, 2: 兩個module是soft
        WireConnectStruct temp;
        temp.ModuleName = newConnection.name2;
        temp.connectNum = newConnection.num;
        if( iter != SoftModule_map.end() ) { // name1 is Soft
            SoftModule_map[newConnection.name1].WireConnect.push_back(temp);
            SoftModule_map[newConnection.name1].total_con+=temp.connectNum;
            flag++;
        }
        else { // name1 is Hard
            FixedModule_map[newConnection.name1].WireConnect.push_back(temp);
        }

        iter = SoftModule_map.find(newConnection.name2);
        temp.ModuleName = newConnection.name1;
        if( iter != SoftModule_map.end() ) { // name2 is Soft
            SoftModule_map[newConnection.name2].WireConnect.push_back(temp);
            SoftModule_map[newConnection.name2].total_con+=temp.connectNum;
            flag++;
        }
        else { // name2 is Hard
            FixedModule_map[newConnection.name2].WireConnect.push_back(temp);
        }
        // 存soft soft間連線
        if(flag == 2) {
            softConnection_vec.push_back(newConnection);
        }
    }

    inFile.close();

//    ofs << "END parser \n";
}


/////////////////////////////////////////////////////////

int avgWL;
vector<pair<string,SoftModule_struct>> SoftModule_vec;
vector<GroupVecStruct> GroupVec;

bool sort_f(connection_struct a,connection_struct b)
{
    return a.num>b.num;
}
bool sort_f1(WireConnectStruct a,WireConnectStruct b)
{
    return a.connectNum>b.connectNum;
}
bool sort_f2(pair<string,SoftModule_struct> a, pair<string,SoftModule_struct> b)
{
    return a.second.total_con>b.second.total_con;
}
bool sort_f3(WireConnectStruct a,WireConnectStruct b)
{
    return a.connectNum>b.connectNum;
}

void weightofwire()
{
//    ofs<<"start WW\n";

    //average wl
    int totalWL=0;
    for(int i=0; i<softConnection_vec.size(); i++)
        totalWL+=softConnection_vec[i].num;
    avgWL=totalWL/connection_vec.size();
//    ofs<<endl<<"avgWL = "<<avgWL<<endl;
    //ofs<<"conn_num = "<<connection_vec.size()<<endl;

    //sort
//    sort(connection_vec.begin(),connection_vec.end(),sort_f);
    for(auto& n : SoftModule_map)
        SoftModule_vec.push_back(n);
    sort(SoftModule_vec.begin(),SoftModule_vec.end(),sort_f2);
    for(auto& n : SoftModule_map)
        sort(SoftModule_map[n.second.name].WireConnect.begin(),SoftModule_map[n.second.name].WireConnect.end(),sort_f1);
    for(int n=0;n<SoftModule_vec.size();n++)
        sort(SoftModule_vec[n].second.WireConnect.begin(),SoftModule_vec[n].second.WireConnect.end(),sort_f1);
    /*for(int i=0;i<connection_vec.size();i++)
        ofs<<i+1<<".\tN1: "<<connection_vec[i].name1<<"\t\tN2: "<<connection_vec[i].name2<<"\t\tnum: "<<connection_vec[i].num<<endl;
    */
    /*
    for(int u=0;u<SoftModule_vec.size();u++)
    {
        ofs<<u+1<<". SoftModule_vec: "<<SoftModule_vec[u].first;
        ofs <<"\tConnection_vec Size: " << SoftModule_vec[u].second.WireConnect.size() << endl;
        for (int i=0; i<SoftModule_vec[u].second.WireConnect.size(); i++)
            ofs<<"\tmodule: "<<SoftModule_vec[u].second.WireConnect[i].ModuleName<<" wirenum: "<<SoftModule_vec[u].second.WireConnect[i].connectNum<<endl;
        //ofs << "*************************************************total_con num: "<<SoftModule_vec[u].second.total_con <<endl;
    }
     */

//    ofs<<"end WW"<<endl;
    return;
}

void groupmodule(int quan)//# of modules
{
//    ofs<<"start GM"<<endl;
    GroupVecStruct tempV;

    for(const auto& n : FixedModule_map)//1. Fixed
    {
        tempV.neighborModule=n.second.name;
        tempV.minarea=n.second.width*n.second.height;
        GroupVec.push_back(tempV);
    }

    for(int s=0;s<connection_vec.size();s++)//1. Fixed
    {
        if(connection_vec[s].num>=avgWL)
        {
            for(const auto& n : FixedModule_map)
            {
                for(int f=0;f<GroupVec.size();f++)
                {
                    if(connection_vec[s].name1==n.second.name && GroupVec[f].neighborModule==n.second.name && !SoftModule_map[connection_vec[s].name2].grouped && !connection_vec[s].grouped && GroupVec[f].ModuleNameVec.size()<quan)// || connection_vec[s].name2==n.second.name)
                    {
                    //ofs<<"&&&&&&&&&&&&&&&&\n"<<n.second.name<<"\t"<<connection_vec[s].name1<<"\t"<<connection_vec[s].name2<<endl;
                        connection_vec[s].grouped=1;
                        SoftModule_map[connection_vec[s].name2].grouped=1;
                        GroupVec[f].ModuleNameVec.push_back(connection_vec[s].name2);
                        GroupVec[f].minarea+=SoftModule_map[connection_vec[s].name2].minArea_restrict;
                        //for(int p=0;p<GroupVec[f]..ModuleNameVec.size();p++) ofs<<GroupVec[f].ModuleNameVec[p]<<" *** ";
                    }
                    else if(connection_vec[s].name2==n.second.name && GroupVec[f].neighborModule==n.second.name && !SoftModule_map[connection_vec[s].name1].grouped && !connection_vec[s].grouped && GroupVec[f].ModuleNameVec.size()<quan)
                    {
                        connection_vec[s].grouped=1;
                        SoftModule_map[connection_vec[s].name1].grouped=1;
                        GroupVec[f].ModuleNameVec.push_back(connection_vec[s].name1);
                        GroupVec[f].minarea+=SoftModule_map[connection_vec[s].name1].minArea_restrict;
                        //for(int p=0;p<GroupVec[f]..ModuleNameVec.size();p++) ofs<<GroupVec[f].ModuleNameVec[p]<<" *** ";
                    }
                }
            }
        }
    }
    //ofs<<"fixed##\n";

    for(int n=0;n<SoftModule_vec.size();n++)//2. Soft
    {
        if(!SoftModule_vec[n].second.grouped)
        {
            for(int s=0;s<connection_vec.size();s++)//SoftModule_vec[u].second.WireConnect.size()
            {
                if(!connection_vec[s].grouped && connection_vec[s].num>=avgWL && (SoftModule_vec[n].second.name==connection_vec[s].name1 || SoftModule_vec[n].second.name==connection_vec[s].name2))
                {
                    for(int j=0;j<GroupVec.size();j++)
                    {
                        if(GroupVec[j].neighborModule==connection_vec[s].name1 || GroupVec[j].neighborModule==connection_vec[s].name2)
                        {//ofs<<"&&&\n";//neighbor is grouped
                            if(GroupVec[j].ModuleNameVec.size()<quan)
                            {
                                if(GroupVec[j].neighborModule==connection_vec[s].name2 && !SoftModule_vec[n].second.grouped && !SoftModule_map[connection_vec[s].name1].grouped)
                                {
                                    GroupVec[j].ModuleNameVec.push_back(connection_vec[s].name1);
                                    SoftModule_vec[n].second.grouped=1;
                                    GroupVec[j].minarea+=SoftModule_map[connection_vec[s].name1].minArea_restrict;
                                }
                                else if(GroupVec[j].neighborModule==connection_vec[s].name1 && !SoftModule_vec[n].second.grouped && !SoftModule_map[connection_vec[s].name2].grouped)
                                {
                                    GroupVec[j].ModuleNameVec.push_back(connection_vec[s].name2);
                                    SoftModule_vec[n].second.grouped=1;
                                    GroupVec[j].minarea+=SoftModule_map[connection_vec[s].name2].minArea_restrict;
                                }
                            }
                            /*else// if(GroupVec[j].ModuleNameVec.size()==quan-1)
                            {
                                //GroupVec.push_back(tempV);
                                //while(tempV.ModuleNameVec.size()>0) tempV.ModuleNameVec.pop_back();//clear

                                GroupVecStruct tempV;
                                tempV.neighborModule=SoftModule_vec[n].second.name;
                                SoftModule_vec[n].second.grouped=1;
                                GroupVec.push_back(tempV);
                                //while(tempV.ModuleNameVec.size()>0) tempV.ModuleNameVec.pop_back();//clear
                            }*/
                        }
                        /*else
                        {ofs<<"&&&not grouped\n";
                            GroupVecStruct tempV;
                            tempV.neighborModule=SoftModule_vec[n].second.name;
                            SoftModule_vec[n].second.grouped=1;
                            GroupVec.push_back(tempV);
                            //while(tempV.ModuleNameVec.size()>0) tempV.ModuleNameVec.pop_back();//clear
                        }*/


                    }

                    if(!SoftModule_vec[n].second.grouped && !SoftModule_map[SoftModule_vec[n].second.name].grouped)
                    {
                        GroupVecStruct tempV;
                        tempV.neighborModule=SoftModule_vec[n].second.name;
                        SoftModule_vec[n].second.grouped=1;
                        tempV.minarea=SoftModule_map[SoftModule_vec[n].second.name].minArea_restrict;
                        GroupVec.push_back(tempV);
                    }
                    /*if(GroupVec[j].ModuleNameVec.size()==0)
                    {ofs<<"&&&size=0\n";
                        tempV.neighborModule=SoftModule_vec[n].second.name;
                        SoftModule_vec[n].second.grouped=1;
                        GroupVec.push_back(tempV);
                        while(tempV.ModuleNameVec.size()>0) tempV.ModuleNameVec.pop_back();//clear

                        /*ofs <<"%%%%GroupVec: "<<tempV.neighborModule;
                        for(int d=0;d<tempV.ModuleNameVec.size();d++)
                            ofs<<"#"<<d+1<<" "<<tempV.ModuleNameVec[d]<<"    ";
                        ofs<<endl;*/
                    /*}
                    else if(tempV.ModuleNameVec.size()==quan)
                    {ofs<<"###size=5\n";
                        GroupVec.push_back(tempV);
                        while(tempV.ModuleNameVec.size()>0) tempV.ModuleNameVec.pop_back();//clear

                        tempV.neighborModule=SoftModule_vec[n].second.name;
                        SoftModule_vec[n].second.grouped=1;
                        GroupVec.push_back(tempV);
                        while(tempV.ModuleNameVec.size()>0) tempV.ModuleNameVec.pop_back();//clear
                    }
                    else
                    {ofs<<"%%%size=\n";
                        if(SoftModule_vec[n].second.name==connection_vec[s].name1)//has connection
                        {
                            for(int j=0;j<GroupVec.size();j++)
                            {
                                if(GroupVec[j].neighborModule == connection_vec[s].name2)
                                {
                                    GroupVec[j].ModuleNameVec.push_back(connection_vec[s].name1);
                                    SoftModule_vec[n].second.grouped=1;
                                }
                            }

                            if(!SoftModule_vec[n].second.grouped)
                            {ofs<<" +++ \n";
                                tempV.ModuleNameVec.push_back(SoftModule_vec[n].second.name);
                                SoftModule_vec[n].second.grouped=1;
                                connection_vec[s].grouped=1;
                                GroupVec.push_back(tempV);
                                while(tempV.ModuleNameVec.size()>0) tempV.ModuleNameVec.pop_back();//clear
                            }
                        }
                        else if(SoftModule_vec[n].second.name==connection_vec[s].name2)
                        {
                            for(int j=0;j<GroupVec.size();j++)
                            {
                                if(GroupVec[j].neighborModule == connection_vec[s].name1)
                                {
                                    GroupVec[j].ModuleNameVec.push_back(connection_vec[s].name2);
                                    SoftModule_vec[n].second.grouped=1;
                                }
                            }

                            if(!SoftModule_vec[n].second.grouped)
                            {ofs<<" @@@ \n";
                                tempV.ModuleNameVec.push_back(SoftModule_vec[n].second.name);
                                SoftModule_vec[n].second.grouped=1;
                                connection_vec[s].grouped=1;
                                GroupVec.push_back(tempV);
                                while(tempV.ModuleNameVec.size()>0) tempV.ModuleNameVec.pop_back();//clear
                            }
                        }

                        //for(int p=0;p<tempV.ModuleNameVec.size();p++) ofs<<tempV.ModuleNameVec[p]<<" *** ";
                    }*/
                    //connection_vec[s].grouped=1;//ofs<<" ### \n";
                }
            }
        }
    }
    //if(tempV.ModuleNameVec.size()>0)
        //GroupVec.push_back(tempV);

    /*ofs<<"/////////////////////////////////"<<endl;
    for(int n=0;n<SoftModule_vec.size();n++)//2. Soft
    {
        ofs<<n+1<<".  NAME: "<<SoftModule_vec[n].second.name<<" G? "<<SoftModule_vec[n].second.grouped<<"\n";
    }
    ofs<<"/////////////////////////////////"<<endl;*/

//    for(int f=0;f<GroupVec.size();f++)
//    {
//        ofs <<f+1<<".  GroupVec: "<<GroupVec[f].neighborModule<<"\t min area: "<<GroupVec[f].minarea<<"\n\t";
//        for(int d=0;d<GroupVec[f].ModuleNameVec.size();d++)
//            ofs<<"#"<<d+1<<" "<<GroupVec[f].ModuleNameVec[d]<<"    ";
//        ofs<<endl;
//    }
//    ofs<<"end GM"<<endl;
    return;
}
float HPWL;
void compute_HPWL()
{
    HPWL = 0.0;
    map<string, SoftModule_struct>::iterator iter1, iter2;
    for(int i=0; i<connection_vec.size(); i++)
    {
        float center1_x, center1_y, center2_x, center2_y;

        iter1 = SoftModule_map.find(connection_vec[i].name1);
        iter2 = SoftModule_map.find(connection_vec[i].name2);
        if(iter1 == SoftModule_map.end())
        {
            center1_x = FixedModule_map[connection_vec[i].name1].x/1.0 + FixedModule_map[connection_vec[i].name1].width/2.0;
            center1_y = FixedModule_map[connection_vec[i].name1].y/1.0 + FixedModule_map[connection_vec[i].name1].height/2.0;
//            cout<<connection_vec[i].name1<<" "<<center1_x<<" "<<center1_y<<endl;
        }
        else
        {
                center1_x = SoftModule_map[connection_vec[i].name1].LegalType[0].centerX;
                center1_y = SoftModule_map[connection_vec[i].name1].LegalType[0].centerY;
//                cout<<connection_vec[i].name1<<" "<<SoftModule_map[connection_vec[i].name1].LegalType[0].centerX<<" "<<SoftModule_map[connection_vec[i].name1].LegalType[0].centerY<<endl;
        }

        if(iter2 == SoftModule_map.end())
        {
            center2_x = FixedModule_map[connection_vec[i].name2].x/1.0 + FixedModule_map[connection_vec[i].name2].width/2.0;
            center2_y = FixedModule_map[connection_vec[i].name2].y/1.0 + FixedModule_map[connection_vec[i].name2].height/2.0;
//            cout<<connection_vec[i].name2<<" "<<center2_x<<" "<<center2_y<<endl;
        }
        else
        {
                center2_x = SoftModule_map[connection_vec[i].name2].LegalType[0].centerX;
                center2_y = SoftModule_map[connection_vec[i].name2].LegalType[0].centerY;
//                cout<<connection_vec[i].name2<<" "<<center2_x<<" "<<center2_y<<endl;
        }
        HPWL+=(abs(center1_x-center2_x)+abs(center1_y-center2_y))*float(connection_vec[i].num);
    }

    cout<<fixed;
    cout << setprecision(1) <<"HPWL = "<<HPWL<<endl;
}
float rounding(float num, int index)
{
    bool isNegative = false; // whether is negative number or not

    if(num < 0) // if this number is negative, then convert to positive number
    {
        isNegative = true;
        num = -num;
    }

    if(index >= 0)
    {
        int multiplier;
        multiplier = pow(10, index);
        num = (int)(num * multiplier + 0.5) / (multiplier * 1.0);
    }

    if(isNegative) // if this number is negative, then convert to negative number
    {
        num = -num;
    }

    return num;
}
