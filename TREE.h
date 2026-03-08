
#ifndef TREE_h
#define TREE_h
#include <bits/stdc++.h>
using namespace std;

class TreeNode
{
public:
    TreeNode(int nNodeID)
    {
        m_pParent = 0;
        m_nNodeID = nNodeID;
        m_level = 0;
    }
    ~TreeNode() {}
    int m_nNodeID, m_level;
    TreeNode* m_pParent;
    vector<TreeNode*> m_vecChild;
    string m_nNodeName;

    int GetNodeID();

    int GetChildNum();

    TreeNode* GetChild(int v_id);

    bool AddChild(TreeNode* pNewChild);

    bool SetParent(TreeNode* pParent);

    TreeNode* GetParent();

    void SetName(string moduleName);

    string GetName();

    void Print(int level);

    friend class Tree; // 讓class Tree能夠存取TreeNode的private data
};

typedef pair<string, string> namePair;
class Tree
{
public:
    Tree() = default;
    ~Tree();
    TreeNode* m_pRoot;
    map<int, TreeNode*> m_mapNode;
    map<namePair, int> MSTedge_map;
    vector<TreeNode*> leafNode_vec;
    map<int, vector<TreeNode*>> mapLevelOrder;
private:
    /* 取得節點 */
    TreeNode* GetNode(int nNodeID);

public:
    /* 加入節點 */
    bool AddChild(int nParent, int nChild);

    /* 加入edge */
    bool AddEdge(string nParent, string nChild, int weight);

     /* 取得葉子節點 & return數量 */
    int GetLeafNum();

    /* 取得根節點 */
    int GetRoot();

    /* 取得某節點的孩子數量 */
    int GetChildNum(int nNodeID);

    /* 取得某節點的高度 */
    int Distance(int nNodeID);
    
    /* 取得Level Order */
    void GetLevelOrder();

    /* 印出 */
    void PrintTree(int nNodeID);
    
    void PrintLeafNode();

    /* 加入module名 */
    void SetName(int nNodeID, string moduleName);

    string GetName(int nNodeID);

    void Destroy();
};


#endif /* TREE_h */
