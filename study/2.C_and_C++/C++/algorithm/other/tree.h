/*************************************************************************
	> File Name: tree.h
	> Author:llb 
	> Mail:llb1008x@126.com 
	> Created Time: 2018年07月29日 星期日 00时29分41秒
 ************************************************************************/

#ifndef _TREE_H
#define _TREE_H


class Tree{

public:
    Tree(int size);
    ~Tree();
    int *SearchNode(int nodeIndex);
    bool AddNode(int nodeIndex,int direction,int *pNode);
    bool DeleteNode(int nodeIndex,int *pNode);
    void TreeTraverse();
    
private:
    int *m_pTree; 
    int m_iSize;   
};

#endif
