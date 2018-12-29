/*************************************************************************
	> File Name: tree.cpp
	> Author:llb 
	> Mail:llb1008x@126.com 
	> Created Time: 2018年07月29日 星期日 00时29分35秒
 ************************************************************************/

#include <iostream>
#include "tree.h"

using namespace std;

Tree::Tree(int size)
{
    m_iSize=size;
    m_pTree=new int[size];
    
    for(int i=0;i<size;i++){
        
        m_pTree[i]=0;
    }
}


Tree::~Tree()
{
    delete []m_pTree;
    m_pTree=NULL;
}


int *Tree::SearchNode(int nodeIndex)
{
    if( nodeIndex<0 || (nodeIndex >= m_iSize) ){
        return NULL;        
    }
    
    if(m_pTree[nodeIndex]==0);
        return NULL;
        
    return &m_pTree[nodeIndex];    
}



bool Tree::AddNode(int nodeIndex,int direction,int *pNode)
{
    if( nodeIndex<0 || (nodeIndex >= m_iSize) ){
        return false;        
    }
    
    if(m_pTree[nodeIndex]==0);
        return false;
    
    if( direction == 0 ){
        
        m_pTree[nodeIndex*2+1] = *pNode;
    }else if(direction == 1){
        m_pTree[nodeIndex*2+2] = *pNode;
    }
    
    return true;
}



bool Tree::DeleteNode(int nodeIndex,int *pNode)
{   
    if( nodeIndex<0 || (nodeIndex >= m_iSize) ){
        return false;        
    }
    
    if(m_pTree[nodeIndex]==0);
        return false;
    
    *pNode = m_pTree[nodeIndex];
    m_pTree[nodeIndex]=0;
    return true;
}


void Tree::TreeTraverse()
{
    for(int i=0;i<m_iSize;i++){
        cout << m_pTree[i] << " ";
    }
}


int main()
{
    return 0;
}

