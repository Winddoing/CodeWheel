#include <iostream>
#include <iomanip>

using namespace std;

#define INITWEIGHT 0

//用矩阵实现图
class graph
{
    private:
      bool isWeighted;  //是否带权？
      bool isDirected;  //是否有向？
      int numV; //顶点数
      int numE; //边数
      int **matrix; //邻接矩阵

    public:
      graph(int numV,bool isWeighted=false,bool isDirected=false);
      void createGraph();
      ~graph();
      int getVerNums()
      {
          return numV;
      }
      int getEdgeNums()
      {
          return numE;
      }
      int **getWeight()
      {
          return matrix;
      }
      void setEdgeWeight(int beg,int end,int weight);
      void printAdjacentMatrix();
      //检查输入
      bool check(int i,int j,int w=1);
};

//类的实现
graph::graph(int numV,bool isWeighted,bool isDirected)
{
    while(numV<=0)
    {
        cout<<"Vertex is wrong! Please enter again! "<<endl;
        cin>>numV;
    }
    this->numV=numV;
    this->isWeighted=isWeighted;
    this->isDirected=isDirected;
    //private之后的成员可以被类的成员函数访问，但是不能够被使用该类的代码访问
    matrix=new int *[numV];

    for(int i=0;i<numV;i++)
      matrix[i]=new int [numV];

    //对图进行初始化
    if(!isWeighted)  //无权图
    {
        //对所有的权值初始化为0
        for(int i=0;i<numV;i++)
          for(int j=0;j<numV;j++)
            matrix[i][j]=0;
    }
    else  //有权图
    {
        for(int i=0;i<numV;i++)
          for(int j=0;j<numV;j++)
            matrix[i][j]=INITWEIGHT;
    }
}

//建图
void graph::createGraph()
{
    cout<<"input edges: "<<endl;
    while(cin>>numE && numE<0)
      cout<<"wrong input! "<<endl;

    int i,j,w;

    if(!isWeighted)  //无权图
    {
        if(!isDirected)  //无向图
        {
            cout<<"Input begin and end "<<endl;
            for(int k=0;k<numE;k++)
            {
                cin>>i>>j;
                while(!check(i,j))
                {
                    cout<<"wrong edges, input again: "<<endl;
                    cin>>i>>j;
                }
                matrix[i][j]=matrix[j][i]=1;
            }
        }
        else
        {
            cout<<"enter begin and end "<<endl;
            for(int k=0;k<numE;k++)
            {
                cin>>i>>j;
                while(!check(i,j))
                {
                    cout<<"wrong edges, input again: "<<endl;
                    cin>>i>>j;
                }
                matrix[i][j]=1;
            }
        }
    }

    else  //有权图
    {
        if(!isDirected)  //无向图
        {
            cout<<"enter begin, end, and weight: "<<endl;
            for(int k=0;k<numE;k++)
            {
                cin>>i>>j>>w;
                while(!check(i,j,w))
                {
                    cout<<"wrong edges,input again: "<<endl;
                    cin>>i>>j>>w;
                }
                matrix[i][j]=matrix[j][i]=w;
            }
        }
        else
        {
            cout<<"begin, end, and weight: "<<endl;
            for(int k=0;k<numE;k++)
            {
                cin>>i>>j>>w;
                while(!check(i,j,w))
                {
                    cout<<"wrong edges, input again: "<<endl;
                    cin>>i>>j>>w;
                }
                matrix[i][j]=w;
            }
        }
    }
}

graph::~graph()  //析构函数
{
    for(int i=0;i<numV;i++)
      delete[] matrix[i];
    delete[] matrix;
}

//设置指定边权值：
void graph::setEdgeWeight(int beg,int end,int weight)
{
    if(isWeighted)
    {
        while(!check(beg,end,weight))
        {
            cout<<"wrong input, input again:"<<endl;
            cin>>beg>>end>>weight;
        }

        if(isDirected)
          matrix[beg][end]=weight;
        else
          matrix[beg][end]=matrix[end][beg]=weight;
    }

    else
    {
        while(!check(beg,end,1))
        {
            cout<<"wrong input, input again: "<<endl;
            cin>>beg>>end;
        }
        if(isDirected)  //对邻接矩阵的值进行反转，重置，1变成0，0变成1
          matrix[beg][end]=1-matrix[beg][end];
        else
          matrix[beg][end]=matrix[end][beg]=1-matrix[beg][end];
    }
}

//输入检查
bool graph::check(int i,int j,int w)
{
    if(i>=0 && i<numV && j>=0 && j<numV && w>0)
      return true;
    else
      return false;
}

void graph::printAdjacentMatrix()
{
    cout.setf(ios::left);
    cout<<setw(4)<<" ";

    for(int i=0;i<numV;i++)
      cout<<setw(4)<<i;
    cout<<endl;

    for(int i=0;i<numV;i++)
    {
        cout<<setw(4)<<i;
        for(int j=0;j<numV;j++)
          cout<<setw(4)<<matrix[i][j];

        cout<<endl;
    }
}
