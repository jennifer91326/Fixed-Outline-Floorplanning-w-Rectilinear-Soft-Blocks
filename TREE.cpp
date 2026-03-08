
#include "myGraph.h"
#include "TREE.h"
#include "parser.h"
#include <bits/stdc++.h>
using namespace std;

int TreeNode:: GetNodeID()
{
    return m_nNodeID;
}

int TreeNode:: GetChildNum()
{
    return (int)m_vecChild.size();
}

TreeNode* TreeNode:: GetChild(int v_id)
{
    if (v_id < 0 || v_id >= (int)m_vecChild.size())
        return 0;
    return m_vecChild[v_id];
}

bool TreeNode:: AddChild(TreeNode* pNewChild)
{
    bool bHave = false;        //是否已經有這個子節點

    TreeNode* pChild = 0;
    for (auto& it : m_vecChild) {
        pChild = it;
        if (pChild->GetNodeID() == pNewChild->GetNodeID()) {
            bHave = true;
            break;
        }
    }

    if (bHave)
        return false;
    m_vecChild.push_back(pNewChild);
    return true;
}

bool TreeNode:: SetParent(TreeNode* pParent)
{
    if (GetParent() != 0)    //已經有父節點了
        return false;        //error
    m_pParent = pParent;
    return true;
}

TreeNode* TreeNode:: GetParent()
{
    return m_pParent;
}

void TreeNode:: SetName(string moduleName)
{
    m_nNodeName = moduleName;
}

string TreeNode:: GetName()
{
    return m_nNodeName;
}

void TreeNode:: Print(int level)
{
    for (int i = 0; i < level; i++) {
//        ofs << "-";
        m_level++;
    }
//    ofs << m_nNodeName << endl;

    TreeNode* pChild = 0;
    for (auto& it : m_vecChild) {
        pChild = it;
        pChild->Print(level + 1);
    }
}

/* 取得節點 */
TreeNode* Tree::GetNode(int nNodeID)
{
    if (m_mapNode.find(nNodeID) != m_mapNode.end())        //節點已存在
        return m_mapNode[nNodeID];

    //create node
    TreeNode* newNode = new TreeNode(nNodeID);
    m_mapNode[nNodeID] = newNode;
    return newNode;
}

/* 加入節點 */
bool Tree::AddChild(int nParent, int nChild)
{
    TreeNode* pParent = GetNode(nParent);
    TreeNode* pChild = GetNode(nChild);

    if (pChild->GetParent() != 0) {    //已經有父節點
        pParent = GetNode(nChild);
        pChild = GetNode(nParent);
        if (pChild->GetParent() != 0) {    //已經有父節點
            return false;
        }
    }

    if (pParent->AddChild(pChild) == false)    //已經有這個子節點
        return false;

    pChild->SetParent(pParent);

    if (pParent->GetParent() == 0)
        m_pRoot = pParent;

    return true;
}

/* 加入edge */
bool Tree::AddEdge(string nParent, string nChild, int weight)
{
    MSTedge_map[make_pair(nParent, nChild)] = weight;
    return true;
}

 /* 取得葉子節點 & return數量 */
int Tree::GetLeafNum()
{
    int nLeafNum = 0;
    TreeNode* pNode;
    for (auto& it : m_mapNode) {
        pNode = it.second;
        if (pNode->GetChildNum() == 0) {
            nLeafNum++;
            leafNode_vec.push_back(pNode);
        }
    }
    leafNode_vec.push_back(m_pRoot);
    return nLeafNum;
}

/* 取得根節點 */
int Tree::GetRoot()
{
    if (m_pRoot == 0)
        return -1;
    return m_pRoot->GetNodeID();
}

/* 取得某節點的孩子數量 */
int Tree::GetChildNum(int nNodeID)
{
    TreeNode* pNode = GetNode(nNodeID);
    if (pNode == 0)
        return -1;
    return pNode->GetChildNum();
}

/* 取得某節點的高度 */
int Tree::Distance(int nNodeID)
{
    TreeNode* pNode = GetNode(nNodeID);
    if (pNode == 0)
        return 0;

    int nChildNum = GetChildNum(nNodeID);
    if (nChildNum == 0)
        return 0;

    int nDepthMax = 0;
    int nDepth = 0;
    TreeNode* pChild = 0;

    for (int k = 0; k < nChildNum; k++) {
        pChild = pNode->GetChild(k);
        nDepth = Distance(pChild->GetNodeID()) + 1;
        if (nDepth > nDepthMax)
            nDepthMax = nDepth;
    }
    return nDepthMax;
}

/* 取得Level Order */
void Tree::GetLevelOrder()
{
    // !要先執行PrintTree()取得每個節點在哪個level!
    for (auto& itr : m_mapNode) {
        mapLevelOrder[itr.second->m_level].push_back(itr.second);
    }

    // print
//    for (auto& itr : mapLevelOrder) {
//        ofs << "#level " << itr.first << ": ";
//        for (auto& itr_vec : itr.second) {
//            ofs << itr_vec->m_nNodeName << " ";
//        }
//        ofs << endl;
//    }
}

/* 印出 */
void Tree::PrintTree(int nNodeID)
{
    TreeNode* pNode = GetNode(nNodeID);
    if (pNode == 0)
        return;
    pNode->Print(0);
}
void Tree::PrintLeafNode()
{
//    ofs << "leaf node : ";
//    for (auto& itr : leafNode_vec) {
//        ofs << itr->m_nNodeName << " ";
//    }
//    ofs << endl;
}

/* 加入module名 */
void Tree::SetName(int nNodeID, string moduleName)
{
    TreeNode* pNode = GetNode(nNodeID);
    if (pNode == 0)
        return;
    pNode->SetName(moduleName);
}

string Tree::GetName(int nNodeID)
{
    TreeNode* pNode = GetNode(nNodeID);
    if (pNode == 0)
        return "error";
    return pNode->GetName();
}

void Tree::Destroy()
{
    TreeNode* pNode;
    for (auto& it : m_mapNode) {
        pNode = it.second;
        if (pNode != 0) {
            delete pNode;
            pNode = 0;
        }
    }
    m_mapNode.clear();
}
