#include "dynamic_longest_path.h"
#include <iostream>

int main()
{
    cout<<"AdjacentMatrix Graphic: "<<endl;
    bool isDirected, isWeighted;
    int numV;
    cout<<"Create Graphics: "<<endl;
    cout<<"input Vertexes: ";
    cin>>numV;

    cout<<"Is weighted? 0(no), 1(yes) : ";
    cin>>isWeighted;
    cout<<"IS directed? 0(no), 1(yes) : ";
    cin>>isDirected;

    graph graph(numV,isWeighted,isDirected);
    cout<<"This is a ";
    isDirected ? cout<<"Directed " : cout<<"Undirected: ";
    isWeighted ? cout<<"Weighted " <<endl : cout<<"Unweighted "<<endl;
    graph.createGraph();

    cout<<"print AdjacentMatrix: "<<endl;
    graph.printAdjacentMatrix();
    cout<<endl;

    int k,t;
    cout<<"input k, t :"<<endl;
    cin>>k>>t;

    int numVertex=graph.getVerNums();
    int **weight_dynamic=graph.getWeight();
    cout<<"test: ";
    cout<<weight_dynamic[k][t]<<endl;
    int result=dynamic_longest_path(weight_dynamic,numVertex,k,t);
    cout<<"The result is :"<<endl;
    cout<<result<<endl;

    int beg, end, weight;
    bool flag;
    cout<<"Adjust the weight, no(0), yes(1): "<<endl;
    cin>>flag;
    if(flag)
    {
        if(isWeighted)
        {
            cout<<"Enter edges--begin, end, and weight: "<<endl;
            cin>>beg>>end>>weight;
            graph.setEdgeWeight(beg,end,weight);
        }
        else
        {
            cout<<"Enter edges--begin, end: "<<endl;
            cin>>beg>>end;
            graph.setEdgeWeight(beg,end,1);
        }
        cout<<"Successed!"<<endl;
        cout<<"Print AdjacentMatrix: "<<endl;
        graph.printAdjacentMatrix();
    }
    return 0;

}
