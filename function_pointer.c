#include <iostream>

using namespace std;


int foo(int x, int y)
{
    int a = x+y;
}

int main()
{
    int (*fcnPtr)(int,int) = foo; // assign fcnPtr to function foo
    (*fcnPtr)(5,6); // call function foo(5) through fcnPtr.

    return 0;
}