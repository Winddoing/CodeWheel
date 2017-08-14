#include "Graphic_longest_path.h"
#include <iostream>
#include <vector>

#define INFINITY 0x7fffffff

int max(int a,int b)
{
    return a>b?a:b;
}

int Find_longest_path(int **weight,int numVertexes,int k,int t,vector<int> &r)  //寻找k到t的最短路径
{

    if(r[k]>=0)
        return r[k];
    if(k==t)
    {
        int q=0;
        r[k]=q;
        return q;
    }
    else
    {
        int q=weight[k][t];
        for(int i=k+1;i<=t && weight[k][i];i++)
        {
            q=max(q,weight[k][i]+Find_longest_path(weight,numVertexes,i,t,r));
        }
        r[k]=q;
        return q;
    }

}

int dynamic_longest_path(int **weight,int numVertexes,int k,int t)
{
    vector<int> r;
    r.resize(numVertexes);

    for(int i=0;i<numVertexes;i++)
        r[i]=-INFINITY;

    return Find_longest_path(weight,numVertexes,k,t,r);
}
