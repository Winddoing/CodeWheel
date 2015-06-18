#include <iostream>

using namespace std;

class A
{
private:
	int a;
	int b;
};

int main(int argc, char *argv[])
{
	A a;
	A *b = new A();

	cout<<"sizeof() = "<<sizeof(a)<<endl;
	cout<<"sizeof() = "<<sizeof(b)<<endl;

	return 0;
}

